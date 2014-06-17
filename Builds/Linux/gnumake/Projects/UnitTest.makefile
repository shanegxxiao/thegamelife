# Included makefiles
include Common.makefile

# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_Include_Path=-I"../../../../Source/UnitTest/TestMain" \
$(Common_Include_Path)
Release_Include_Path=$(Debug_Include_Path)

# Library paths...
Debug_Library_Path=$(Common_Lib_Path)
Release_Library_Path=$(Debug_Library_Path)

# Additional libraries...
Debug_Libraries=$(Common_Link_Libs)
Release_Libraries=$(Common_Link_Libs_Release)

# Preprocessor definitions...
Debug_Preprocessor_Definitions=-D GCC_BUILD -D _DEBUG -D _CONSOLE 
Release_Preprocessor_Definitions=-D GCC_BUILD -D NDEBUG -D _CONSOLE 

# Implictly linked object files...
Debug_Implicitly_Linked_Objects=
Release_Implicitly_Linked_Objects=

# Compiler flags...
Debug_Compiler_Flags=-O0 -g 
Release_Compiler_Flags=-O2 

# Builds all configurations for this project...
.PHONY: build_all_configurations
build_all_configurations: Debug Release 

# Builds the Debug configuration...
.PHONY: Debug
Debug: create_folders ../IntermediateDir/UnitTest/gccDebug/revision.o ../IntermediateDir/UnitTest/gccDebug/TestMain.o ../IntermediateDir/UnitTest/gccDebug/UnitTestPCH.o ../IntermediateDir/UnitTest/gccDebug/RingBufferTest.o ../IntermediateDir/UnitTest/gccDebug/DatabaseTest.o ../IntermediateDir/UnitTest/gccDebug/LogTest.o ../IntermediateDir/UnitTest/gccDebug/RuntimeTest.o ../IntermediateDir/UnitTest/gccDebug/Service0.o ../IntermediateDir/UnitTest/gccDebug/Service1.o ../IntermediateDir/UnitTest/gccDebug/Service2.o ../IntermediateDir/UnitTest/gccDebug/OpensslTest.o ../IntermediateDir/UnitTest/gccDebug/DownloadTest.o ../IntermediateDir/UnitTest/gccDebug/ExtraSendBufferPoolTest.o 
	g++ ../IntermediateDir/UnitTest/gccDebug/revision.o ../IntermediateDir/UnitTest/gccDebug/TestMain.o ../IntermediateDir/UnitTest/gccDebug/UnitTestPCH.o ../IntermediateDir/UnitTest/gccDebug/RingBufferTest.o ../IntermediateDir/UnitTest/gccDebug/DatabaseTest.o ../IntermediateDir/UnitTest/gccDebug/LogTest.o ../IntermediateDir/UnitTest/gccDebug/RuntimeTest.o ../IntermediateDir/UnitTest/gccDebug/Service0.o ../IntermediateDir/UnitTest/gccDebug/Service1.o ../IntermediateDir/UnitTest/gccDebug/Service2.o ../IntermediateDir/UnitTest/gccDebug/OpensslTest.o ../IntermediateDir/UnitTest/gccDebug/DownloadTest.o ../IntermediateDir/UnitTest/gccDebug/ExtraSendBufferPoolTest.o  $(Debug_Library_Path) $(Debug_Libraries) -Wl,-rpath,./ -o ../../../../Binary/gccTools/UnitTest.exe

#Compiles PCH ../../../../Source/UnitTest/TestMain/UnitTestPCH.h for the Debug configuration...
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) $(Debug_Include_Path) ../../../../Source/UnitTest/TestMain/UnitTestPCH.h
	
# Compiles file ../../../../Source/Common/Revision/revision.cpp for the Debug configuration...
-include ../IntermediateDir/UnitTest/gccDebug/revision.d
../IntermediateDir/UnitTest/gccDebug/revision.o: ../../../../Source/Common/Revision/revision.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Revision/revision.cpp $(Debug_Include_Path) -o ../IntermediateDir/UnitTest/gccDebug/revision.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Revision/revision.cpp $(Debug_Include_Path) > ../IntermediateDir/UnitTest/gccDebug/revision.d

# Compiles file ../../../../Source/UnitTest/TestMain/TestMain.cpp for the Debug configuration...
-include ../IntermediateDir/UnitTest/gccDebug/TestMain.d
../IntermediateDir/UnitTest/gccDebug/TestMain.o: ../../../../Source/UnitTest/TestMain/TestMain.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/TestMain/TestMain.cpp $(Debug_Include_Path) -o ../IntermediateDir/UnitTest/gccDebug/TestMain.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/TestMain/TestMain.cpp $(Debug_Include_Path) > ../IntermediateDir/UnitTest/gccDebug/TestMain.d

# Compiles file ../../../../Source/UnitTest/TestMain/UnitTestPCH.cpp for the Debug configuration...
-include ../IntermediateDir/UnitTest/gccDebug/UnitTestPCH.d
../IntermediateDir/UnitTest/gccDebug/UnitTestPCH.o: ../../../../Source/UnitTest/TestMain/UnitTestPCH.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/TestMain/UnitTestPCH.cpp $(Debug_Include_Path) -o ../IntermediateDir/UnitTest/gccDebug/UnitTestPCH.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/TestMain/UnitTestPCH.cpp $(Debug_Include_Path) > ../IntermediateDir/UnitTest/gccDebug/UnitTestPCH.d

# Compiles file ../../../../Source/UnitTest/RingBufferTest/RingBufferTest.cpp for the Debug configuration...
-include ../IntermediateDir/UnitTest/gccDebug/RingBufferTest.d
../IntermediateDir/UnitTest/gccDebug/RingBufferTest.o: ../../../../Source/UnitTest/RingBufferTest/RingBufferTest.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/RingBufferTest/RingBufferTest.cpp $(Debug_Include_Path) -o ../IntermediateDir/UnitTest/gccDebug/RingBufferTest.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/RingBufferTest/RingBufferTest.cpp $(Debug_Include_Path) > ../IntermediateDir/UnitTest/gccDebug/RingBufferTest.d

# Compiles file ../../../../Source/UnitTest/DatabaseTest/DatabaseTest.cpp for the Debug configuration...
-include ../IntermediateDir/UnitTest/gccDebug/DatabaseTest.d
../IntermediateDir/UnitTest/gccDebug/DatabaseTest.o: ../../../../Source/UnitTest/DatabaseTest/DatabaseTest.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/DatabaseTest/DatabaseTest.cpp $(Debug_Include_Path) -o ../IntermediateDir/UnitTest/gccDebug/DatabaseTest.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/DatabaseTest/DatabaseTest.cpp $(Debug_Include_Path) > ../IntermediateDir/UnitTest/gccDebug/DatabaseTest.d

# Compiles file ../../../../Source/UnitTest/LogTest/LogTest.cpp for the Debug configuration...
-include ../IntermediateDir/UnitTest/gccDebug/LogTest.d
../IntermediateDir/UnitTest/gccDebug/LogTest.o: ../../../../Source/UnitTest/LogTest/LogTest.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/LogTest/LogTest.cpp $(Debug_Include_Path) -o ../IntermediateDir/UnitTest/gccDebug/LogTest.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/LogTest/LogTest.cpp $(Debug_Include_Path) > ../IntermediateDir/UnitTest/gccDebug/LogTest.d

# Compiles file ../../../../Source/UnitTest/RuntimeTest/RuntimeTest.cpp for the Debug configuration...
-include ../IntermediateDir/UnitTest/gccDebug/RuntimeTest.d
../IntermediateDir/UnitTest/gccDebug/RuntimeTest.o: ../../../../Source/UnitTest/RuntimeTest/RuntimeTest.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/RuntimeTest/RuntimeTest.cpp $(Debug_Include_Path) -o ../IntermediateDir/UnitTest/gccDebug/RuntimeTest.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/RuntimeTest/RuntimeTest.cpp $(Debug_Include_Path) > ../IntermediateDir/UnitTest/gccDebug/RuntimeTest.d

# Compiles file ../../../../Source/UnitTest/RuntimeTest/Service0.cpp for the Debug configuration...
-include ../IntermediateDir/UnitTest/gccDebug/Service0.d
../IntermediateDir/UnitTest/gccDebug/Service0.o: ../../../../Source/UnitTest/RuntimeTest/Service0.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/RuntimeTest/Service0.cpp $(Debug_Include_Path) -o ../IntermediateDir/UnitTest/gccDebug/Service0.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/RuntimeTest/Service0.cpp $(Debug_Include_Path) > ../IntermediateDir/UnitTest/gccDebug/Service0.d

# Compiles file ../../../../Source/UnitTest/RuntimeTest/Service1.cpp for the Debug configuration...
-include ../IntermediateDir/UnitTest/gccDebug/Service1.d
../IntermediateDir/UnitTest/gccDebug/Service1.o: ../../../../Source/UnitTest/RuntimeTest/Service1.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/RuntimeTest/Service1.cpp $(Debug_Include_Path) -o ../IntermediateDir/UnitTest/gccDebug/Service1.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/RuntimeTest/Service1.cpp $(Debug_Include_Path) > ../IntermediateDir/UnitTest/gccDebug/Service1.d

# Compiles file ../../../../Source/UnitTest/RuntimeTest/Service2.cpp for the Debug configuration...
-include ../IntermediateDir/UnitTest/gccDebug/Service2.d
../IntermediateDir/UnitTest/gccDebug/Service2.o: ../../../../Source/UnitTest/RuntimeTest/Service2.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/RuntimeTest/Service2.cpp $(Debug_Include_Path) -o ../IntermediateDir/UnitTest/gccDebug/Service2.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/RuntimeTest/Service2.cpp $(Debug_Include_Path) > ../IntermediateDir/UnitTest/gccDebug/Service2.d

# Compiles file ../../../../Source/UnitTest/OpensslTest/OpensslTest.cpp for the Debug configuration...
-include ../IntermediateDir/UnitTest/gccDebug/OpensslTest.d
../IntermediateDir/UnitTest/gccDebug/OpensslTest.o: ../../../../Source/UnitTest/OpensslTest/OpensslTest.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/OpensslTest/OpensslTest.cpp $(Debug_Include_Path) -o ../IntermediateDir/UnitTest/gccDebug/OpensslTest.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/OpensslTest/OpensslTest.cpp $(Debug_Include_Path) > ../IntermediateDir/UnitTest/gccDebug/OpensslTest.d

# Compiles file ../../../../Source/UnitTest/DownloadTest/DownloadTest.cpp for the Debug configuration...
-include ../IntermediateDir/UnitTest/gccDebug/DownloadTest.d
../IntermediateDir/UnitTest/gccDebug/DownloadTest.o: ../../../../Source/UnitTest/DownloadTest/DownloadTest.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/DownloadTest/DownloadTest.cpp $(Debug_Include_Path) -o ../IntermediateDir/UnitTest/gccDebug/DownloadTest.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/DownloadTest/DownloadTest.cpp $(Debug_Include_Path) > ../IntermediateDir/UnitTest/gccDebug/DownloadTest.d

# Compiles file ../../../../Source/UnitTest/ExtraSendBufferPoolTest/ExtraSendBufferPoolTest.cpp for the Debug configuration...
-include ../IntermediateDir/UnitTest/gccDebug/ExtraSendBufferPoolTest.d
../IntermediateDir/UnitTest/gccDebug/ExtraSendBufferPoolTest.o: ../../../../Source/UnitTest/ExtraSendBufferPoolTest/ExtraSendBufferPoolTest.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/ExtraSendBufferPoolTest/ExtraSendBufferPoolTest.cpp $(Debug_Include_Path) -o ../IntermediateDir/UnitTest/gccDebug/ExtraSendBufferPoolTest.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/ExtraSendBufferPoolTest/ExtraSendBufferPoolTest.cpp $(Debug_Include_Path) > ../IntermediateDir/UnitTest/gccDebug/ExtraSendBufferPoolTest.d

# Builds the Release configuration...
.PHONY: Release
Release: create_folders ../IntermediateDir/UnitTest/gccRelease/revision.o ../IntermediateDir/UnitTest/gccRelease/TestMain.o ../IntermediateDir/UnitTest/gccRelease/UnitTestPCH.o ../IntermediateDir/UnitTest/gccRelease/RingBufferTest.o ../IntermediateDir/UnitTest/gccRelease/DatabaseTest.o ../IntermediateDir/UnitTest/gccRelease/LogTest.o ../IntermediateDir/UnitTest/gccRelease/RuntimeTest.o ../IntermediateDir/UnitTest/gccRelease/Service0.o ../IntermediateDir/UnitTest/gccRelease/Service1.o ../IntermediateDir/UnitTest/gccRelease/Service2.o ../IntermediateDir/UnitTest/gccRelease/OpensslTest.o ../IntermediateDir/UnitTest/gccRelease/DownloadTest.o ../IntermediateDir/UnitTest/gccRelease/ExtraSendBufferPoolTest.o 
	g++ ../IntermediateDir/UnitTest/gccRelease/revision.o ../IntermediateDir/UnitTest/gccRelease/TestMain.o ../IntermediateDir/UnitTest/gccRelease/UnitTestPCH.o ../IntermediateDir/UnitTest/gccRelease/RingBufferTest.o ../IntermediateDir/UnitTest/gccRelease/DatabaseTest.o ../IntermediateDir/UnitTest/gccRelease/LogTest.o ../IntermediateDir/UnitTest/gccRelease/RuntimeTest.o ../IntermediateDir/UnitTest/gccRelease/Service0.o ../IntermediateDir/UnitTest/gccRelease/Service1.o ../IntermediateDir/UnitTest/gccRelease/Service2.o ../IntermediateDir/UnitTest/gccRelease/OpensslTest.o ../IntermediateDir/UnitTest/gccRelease/DownloadTest.o ../IntermediateDir/UnitTest/gccRelease/ExtraSendBufferPoolTest.o  $(Release_Library_Path) $(Release_Libraries) -Wl,-rpath,./ -o ../../../../Binary/gccTools/UnitTest.exe

# Compiles file ../../../../Source/Common/Revision/revision.cpp for the Release configuration...
-include ../IntermediateDir/UnitTest/gccRelease/revision.d
../IntermediateDir/UnitTest/gccRelease/revision.o: ../../../../Source/Common/Revision/revision.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Revision/revision.cpp $(Release_Include_Path) -o ../IntermediateDir/UnitTest/gccRelease/revision.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Revision/revision.cpp $(Release_Include_Path) > ../IntermediateDir/UnitTest/gccRelease/revision.d

# Compiles file ../../../../Source/UnitTest/TestMain/TestMain.cpp for the Release configuration...
-include ../IntermediateDir/UnitTest/gccRelease/TestMain.d
../IntermediateDir/UnitTest/gccRelease/TestMain.o: ../../../../Source/UnitTest/TestMain/TestMain.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/TestMain/TestMain.cpp $(Release_Include_Path) -o ../IntermediateDir/UnitTest/gccRelease/TestMain.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/TestMain/TestMain.cpp $(Release_Include_Path) > ../IntermediateDir/UnitTest/gccRelease/TestMain.d

# Compiles file ../../../../Source/UnitTest/TestMain/UnitTestPCH.cpp for the Release configuration...
-include ../IntermediateDir/UnitTest/gccRelease/UnitTestPCH.d
../IntermediateDir/UnitTest/gccRelease/UnitTestPCH.o: ../../../../Source/UnitTest/TestMain/UnitTestPCH.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/TestMain/UnitTestPCH.cpp $(Release_Include_Path) -o ../IntermediateDir/UnitTest/gccRelease/UnitTestPCH.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/TestMain/UnitTestPCH.cpp $(Release_Include_Path) > ../IntermediateDir/UnitTest/gccRelease/UnitTestPCH.d

# Compiles file ../../../../Source/UnitTest/RingBufferTest/RingBufferTest.cpp for the Release configuration...
-include ../IntermediateDir/UnitTest/gccRelease/RingBufferTest.d
../IntermediateDir/UnitTest/gccRelease/RingBufferTest.o: ../../../../Source/UnitTest/RingBufferTest/RingBufferTest.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/RingBufferTest/RingBufferTest.cpp $(Release_Include_Path) -o ../IntermediateDir/UnitTest/gccRelease/RingBufferTest.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/RingBufferTest/RingBufferTest.cpp $(Release_Include_Path) > ../IntermediateDir/UnitTest/gccRelease/RingBufferTest.d

# Compiles file ../../../../Source/UnitTest/DatabaseTest/DatabaseTest.cpp for the Release configuration...
-include ../IntermediateDir/UnitTest/gccRelease/DatabaseTest.d
../IntermediateDir/UnitTest/gccRelease/DatabaseTest.o: ../../../../Source/UnitTest/DatabaseTest/DatabaseTest.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/DatabaseTest/DatabaseTest.cpp $(Release_Include_Path) -o ../IntermediateDir/UnitTest/gccRelease/DatabaseTest.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/DatabaseTest/DatabaseTest.cpp $(Release_Include_Path) > ../IntermediateDir/UnitTest/gccRelease/DatabaseTest.d

# Compiles file ../../../../Source/UnitTest/LogTest/LogTest.cpp for the Release configuration...
-include ../IntermediateDir/UnitTest/gccRelease/LogTest.d
../IntermediateDir/UnitTest/gccRelease/LogTest.o: ../../../../Source/UnitTest/LogTest/LogTest.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/LogTest/LogTest.cpp $(Release_Include_Path) -o ../IntermediateDir/UnitTest/gccRelease/LogTest.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/LogTest/LogTest.cpp $(Release_Include_Path) > ../IntermediateDir/UnitTest/gccRelease/LogTest.d

# Compiles file ../../../../Source/UnitTest/RuntimeTest/RuntimeTest.cpp for the Release configuration...
-include ../IntermediateDir/UnitTest/gccRelease/RuntimeTest.d
../IntermediateDir/UnitTest/gccRelease/RuntimeTest.o: ../../../../Source/UnitTest/RuntimeTest/RuntimeTest.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/RuntimeTest/RuntimeTest.cpp $(Release_Include_Path) -o ../IntermediateDir/UnitTest/gccRelease/RuntimeTest.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/RuntimeTest/RuntimeTest.cpp $(Release_Include_Path) > ../IntermediateDir/UnitTest/gccRelease/RuntimeTest.d

# Compiles file ../../../../Source/UnitTest/RuntimeTest/Service0.cpp for the Release configuration...
-include ../IntermediateDir/UnitTest/gccRelease/Service0.d
../IntermediateDir/UnitTest/gccRelease/Service0.o: ../../../../Source/UnitTest/RuntimeTest/Service0.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/RuntimeTest/Service0.cpp $(Release_Include_Path) -o ../IntermediateDir/UnitTest/gccRelease/Service0.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/RuntimeTest/Service0.cpp $(Release_Include_Path) > ../IntermediateDir/UnitTest/gccRelease/Service0.d

# Compiles file ../../../../Source/UnitTest/RuntimeTest/Service1.cpp for the Release configuration...
-include ../IntermediateDir/UnitTest/gccRelease/Service1.d
../IntermediateDir/UnitTest/gccRelease/Service1.o: ../../../../Source/UnitTest/RuntimeTest/Service1.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/RuntimeTest/Service1.cpp $(Release_Include_Path) -o ../IntermediateDir/UnitTest/gccRelease/Service1.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/RuntimeTest/Service1.cpp $(Release_Include_Path) > ../IntermediateDir/UnitTest/gccRelease/Service1.d

# Compiles file ../../../../Source/UnitTest/RuntimeTest/Service2.cpp for the Release configuration...
-include ../IntermediateDir/UnitTest/gccRelease/Service2.d
../IntermediateDir/UnitTest/gccRelease/Service2.o: ../../../../Source/UnitTest/RuntimeTest/Service2.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/RuntimeTest/Service2.cpp $(Release_Include_Path) -o ../IntermediateDir/UnitTest/gccRelease/Service2.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/RuntimeTest/Service2.cpp $(Release_Include_Path) > ../IntermediateDir/UnitTest/gccRelease/Service2.d

# Compiles file ../../../../Source/UnitTest/OpensslTest/OpensslTest.cpp for the Release configuration...
-include ../IntermediateDir/UnitTest/gccRelease/OpensslTest.d
../IntermediateDir/UnitTest/gccRelease/OpensslTest.o: ../../../../Source/UnitTest/OpensslTest/OpensslTest.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/OpensslTest/OpensslTest.cpp $(Release_Include_Path) -o ../IntermediateDir/UnitTest/gccRelease/OpensslTest.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/OpensslTest/OpensslTest.cpp $(Release_Include_Path) > ../IntermediateDir/UnitTest/gccRelease/OpensslTest.d

# Compiles file ../../../../Source/UnitTest/DownloadTest/DownloadTest.cpp for the Release configuration...
-include ../IntermediateDir/UnitTest/gccRelease/DownloadTest.d
../IntermediateDir/UnitTest/gccRelease/DownloadTest.o: ../../../../Source/UnitTest/DownloadTest/DownloadTest.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/DownloadTest/DownloadTest.cpp $(Release_Include_Path) -o ../IntermediateDir/UnitTest/gccRelease/DownloadTest.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/DownloadTest/DownloadTest.cpp $(Release_Include_Path) > ../IntermediateDir/UnitTest/gccRelease/DownloadTest.d

# Compiles file ../../../../Source/UnitTest/ExtraSendBufferPoolTest/ExtraSendBufferPoolTest.cpp for the Release configuration...
-include ../IntermediateDir/UnitTest/gccRelease/ExtraSendBufferPoolTest.d
../IntermediateDir/UnitTest/gccRelease/ExtraSendBufferPoolTest.o: ../../../../Source/UnitTest/ExtraSendBufferPoolTest/ExtraSendBufferPoolTest.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/ExtraSendBufferPoolTest/ExtraSendBufferPoolTest.cpp $(Release_Include_Path) -o ../IntermediateDir/UnitTest/gccRelease/ExtraSendBufferPoolTest.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/ExtraSendBufferPoolTest/ExtraSendBufferPoolTest.cpp $(Release_Include_Path) > ../IntermediateDir/UnitTest/gccRelease/ExtraSendBufferPoolTest.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p ../IntermediateDir/UnitTest/gccDebug
	mkdir -p ../../../../Binary/gccTools
	mkdir -p ../IntermediateDir/UnitTest/gccRelease
	mkdir -p ../../../../Binary/gccTools

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f ../IntermediateDir/UnitTest/gccDebug/*.o
	rm -f ../IntermediateDir/UnitTest/gccDebug/*.d
	rm -f ../../../../Binary/gccTools/*.a
	rm -f ../../../../Binary/gccTools/*.so
	rm -f ../../../../Binary/gccTools/*.dll
	rm -f ../../../../Binary/gccTools/*.exe
	rm -f ../IntermediateDir/UnitTest/gccRelease/*.o
	rm -f ../IntermediateDir/UnitTest/gccRelease/*.d
	rm -f ../../../../Binary/gccTools/*.a
	rm -f ../../../../Binary/gccTools/*.so
	rm -f ../../../../Binary/gccTools/*.dll
	rm -f ../../../../Binary/gccTools/*.exe

