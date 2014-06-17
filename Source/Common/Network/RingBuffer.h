#ifndef __RingBuffer_h__
#define __RingBuffer_h__

namespace Network
{
	/// 初始Head = 0, Tail = 1
	/// Tail - Head = 1时为空
	/// Tail - Head = 0时为满
	/// 0 <= Head < Size
	/// 0 <= Tail < Size
	/// 保持对m_uiHead, m_uiTail只进行原子操作
	/// Tail指向的元素是直接可写的元素
	/// Head + 1指向的元素是直接可读的元素

	template<typename T>
	class RingBuffer
	{
	public:
		/// @param uiSize 容量
		/// @param uiMaxPacketSize 最大包大小
		RingBuffer(unsigned int uiSize, unsigned int uiMaxPacketSize)
			: m_uiTotalSize(uiSize + 1), m_uiMaxPacketSize(uiMaxPacketSize),
			m_uiHead(0), m_uiTail(1), m_bReading(false), m_bWritting(false),
			m_uiPacketFromBufferHeadSize(0)
		{
			/// 多申请m_uiMaxPacketSize用于@see readPacketBegin
			/// 和@see readPacketEnd的实现
			m_pkBuffer = new T[m_uiTotalSize + m_uiMaxPacketSize];
		}
		/// 清空缓存
		~RingBuffer()
		{
			delete[] m_pkBuffer;
		}

		void clear()
		{
			m_uiHead = 0;
			m_uiTail = 1;
		}

		/// 将pkBuffer内的uiSize复制到RingBuffer，有可数据分布在RingBuffer头尾而不连续
		/// 该函数只修改m_uiTail不修改m_uiHead，函数内m_uiHead可能会被其它进行修改
		bool writeImmediately(T *pkBuffer, unsigned int uiSize)
		{
			unsigned int uiHead = m_uiHead;
			if (uiSize > writableSize(uiHead, m_uiTail) || !pkBuffer)
			{
				return false;
			}
			unsigned int uiNextTail = 0;
			if (m_uiTail > uiHead)
			{
				unsigned int uiDivider = m_uiTotalSize - m_uiTail;
				if (uiDivider > uiSize)
				{
					memcpy(m_pkBuffer + m_uiTail, pkBuffer, uiSize * sizeof(T));
					uiNextTail = m_uiTail + uiSize;
				}
				else if (uiDivider < uiSize)
				{
					memcpy(m_pkBuffer + m_uiTail, pkBuffer, uiDivider * sizeof(T));
					uiNextTail = uiSize - uiDivider;
					memcpy(m_pkBuffer, pkBuffer + uiDivider, uiNextTail * sizeof(T));
				}
				else //if (uiDivider == uiSize)
				{
					memcpy(m_pkBuffer + m_uiTail, pkBuffer, uiSize * sizeof(T));
					uiNextTail = 0;
				}
			}
			else
			{
				memcpy(m_pkBuffer + m_uiTail, pkBuffer, uiSize * sizeof(T));
				uiNextTail = m_uiTail + uiSize;
			}
			m_uiTail = uiNextTail;
			return true;
		}
		/// 取出RingBuffer连续可写的起始地址及可写的大小，然后对pkBuffer填写数据
		/// 结束需要调用@see writeEnd
		/// 如果正在写才返回false
		/// 不修改m_uiHead，不修改m_uiTail，函数内m_uiHead可能会被其它线程修改
		bool writeBegin(T *&pkBuffer, unsigned int &uiWritableSize)
		{
			if (m_bWritting)
			{
				return false;
			}
			m_bWritting = true;
			unsigned int uiHead = m_uiHead;
			if (m_uiTail == m_uiTotalSize)
			{
				pkBuffer = m_pkBuffer;
			}
			else
			{
				pkBuffer = m_pkBuffer + m_uiTail;
			}
			uiWritableSize = consequentMemWritableSize(uiHead, m_uiTail);
			return true;
		}
		/// 对应@see writeBegin, 告诉RingBuffer写入了多少数据@param uiWroteSize
		/// 不修改m_uiHead，修改m_uiTail，函数内m_uiHead可能会被其它线程修改
		void writeEnd(unsigned int uiWroteSize)
		{
			m_uiTail = (m_uiTail + uiWroteSize) % m_uiTotalSize;
			m_bWritting = false;
		}
		/// 读取最多uiBuffSize数据量，有可能读出的数据在RingBuffer中不是连续的内存块
		/// 修改m_uiHead，不修改m_uiTail，函数内m_uiTail可能会被其它线程修改
		unsigned int readImmediately(T *pkBuffer, unsigned int uiBuffSize)
		{
			unsigned int uiTail = m_uiTail;
			unsigned int uiReadableSize = readableSize(m_uiHead, uiTail);
			if (uiReadableSize == 0 || !pkBuffer)
			{
				return 0;
			}
			unsigned int uiSizeToRead = uiReadableSize < uiBuffSize ? uiReadableSize : uiBuffSize;
			unsigned int uiNextHead = 0;
			if (uiTail > m_uiHead)
			{
				memcpy(pkBuffer, m_pkBuffer + (m_uiHead + 1), uiSizeToRead * sizeof(T));
				uiNextHead = m_uiHead + uiSizeToRead;
			}
			else
			{
				unsigned int uiDivider = m_uiTotalSize - (m_uiHead + 1);
				if (uiSizeToRead <= uiDivider)
				{
					memcpy(pkBuffer, m_pkBuffer + (m_uiHead + 1), uiSizeToRead * sizeof(T));
					uiNextHead = m_uiHead + uiSizeToRead;
				}
				else if (uiSizeToRead > uiDivider)
				{
					memcpy(pkBuffer, m_pkBuffer + (m_uiHead + 1), uiDivider * sizeof(T));
					uiNextHead = uiSizeToRead - uiDivider;
					memcpy(pkBuffer + uiDivider, m_pkBuffer, uiNextHead * sizeof(T));
					uiNextHead -= 1;
				}
			}
			m_uiHead = uiNextHead;
			return uiSizeToRead;
		}
		/// 取出连续可以读的内存地址@param pkBuffer及可读的大小@param uiReadableSize
		/// 从@param pkBuffer读取数据最大@param uiReadableSize后需要调用readEnd
		/// 如果正在读才返回false
		/// 不修改m_uiHead，不修改m_uiTail，函数内m_uiTail可能会被其它线程修改
		bool readBegin(T *&pkBuffer, unsigned int &uiReadableSize)
		{
			if (m_bReading)
			{
				return false;
			}
			unsigned int uiTail = m_uiTail;
			m_bReading = true;
			if (m_uiHead + 1 == m_uiTotalSize)
			{
				pkBuffer = m_pkBuffer;
			}
			else
			{
				pkBuffer = m_pkBuffer + (m_uiHead + 1);
			}
			uiReadableSize = consequentMemReadableSize(m_uiHead, uiTail);
			return true;
		}
		/// 对应@see readBegin, 告诉RingBuffer读了多少数据@param uiReadSize
		/// 修改m_uiHead，不修改m_uiTail，函数内m_uiTail可能会被其它线程修改
		void readEnd(unsigned int uiReadSize)
		{
			m_uiHead = (m_uiHead + uiReadSize) % m_uiTotalSize;
			m_bReading = false;
		}
		/// 不一定是连续的可读内存大小
		unsigned int readableSize()
		{
			return readableSize(m_uiHead, m_uiTail);
		}
		/// 不一定是连续的可写内存大小
		unsigned int writableSize()
		{
			return writableSize(m_uiHead, m_uiTail);
		}
		/// RingBuffer容量
		unsigned int capacity()
		{
			return m_uiTotalSize - 1;
		}
		/// 是否调用了@see readBegin，还未调用@see readEnd
		bool isReading()
		{
			return m_bReading;
		}
		/// 是否调用了@see writeBegin，还未调用@see writeEnd
		bool isWritting()
		{
			return m_bWritting;
		}
		/// 包处理时取出保证至少包含一个数据包的连续内存地址
		/// 需要与@see readPacketEnd对应调用
		/// 包的前sizeof(unsigned int)个字节表示包的大小
		/// 只有当前正在读返回false, uiReadableSize置为0
		/// 如果当前没有完整的数据包返回true, uiReadableSize置为0
		bool readPacketsBegin(T *&pkBuffer, unsigned int &uiReadableSize)
		{
			if (m_bReading)
			{
				return false;
			}
			uiReadableSize = 0;
			unsigned int uiTail = m_uiTail;
			m_bReading = true;
			unsigned int uiTotalReadableSize = readableSize(m_uiHead, uiTail);
			const unsigned int uiBitOfSize = sizeof(unsigned int);
			if (uiTotalReadableSize <= uiBitOfSize)
			{
				return true;
			}
			m_uiPacketFromBufferHeadSize = 0;
			unsigned int uiConsequentMemReadableSize = consequentMemReadableSize(m_uiHead, uiTail);
			if (uiConsequentMemReadableSize < uiBitOfSize)
			{
				m_uiPacketFromBufferHeadSize = uiBitOfSize - uiConsequentMemReadableSize;
#ifdef WIN32
				memcpy_s(m_pkBuffer + m_uiHead + 1 + uiConsequentMemReadableSize, m_uiMaxPacketSize,
					m_pkBuffer, m_uiPacketFromBufferHeadSize);
#else
				memcpy(m_pkBuffer + m_uiHead + 1 + uiConsequentMemReadableSize,
					m_pkBuffer, m_uiPacketFromBufferHeadSize);
#endif // WIN32
			}
			unsigned int uiCurrentPacketSize = 0;
			if (m_uiTotalSize - m_uiHead == 1)
			{
				uiCurrentPacketSize = *((unsigned int*)m_pkBuffer);
				pkBuffer = m_pkBuffer;
			}
			else
			{
				uiCurrentPacketSize = *((unsigned int*)(m_pkBuffer + m_uiHead + 1));
				pkBuffer = m_pkBuffer + m_uiHead + 1;
			}
			if (uiCurrentPacketSize > uiTotalReadableSize)
			{
				m_uiPacketFromBufferHeadSize = 0;
				return true;
			}
			if (uiCurrentPacketSize > uiConsequentMemReadableSize)
			{
				m_uiPacketFromBufferHeadSize = uiCurrentPacketSize - uiConsequentMemReadableSize;
#ifdef WIN32
				memcpy_s(m_pkBuffer + m_uiHead + 1 + uiConsequentMemReadableSize, m_uiMaxPacketSize,
					m_pkBuffer, m_uiPacketFromBufferHeadSize);
#else
				memcpy(m_pkBuffer + m_uiHead + 1 + uiConsequentMemReadableSize,
					m_pkBuffer, m_uiPacketFromBufferHeadSize);
#endif // WIN32
			}
			if (m_uiPacketFromBufferHeadSize == 0)
			{
				uiReadableSize = uiConsequentMemReadableSize;
			}
			else
			{
				uiReadableSize = uiConsequentMemReadableSize + m_uiPacketFromBufferHeadSize;
			}
			return true;
		}
		/// 对应@see readPacketBegin, 告诉RingBuffer读了多少连续内存@param uiReadSize
		/// 修改m_uiHead，不修改m_uiTail，函数内m_uiTail可能会被其它线程修改
		void readPacketEnd(unsigned int uiReadSize)
		{
			if (m_uiPacketFromBufferHeadSize == 0)
			{
				m_uiHead = (m_uiHead + uiReadSize) % m_uiTotalSize;
			}
			else
			{
				m_uiHead = m_uiPacketFromBufferHeadSize - 1;
			}
			m_bReading = false;
		}
		/// 判断当前包大小是否可用
		bool packetSizeValid()
		{
			return readableSize() >= sizeof(unsigned int);
		}
		/// 读取当前包大小，默认前四字节为包大小
		bool currentPacketSize(unsigned int& uiPacketSize)
		{
			unsigned int uiCurrentPacketSize = (unsigned int)-1;
			unsigned int uiTail = m_uiTail;
			unsigned int uiTotalReadableSize = readableSize(m_uiHead, uiTail);
			const unsigned int uiBitOfSize = sizeof(unsigned int);
			if (uiTotalReadableSize < uiBitOfSize)
			{
				uiPacketSize = uiCurrentPacketSize;
				return false;
			}
			unsigned int uiConsequentMemReadableSize = consequentMemReadableSize(m_uiHead, uiTail);
			if (uiConsequentMemReadableSize < uiBitOfSize)
			{
#ifdef WIN32
				memcpy_s(&uiCurrentPacketSize, uiBitOfSize,
					m_pkBuffer + m_uiHead + 1, uiConsequentMemReadableSize);
				memcpy_s((((char*)&uiCurrentPacketSize) + uiConsequentMemReadableSize), 
					uiBitOfSize - uiConsequentMemReadableSize,
					m_pkBuffer, uiBitOfSize - uiConsequentMemReadableSize);
#else
				memcpy(&uiCurrentPacketSize, m_pkBuffer + m_uiHead + 1, uiConsequentMemReadableSize);
				memcpy((((char*)&uiCurrentPacketSize) + uiConsequentMemReadableSize),
					m_pkBuffer, uiBitOfSize - uiConsequentMemReadableSize);
#endif // WIN32
			}
			else
			{
				if (m_uiHead + 1 == m_uiTotalSize)
				{
					uiCurrentPacketSize = *((unsigned int*)m_pkBuffer);
				}
				else
				{
					uiCurrentPacketSize = *((unsigned int*)(m_pkBuffer + m_uiHead + 1));
				}
			}
			uiPacketSize = uiCurrentPacketSize;
			return true;
		}

	private:
		/// 有可能非连续的可读内存块大小
		unsigned int readableSize(unsigned int uiHead, unsigned int uiTail)
		{
			if (uiTail > uiHead)
			{
				return uiTail - (uiHead + 1);
			}
			return m_uiTotalSize - (uiHead + 1) + uiTail;
		}
		/// 有可能非连续的可写内存块大小
		unsigned int writableSize(unsigned int uiHead, unsigned int uiTail)
		{
			if (uiTail > uiHead)
			{
				return m_uiTotalSize - uiTail + uiHead;
			}
			return uiHead - uiTail;
		}
		/// 可读的连续内存块大小，使用此接口的时候注意m_uiTotalSize -  m_uiHead == 1的时候，
		/// 可读的起始内存地址是m_pkBuffer，并不是m_pkBuffer + m_uiHead + 1
		unsigned int consequentMemReadableSize(unsigned int uiHead, unsigned int uiTail)
		{
			if (uiTail > uiHead)
			{
				return uiTail - (uiHead + 1);
			}
			else
			{
				if (m_uiTotalSize -  uiHead == 1)
				{
					return uiTail;
				}
				else
				{
					return m_uiTotalSize - (uiHead + 1);
				}
			}
		}
		/// 可写的连续内存块大小，使用此接口的时候注意m_uiTotalSize -  m_uiTail == 0的时候，
		/// 可写的起始内存地址是m_pkBuffer，并不是m_pkBuffer + m_uiTail
		unsigned int consequentMemWritableSize(unsigned int uiHead, unsigned int uiTail)
		{
			if (uiTail > uiHead)
			{
				if (uiTail == m_uiTotalSize)
				{
					return uiHead;
				}
				return m_uiTotalSize - uiTail;
			}
			return uiHead - uiTail;
		}

	private:
		/// 当读到RingBuffer的内存尾需要补齐一个包时，
		/// 这个变量记录从m_pkBuffer起多少内存已经被
		/// 复制到末尾用来补齐一个包
		/// @{
		unsigned int volatile m_uiPacketFromBufferHeadSize;
		/// @}
		unsigned int m_uiTotalSize;      ///< 用于RingBuffer的缓存大小
		unsigned int m_uiMaxPacketSize;  ///< 最大包大小
		unsigned int volatile m_uiHead;  ///< m_uiHead + 1为直接可读内存起始位置
		unsigned int volatile m_uiTail;  ///< m_uiTail为直接可写内存起始位置
		bool volatile m_bReading;
		bool volatile m_bWritting;
		T *m_pkBuffer;

		friend class TcpConnection;
	};
}

#endif //__RingBuffer_h__