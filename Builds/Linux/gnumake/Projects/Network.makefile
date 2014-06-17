# Included makefiles
include Common.makefile

# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_Include_Path=$(Common_Include_Path)
Release_Include_Path=$(Debug_Include_Path)

# Library paths...
Debug_Library_Path=-L"../../../../Library/Runtime/gccLib" \
-L"../../../../Library/Utility/gccLib"
Release_Library_Path=$(Debug_Library_Path)

# Additional libraries...
Debug_Libraries=-Wl,--start-group -lRuntime -lUtility  -Wl,--end-group
Release_Libraries=-Wl,--start-group -lRuntime -lUtility  -Wl,--end-group

# Preprocessor definitions...
Debug_Preprocessor_Definitions=-D GCC_BUILD -D _DEBUG -D _LIB 
Release_Preprocessor_Definitions=-D GCC_BUILD -D NDEBUG -D _LIB 

# Implictly linked object files...
Debug_Implicitly_Linked_Objects=
Release_Implicitly_Linked_Objects=

# Compiler flags...
Debug_Compiler_Flags=-O0 
Release_Compiler_Flags=-O2 

# Builds all configurations for this project...
.PHONY: build_all_configurations
build_all_configurations: Debug Release 

# Builds the Debug configuration...
.PHONY: Debug
Debug: create_folders ../IntermediateDir/Network/gccDebug/ActiveConnectionContainer.o ../IntermediateDir/Network/gccDebug/Connection.o ../IntermediateDir/Network/gccDebug/DownloadMgr.o ../IntermediateDir/Network/gccDebug/DownloadTask.o ../IntermediateDir/Network/gccDebug/HttpDownloadTask.o ../IntermediateDir/Network/gccDebug/IdealConnectionContainer.o ../IntermediateDir/Network/gccDebug/NetworkDefine.o ../IntermediateDir/Network/gccDebug/NetworkPCH.o ../IntermediateDir/Network/gccDebug/ConnectionIDGenerator.o ../IntermediateDir/Network/gccDebug/SSLConnection.o ../IntermediateDir/Network/gccDebug/TcpConnection.o ../IntermediateDir/Network/gccDebug/TcpNetwork.o post_buid_event 
	ar rcs ../../../../Library/Network/gccLib/libNetwork.a ../IntermediateDir/Network/gccDebug/ActiveConnectionContainer.o ../IntermediateDir/Network/gccDebug/Connection.o ../IntermediateDir/Network/gccDebug/DownloadMgr.o ../IntermediateDir/Network/gccDebug/DownloadTask.o ../IntermediateDir/Network/gccDebug/HttpDownloadTask.o ../IntermediateDir/Network/gccDebug/IdealConnectionContainer.o ../IntermediateDir/Network/gccDebug/NetworkDefine.o ../IntermediateDir/Network/gccDebug/NetworkPCH.o ../IntermediateDir/Network/gccDebug/ConnectionIDGenerator.o ../IntermediateDir/Network/gccDebug/SSLConnection.o ../IntermediateDir/Network/gccDebug/TcpConnection.o ../IntermediateDir/Network/gccDebug/TcpNetwork.o  $(Debug_Implicitly_Linked_Objects)

#Compiles PCH ../../../../Source/Common/Network/NetworkPCH.h for the Debug configuration...
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) $(Debug_Include_Path) ../../../../Source/Common/Network/NetworkPCH.h
	
# Compiles file ../../../../Source/Common/Network/ActiveConnectionContainer.cpp for the Debug configuration...
-include ../IntermediateDir/Network/gccDebug/ActiveConnectionContainer.d
../IntermediateDir/Network/gccDebug/ActiveConnectionContainer.o: ../../../../Source/Common/Network/ActiveConnectionContainer.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Network/ActiveConnectionContainer.cpp $(Debug_Include_Path) -o ../IntermediateDir/Network/gccDebug/ActiveConnectionContainer.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Network/ActiveConnectionContainer.cpp $(Debug_Include_Path) > ../IntermediateDir/Network/gccDebug/ActiveConnectionContainer.d

# Compiles file ../../../../Source/Common/Network/Connection.cpp for the Debug configuration...
-include ../IntermediateDir/Network/gccDebug/Connection.d
../IntermediateDir/Network/gccDebug/Connection.o: ../../../../Source/Common/Network/Connection.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Network/Connection.cpp $(Debug_Include_Path) -o ../IntermediateDir/Network/gccDebug/Connection.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Network/Connection.cpp $(Debug_Include_Path) > ../IntermediateDir/Network/gccDebug/Connection.d

# Compiles file ../../../../Source/Common/Network/DownloadMgr.cpp for the Debug configuration...
-include ../IntermediateDir/Network/gccDebug/DownloadMgr.d
../IntermediateDir/Network/gccDebug/DownloadMgr.o: ../../../../Source/Common/Network/DownloadMgr.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Network/DownloadMgr.cpp $(Debug_Include_Path) -o ../IntermediateDir/Network/gccDebug/DownloadMgr.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Network/DownloadMgr.cpp $(Debug_Include_Path) > ../IntermediateDir/Network/gccDebug/DownloadMgr.d

# Compiles file ../../../../Source/Common/Network/DownloadTask.cpp for the Debug configuration...
-include ../IntermediateDir/Network/gccDebug/DownloadTask.d
../IntermediateDir/Network/gccDebug/DownloadTask.o: ../../../../Source/Common/Network/DownloadTask.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Network/DownloadTask.cpp $(Debug_Include_Path) -o ../IntermediateDir/Network/gccDebug/DownloadTask.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Network/DownloadTask.cpp $(Debug_Include_Path) > ../IntermediateDir/Network/gccDebug/DownloadTask.d

# Compiles file ../../../../Source/Common/Network/HttpDownloadTask.cpp for the Debug configuration...
-include ../IntermediateDir/Network/gccDebug/HttpDownloadTask.d
../IntermediateDir/Network/gccDebug/HttpDownloadTask.o: ../../../../Source/Common/Network/HttpDownloadTask.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Network/HttpDownloadTask.cpp $(Debug_Include_Path) -o ../IntermediateDir/Network/gccDebug/HttpDownloadTask.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Network/HttpDownloadTask.cpp $(Debug_Include_Path) > ../IntermediateDir/Network/gccDebug/HttpDownloadTask.d

# Compiles file ../../../../Source/Common/Network/IdealConnectionContainer.cpp for the Debug configuration...
-include ../IntermediateDir/Network/gccDebug/IdealConnectionContainer.d
../IntermediateDir/Network/gccDebug/IdealConnectionContainer.o: ../../../../Source/Common/Network/IdealConnectionContainer.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Network/IdealConnectionContainer.cpp $(Debug_Include_Path) -o ../IntermediateDir/Network/gccDebug/IdealConnectionContainer.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Network/IdealConnectionContainer.cpp $(Debug_Include_Path) > ../IntermediateDir/Network/gccDebug/IdealConnectionContainer.d

# Compiles file ../../../../Source/Common/Network/NetworkDefine.cpp for the Debug configuration...
-include ../IntermediateDir/Network/gccDebug/NetworkDefine.d
../IntermediateDir/Network/gccDebug/NetworkDefine.o: ../../../../Source/Common/Network/NetworkDefine.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Network/NetworkDefine.cpp $(Debug_Include_Path) -o ../IntermediateDir/Network/gccDebug/NetworkDefine.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Network/NetworkDefine.cpp $(Debug_Include_Path) > ../IntermediateDir/Network/gccDebug/NetworkDefine.d

# Compiles file ../../../../Source/Common/Network/NetworkPCH.cpp for the Debug configuration...
-include ../IntermediateDir/Network/gccDebug/NetworkPCH.d
../IntermediateDir/Network/gccDebug/NetworkPCH.o: ../../../../Source/Common/Network/NetworkPCH.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Network/NetworkPCH.cpp $(Debug_Include_Path) -o ../IntermediateDir/Network/gccDebug/NetworkPCH.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Network/NetworkPCH.cpp $(Debug_Include_Path) > ../IntermediateDir/Network/gccDebug/NetworkPCH.d

# Compiles file ../../../../Source/Common/Network/ConnectionIDGenerator.cpp for the Debug configuration...
-include ../IntermediateDir/Network/gccDebug/ConnectionIDGenerator.d
../IntermediateDir/Network/gccDebug/ConnectionIDGenerator.o: ../../../../Source/Common/Network/ConnectionIDGenerator.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Network/ConnectionIDGenerator.cpp $(Debug_Include_Path) -o ../IntermediateDir/Network/gccDebug/ConnectionIDGenerator.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Network/ConnectionIDGenerator.cpp $(Debug_Include_Path) > ../IntermediateDir/Network/gccDebug/ConnectionIDGenerator.d

# Compiles file ../../../../Source/Common/Network/SSLConnection.cpp for the Debug configuration...
-include ../IntermediateDir/Network/gccDebug/SSLConnection.d
../IntermediateDir/Network/gccDebug/SSLConnection.o: ../../../../Source/Common/Network/SSLConnection.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Network/SSLConnection.cpp $(Debug_Include_Path) -o ../IntermediateDir/Network/gccDebug/SSLConnection.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Network/SSLConnection.cpp $(Debug_Include_Path) > ../IntermediateDir/Network/gccDebug/SSLConnection.d

# Compiles file ../../../../Source/Common/Network/TcpConnection.cpp for the Debug configuration...
-include ../IntermediateDir/Network/gccDebug/TcpConnection.d
../IntermediateDir/Network/gccDebug/TcpConnection.o: ../../../../Source/Common/Network/TcpConnection.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Network/TcpConnection.cpp $(Debug_Include_Path) -o ../IntermediateDir/Network/gccDebug/TcpConnection.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Network/TcpConnection.cpp $(Debug_Include_Path) > ../IntermediateDir/Network/gccDebug/TcpConnection.d

# Compiles file ../../../../Source/Common/Network/TcpNetwork.cpp for the Debug configuration...
-include ../IntermediateDir/Network/gccDebug/TcpNetwork.d
../IntermediateDir/Network/gccDebug/TcpNetwork.o: ../../../../Source/Common/Network/TcpNetwork.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Network/TcpNetwork.cpp $(Debug_Include_Path) -o ../IntermediateDir/Network/gccDebug/TcpNetwork.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Network/TcpNetwork.cpp $(Debug_Include_Path) > ../IntermediateDir/Network/gccDebug/TcpNetwork.d

# Builds the Release configuration...
.PHONY: Release
Release: create_folders ../IntermediateDir/Network/gccRelease/ActiveConnectionContainer.o ../IntermediateDir/Network/gccRelease/Connection.o ../IntermediateDir/Network/gccRelease/DownloadMgr.o ../IntermediateDir/Network/gccRelease/DownloadTask.o ../IntermediateDir/Network/gccRelease/HttpDownloadTask.o ../IntermediateDir/Network/gccRelease/IdealConnectionContainer.o ../IntermediateDir/Network/gccRelease/NetworkDefine.o ../IntermediateDir/Network/gccRelease/NetworkPCH.o ../IntermediateDir/Network/gccRelease/ConnectionIDGenerator.o ../IntermediateDir/Network/gccRelease/SSLConnection.o ../IntermediateDir/Network/gccRelease/TcpConnection.o ../IntermediateDir/Network/gccRelease/TcpNetwork.o 
	ar rcs ../../../../Library/Network/gccLib/libNetwork.a ../IntermediateDir/Network/gccRelease/ActiveConnectionContainer.o ../IntermediateDir/Network/gccRelease/Connection.o ../IntermediateDir/Network/gccRelease/DownloadMgr.o ../IntermediateDir/Network/gccRelease/DownloadTask.o ../IntermediateDir/Network/gccRelease/HttpDownloadTask.o ../IntermediateDir/Network/gccRelease/IdealConnectionContainer.o ../IntermediateDir/Network/gccRelease/NetworkDefine.o ../IntermediateDir/Network/gccRelease/NetworkPCH.o ../IntermediateDir/Network/gccRelease/ConnectionIDGenerator.o ../IntermediateDir/Network/gccRelease/SSLConnection.o ../IntermediateDir/Network/gccRelease/TcpConnection.o ../IntermediateDir/Network/gccRelease/TcpNetwork.o  $(Release_Implicitly_Linked_Objects)

# Compiles file ../../../../Source/Common/Network/ActiveConnectionContainer.cpp for the Release configuration...
-include ../IntermediateDir/Network/gccRelease/ActiveConnectionContainer.d
../IntermediateDir/Network/gccRelease/ActiveConnectionContainer.o: ../../../../Source/Common/Network/ActiveConnectionContainer.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Network/ActiveConnectionContainer.cpp $(Release_Include_Path) -o ../IntermediateDir/Network/gccRelease/ActiveConnectionContainer.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Network/ActiveConnectionContainer.cpp $(Release_Include_Path) > ../IntermediateDir/Network/gccRelease/ActiveConnectionContainer.d

# Compiles file ../../../../Source/Common/Network/Connection.cpp for the Release configuration...
-include ../IntermediateDir/Network/gccRelease/Connection.d
../IntermediateDir/Network/gccRelease/Connection.o: ../../../../Source/Common/Network/Connection.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Network/Connection.cpp $(Release_Include_Path) -o ../IntermediateDir/Network/gccRelease/Connection.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Network/Connection.cpp $(Release_Include_Path) > ../IntermediateDir/Network/gccRelease/Connection.d

# Compiles file ../../../../Source/Common/Network/DownloadMgr.cpp for the Release configuration...
-include ../IntermediateDir/Network/gccRelease/DownloadMgr.d
../IntermediateDir/Network/gccRelease/DownloadMgr.o: ../../../../Source/Common/Network/DownloadMgr.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Network/DownloadMgr.cpp $(Release_Include_Path) -o ../IntermediateDir/Network/gccRelease/DownloadMgr.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Network/DownloadMgr.cpp $(Release_Include_Path) > ../IntermediateDir/Network/gccRelease/DownloadMgr.d

# Compiles file ../../../../Source/Common/Network/DownloadTask.cpp for the Release configuration...
-include ../IntermediateDir/Network/gccRelease/DownloadTask.d
../IntermediateDir/Network/gccRelease/DownloadTask.o: ../../../../Source/Common/Network/DownloadTask.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Network/DownloadTask.cpp $(Release_Include_Path) -o ../IntermediateDir/Network/gccRelease/DownloadTask.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Network/DownloadTask.cpp $(Release_Include_Path) > ../IntermediateDir/Network/gccRelease/DownloadTask.d

# Compiles file ../../../../Source/Common/Network/HttpDownloadTask.cpp for the Release configuration...
-include ../IntermediateDir/Network/gccRelease/HttpDownloadTask.d
../IntermediateDir/Network/gccRelease/HttpDownloadTask.o: ../../../../Source/Common/Network/HttpDownloadTask.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Network/HttpDownloadTask.cpp $(Release_Include_Path) -o ../IntermediateDir/Network/gccRelease/HttpDownloadTask.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Network/HttpDownloadTask.cpp $(Release_Include_Path) > ../IntermediateDir/Network/gccRelease/HttpDownloadTask.d

# Compiles file ../../../../Source/Common/Network/IdealConnectionContainer.cpp for the Release configuration...
-include ../IntermediateDir/Network/gccRelease/IdealConnectionContainer.d
../IntermediateDir/Network/gccRelease/IdealConnectionContainer.o: ../../../../Source/Common/Network/IdealConnectionContainer.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Network/IdealConnectionContainer.cpp $(Release_Include_Path) -o ../IntermediateDir/Network/gccRelease/IdealConnectionContainer.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Network/IdealConnectionContainer.cpp $(Release_Include_Path) > ../IntermediateDir/Network/gccRelease/IdealConnectionContainer.d

# Compiles file ../../../../Source/Common/Network/NetworkDefine.cpp for the Release configuration...
-include ../IntermediateDir/Network/gccRelease/NetworkDefine.d
../IntermediateDir/Network/gccRelease/NetworkDefine.o: ../../../../Source/Common/Network/NetworkDefine.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Network/NetworkDefine.cpp $(Release_Include_Path) -o ../IntermediateDir/Network/gccRelease/NetworkDefine.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Network/NetworkDefine.cpp $(Release_Include_Path) > ../IntermediateDir/Network/gccRelease/NetworkDefine.d

# Compiles file ../../../../Source/Common/Network/NetworkPCH.cpp for the Release configuration...
-include ../IntermediateDir/Network/gccRelease/NetworkPCH.d
../IntermediateDir/Network/gccRelease/NetworkPCH.o: ../../../../Source/Common/Network/NetworkPCH.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Network/NetworkPCH.cpp $(Release_Include_Path) -o ../IntermediateDir/Network/gccRelease/NetworkPCH.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Network/NetworkPCH.cpp $(Release_Include_Path) > ../IntermediateDir/Network/gccRelease/NetworkPCH.d

# Compiles file ../../../../Source/Common/Network/ConnectionIDGenerator.cpp for the Release configuration...
-include ../IntermediateDir/Network/gccRelease/ConnectionIDGenerator.d
../IntermediateDir/Network/gccRelease/ConnectionIDGenerator.o: ../../../../Source/Common/Network/ConnectionIDGenerator.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Network/ConnectionIDGenerator.cpp $(Release_Include_Path) -o ../IntermediateDir/Network/gccRelease/ConnectionIDGenerator.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Network/ConnectionIDGenerator.cpp $(Release_Include_Path) > ../IntermediateDir/Network/gccRelease/ConnectionIDGenerator.d

# Compiles file ../../../../Source/Common/Network/SSLConnection.cpp for the Release configuration...
-include ../IntermediateDir/Network/gccRelease/SSLConnection.d
../IntermediateDir/Network/gccRelease/SSLConnection.o: ../../../../Source/Common/Network/SSLConnection.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Network/SSLConnection.cpp $(Release_Include_Path) -o ../IntermediateDir/Network/gccRelease/SSLConnection.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Network/SSLConnection.cpp $(Release_Include_Path) > ../IntermediateDir/Network/gccRelease/SSLConnection.d

# Compiles file ../../../../Source/Common/Network/TcpConnection.cpp for the Release configuration...
-include ../IntermediateDir/Network/gccRelease/TcpConnection.d
../IntermediateDir/Network/gccRelease/TcpConnection.o: ../../../../Source/Common/Network/TcpConnection.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Network/TcpConnection.cpp $(Release_Include_Path) -o ../IntermediateDir/Network/gccRelease/TcpConnection.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Network/TcpConnection.cpp $(Release_Include_Path) > ../IntermediateDir/Network/gccRelease/TcpConnection.d

# Compiles file ../../../../Source/Common/Network/TcpNetwork.cpp for the Release configuration...
-include ../IntermediateDir/Network/gccRelease/TcpNetwork.d
../IntermediateDir/Network/gccRelease/TcpNetwork.o: ../../../../Source/Common/Network/TcpNetwork.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Network/TcpNetwork.cpp $(Release_Include_Path) -o ../IntermediateDir/Network/gccRelease/TcpNetwork.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Network/TcpNetwork.cpp $(Release_Include_Path) > ../IntermediateDir/Network/gccRelease/TcpNetwork.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p ../IntermediateDir/Network/gccDebug
	mkdir -p ../../../../Library/Network/gccLib
	mkdir -p ../IntermediateDir/Network/gccRelease
	mkdir -p ../../../../Library/Network/gccLib
	
.PHONY: post_buid_event
post_build_event:
	mkdir -p ../../../../Library/Network/Network
	rm ../../../../Library/Network/Network/*.h
	cp ../../../../Source/Network/*.h ../../../../Library/Network/Network/

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f ../IntermediateDir/Network/gccDebug/*.o
	rm -f ../IntermediateDir/Network/gccDebug/*.d
	rm -f ../../../../Library/Network/gccLib/*.a
	rm -f ../../../../Library/Network/gccLib/*.so
	rm -f ../../../../Library/Network/gccLib/*.dll
	rm -f ../../../../Library/Network/gccLib/*.exe
	rm -f ../IntermediateDir/Network/gccRelease/*.o
	rm -f ../IntermediateDir/Network/gccRelease/*.d
	rm -f ../../../../Library/Network/gccLib/*.a
	rm -f ../../../../Library/Network/gccLib/*.so
	rm -f ../../../../Library/Network/gccLib/*.dll
	rm -f ../../../../Library/Network/gccLib/*.exe

