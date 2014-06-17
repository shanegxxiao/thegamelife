所有库目录结构如下。不是由解决方案内的项目生成库目录需要打包成“库名.rar”形式的压缩包上传到SVN，项目编译前需要解压这些压缩包。由解决方案内项目生成的库如果目录不能自动生成，则需要手动添加如以下树形所示的相应库目录结构。

由解决方案内项目生成的库:
Database
Network
Runtime
Utility


Library
│  库名0.rar
│  库名1.rar
│  ...
│  库名n.rar
│  Network.rar                                   ///< 例如                                  
│  ReadMe.txt
│  
│          
├─库名0
│  ├─库名0
│  │      库头文件0
│  │      库头文件1
│  │      ...
│  │      库头文件n
│  │      
│  └─Lib
│          库名0_Debug.lib
│          库名0_Release.lib
│          
├─库名1
│  ├─库名1
│  │      库头文件0
│  │      库头文件1
│  │      ...
│  │      库头文件n
│  │      
│  └─Lib
│          库名1_Debug.lib
│          库名1_Release.lib
│
├─...
│  ├─...
│  │      ...
│  │      ...
│  └─Lib
│          ..._Debug.lib
│          ..._Release.lib
│          
├─库名n
│  ├─库名n
│  │      库头文件0
│  │      库头文件1
│  │      库头文件2
│  │      ...
│  │      库头文件n
│  │      
│  └─Lib
│          库名n_Debug.lib
│          库名n_Release.lib
│          
├─Network                                       ///< 例如
│  ├─Network
│  │      ActiveTcpConnectionContainer.h
│  │      ConnectionIDGenerator.h
│  │      ExtraSendBufferPool.h
│  │      IdealTcpConnectionContainer.h
│  │      Network.h
│  │      NetworkDefine.h
│  │      NetworkPCH.h
│  │      RingBuffer.h
│  │      targetver.h
│  │      TcpConnection.h
│  │      TcpNetwork.h
│  │      
│  └─Lib
│          Network_Debug.lib
│          Network_Release.lib
