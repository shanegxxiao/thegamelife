#ifndef __ExtraSendBufferPool_h__
#define __ExtraSendBufferPool_h__

/// 非多线程安全，不能多线程访问
/// 当发送缓存区溢出且溢出不直接断开的时候使用此类缓存不能及时发送的数据
namespace Network
{
	template<typename T>
	class ExtraSendBufferPool
	{
	private:
		struct ExtraSendBuffer
		{
			T* pBuffer;
			unsigned int uiDataSize;///< 已填充数据大小
			unsigned int uiLeftSize;///< 未填充数据大小
			ExtraSendBuffer* pkPrev;
			ExtraSendBuffer* pkNext;

			ExtraSendBuffer(unsigned int uiBuffSize)
			{
				uiLeftSize = uiBuffSize;
				pBuffer = new T[uiLeftSize];
				uiDataSize = 0;
				pkPrev = 0;
				pkNext = 0;
			}
			~ExtraSendBuffer()
			{
				delete[] pBuffer;
			}

			bool pushData(T* pData, unsigned int uiSize)
			{
				if (uiSize > uiLeftSize)
				{
					return false;
				}
#ifdef WIN32
				memcpy_s(pBuffer + uiDataSize, uiLeftSize, pData, uiSize);
#else
				memcpy(pBuffer + uiDataSize, pData, uiSize);
#endif//WIN32
				uiLeftSize -= uiSize;
				uiDataSize += uiSize;
				return true;
			}
			void clearData()
			{
				uiLeftSize += uiDataSize;
				uiDataSize = 0;
			}
		};

	public:
		ExtraSendBufferPool(unsigned int uiPoolSize, unsigned int uiBufferSize)
			: muiPoolSize(uiPoolSize), muiBufferSize(uiBufferSize), mpkIdealBufferHead(0),
			mpkIdealBufferTail(0), mpkFullBufferHead(0), mpkFullBufferTail(0),
			mpkActiveBuffer(0), muiIdealBufferCnt(0), muiFullBufferCnt(0)
		{
			for (unsigned int ui = 0; ui < muiPoolSize; ++ ui)
			{
				ExtraSendBuffer *pkBuffer = new ExtraSendBuffer(muiBufferSize);
				pushIdeal(pkBuffer);
			}
		}
		~ExtraSendBufferPool()
		{
			deleteBufferList(mpkIdealBufferHead);
			deleteBufferList(mpkFullBufferHead);
			if (mpkActiveBuffer)
			{
				delete mpkActiveBuffer;
			}
		}

		void resetPool()
		{
			if (mpkActiveBuffer)
			{
				pushIdeal(mpkActiveBuffer);
				mpkActiveBuffer = 0;
			}
			ExtraSendBuffer* pkBuff = popFull();
			while (pkBuff)
			{
				pushIdeal(pkBuff);
				pkBuff = popFull();
			}
			while (muiIdealBufferCnt > muiPoolSize)
			{
				pkBuff = popIdeal();
				delete pkBuff;
			}
		}

		bool hasData()
		{
			return mpkFullBufferHead || 
				(mpkActiveBuffer && mpkActiveBuffer->uiDataSize > 0);
		}

		bool pushData(T* pData, unsigned int uiSize)
		{
			if (uiSize > muiBufferSize)
			{
				return false;
			}
			if (mpkActiveBuffer)
			{
				if (mpkActiveBuffer->pushData(pData, uiSize))
				{
					return true;
				}
				pushFull(mpkActiveBuffer);
			}
			mpkActiveBuffer = popIdeal();
			return mpkActiveBuffer->pushData(pData, uiSize);
		}
		void getData(T*& pData, unsigned int& uiSize)
		{
			if (mpkFullBufferHead)
			{
				pData = mpkFullBufferHead->pBuffer;
				uiSize = mpkFullBufferHead->uiDataSize;
				return;
			}
			if (mpkActiveBuffer)
			{
				pData = mpkActiveBuffer->pBuffer;
				uiSize = mpkActiveBuffer->uiDataSize;
				return;
			}
			pData = 0;
			uiSize = 0;
		}
		void popData()
		{
			if (mpkFullBufferHead)
			{
				ExtraSendBuffer* pkBuff = popFull();
				pushIdeal(pkBuff);
				return;
			}
			if (mpkActiveBuffer)
			{
				mpkActiveBuffer->clearData();
			}
		}

	private:
		ExtraSendBuffer* popIdeal()
		{
			ExtraSendBuffer* pkBuff = popBuffer(mpkIdealBufferHead, mpkIdealBufferTail, true);
			if (pkBuff)
			{
				pkBuff->clearData();
				if (muiIdealBufferCnt > 0)
				{
					--muiIdealBufferCnt;
				}
			}
			return pkBuff;
		}
		void pushIdeal(ExtraSendBuffer* pkBuffer)
		{
			pushBuff(mpkIdealBufferHead, mpkIdealBufferTail, pkBuffer);
			if (pkBuffer)
			{
				++muiIdealBufferCnt;
			}
		}
		ExtraSendBuffer* popFull()
		{
			ExtraSendBuffer* pkBuff = popBuffer(mpkFullBufferHead, mpkFullBufferTail, false);
			if (pkBuff && muiFullBufferCnt > 0)
			{
				--muiFullBufferCnt;
			}
			return pkBuff;
		}
		void pushFull(ExtraSendBuffer* pkBuffer)
		{
			pushBuff(mpkFullBufferHead, mpkFullBufferTail, pkBuffer);
			if (pkBuffer)
			{
				++muiFullBufferCnt;
			}
		}

		void deleteBufferList(ExtraSendBuffer* pkBuffer)
		{
			while (pkBuffer)
			{
				ExtraSendBuffer* pkToDelete = pkBuffer;
				pkBuffer = pkToDelete->pkNext;
				delete pkToDelete;
			}
		}

		ExtraSendBuffer* popBuffer(ExtraSendBuffer* &pkBuffHead, ExtraSendBuffer* &pkBuffTail, bool bNew)
		{
			if (pkBuffHead)
			{
				ExtraSendBuffer* pkResult = pkBuffHead;
				pkBuffHead = pkResult->pkNext;
				if (pkBuffHead)
				{
					pkBuffHead->pkPrev = 0;
				}
				pkResult->pkNext = 0;
				pkResult->pkPrev = 0;
				if (pkResult == pkBuffTail)
				{
					pkBuffTail = 0;
				}
				return pkResult;
			}
			if (bNew)
			{
				return new ExtraSendBuffer(muiBufferSize);
			}
			return 0;
		}
		void pushBuff(ExtraSendBuffer* &pkBuffHead, ExtraSendBuffer* &pkBuffTail, ExtraSendBuffer* pkBuff)
		{
			if (!pkBuffTail)
			{
				pkBuffTail = pkBuff;
				pkBuffTail->pkNext = 0;
				pkBuffTail->pkPrev = 0;
				pkBuffHead = pkBuffTail;
				return;
			}
			pkBuffTail->pkNext = pkBuff;
			pkBuff->pkNext = 0;
			pkBuff->pkPrev = pkBuffTail;
			pkBuffTail = pkBuff;
		}	

	private:
		unsigned int muiBufferSize;///< 单个缓冲区块大小
		unsigned int muiPoolSize;///< 缓冲区池大小（持有的缓冲区块数量）
		unsigned int muiIdealBufferCnt;///< 空闲块数量
		unsigned int muiFullBufferCnt;///< 不能再写入块数量
		ExtraSendBuffer *mpkIdealBufferHead;///< 空闲块的链表头
		ExtraSendBuffer *mpkIdealBufferTail;///< 空闲块的链表尾
		ExtraSendBuffer *mpkFullBufferHead;///< 不能再写入的块的链表头
		ExtraSendBuffer *mpkFullBufferTail;///< 不能再写入的块的链表尾
		ExtraSendBuffer *mpkActiveBuffer;///< 正在使用的块，有可能可以继续写入数据
	};
}

#endif//__ExtraSendBufferPooll_h__