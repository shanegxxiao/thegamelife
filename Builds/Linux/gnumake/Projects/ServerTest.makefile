# Included makefiles
include Common.makefile

# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_Include_Path=$(Common_Include_Path)
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
Debug: create_folders ../IntermediateDir/ServerTest/gccDebug/ConCallback.o ../IntermediateDir/ServerTest/gccDebug/Main.o ../IntermediateDir/ServerTest/gccDebug/SSLServer.o ../IntermediateDir/ServerTest/gccDebug/ServerTestPCH.o 
	g++ ../IntermediateDir/ServerTest/gccDebug/ConCallback.o ../IntermediateDir/ServerTest/gccDebug/Main.o ../IntermediateDir/ServerTest/gccDebug/SSLServer.o ../IntermediateDir/ServerTest/gccDebug/ServerTestPCH.o  $(Debug_Library_Path) $(Debug_Libraries) -Wl,-rpath,./ -o ../../../../Binary/gccTools/ServerTest.exe

#Compiles PCH ../../../../Source/UnitTest/ServerTest/ServerTestPCH.h for the Debug configuration...
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) $(Debug_Include_Path) ../../../../Source/UnitTest/ServerTest/ServerTestPCH.h
	
# Compiles file ../../../../Source/UnitTest/ServerTest/ConCallback.cpp for the Debug configuration...
-include ../IntermediateDir/ServerTest/gccDebug/ConCallback.d
../IntermediateDir/ServerTest/gccDebug/ConCallback.o: ../../../../Source/UnitTest/ServerTest/ConCallback.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/ServerTest/ConCallback.cpp $(Debug_Include_Path) -o ../IntermediateDir/ServerTest/gccDebug/ConCallback.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/ServerTest/ConCallback.cpp $(Debug_Include_Path) > ../IntermediateDir/ServerTest/gccDebug/ConCallback.d

# Compiles file ../../../../Source/UnitTest/ServerTest/Main.cpp for the Debug configuration...
-include ../IntermediateDir/ServerTest/gccDebug/Main.d
../IntermediateDir/ServerTest/gccDebug/Main.o: ../../../../Source/UnitTest/ServerTest/Main.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/ServerTest/Main.cpp $(Debug_Include_Path) -o ../IntermediateDir/ServerTest/gccDebug/Main.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/ServerTest/Main.cpp $(Debug_Include_Path) > ../IntermediateDir/ServerTest/gccDebug/Main.d

# Compiles file ../../../../Source/UnitTest/ServerTest/SSLServer.cpp for the Debug configuration...
-include ../IntermediateDir/ServerTest/gccDebug/SSLServer.d
../IntermediateDir/ServerTest/gccDebug/SSLServer.o: ../../../../Source/UnitTest/ServerTest/SSLServer.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/ServerTest/SSLServer.cpp $(Debug_Include_Path) -o ../IntermediateDir/ServerTest/gccDebug/SSLServer.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/ServerTest/SSLServer.cpp $(Debug_Include_Path) > ../IntermediateDir/ServerTest/gccDebug/SSLServer.d

# Compiles file ../../../../Source/UnitTest/ServerTest/ServerTestPCH.cpp for the Debug configuration...
-include ../IntermediateDir/ServerTest/gccDebug/ServerTestPCH.d
../IntermediateDir/ServerTest/gccDebug/ServerTestPCH.o: ../../../../Source/UnitTest/ServerTest/ServerTestPCH.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/ServerTest/ServerTestPCH.cpp $(Debug_Include_Path) -o ../IntermediateDir/ServerTest/gccDebug/ServerTestPCH.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/ServerTest/ServerTestPCH.cpp $(Debug_Include_Path) > ../IntermediateDir/ServerTest/gccDebug/ServerTestPCH.d

# Builds the Release configuration...
.PHONY: Release
Release: create_folders ../IntermediateDir/ServerTest/gccRelease/ConCallback.o ../IntermediateDir/ServerTest/gccRelease/Main.o ../IntermediateDir/ServerTest/gccRelease/SSLServer.o ../IntermediateDir/ServerTest/gccRelease/ServerTestPCH.o 
	g++ ../IntermediateDir/ServerTest/gccRelease/ConCallback.o ../IntermediateDir/ServerTest/gccRelease/Main.o ../IntermediateDir/ServerTest/gccRelease/SSLServer.o ../IntermediateDir/ServerTest/gccRelease/ServerTestPCH.o  $(Release_Library_Path) $(Release_Libraries) -Wl,-rpath,./ -o ../../../../Binary/gccTools/ServerTest.exe

# Compiles file ../../../../Source/UnitTest/ServerTest/ConCallback.cpp for the Release configuration...
-include ../IntermediateDir/ServerTest/gccRelease/ConCallback.d
../IntermediateDir/ServerTest/gccRelease/ConCallback.o: ../../../../Source/UnitTest/ServerTest/ConCallback.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/ServerTest/ConCallback.cpp $(Release_Include_Path) -o ../IntermediateDir/ServerTest/gccRelease/ConCallback.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/ServerTest/ConCallback.cpp $(Release_Include_Path) > ../IntermediateDir/ServerTest/gccRelease/ConCallback.d

# Compiles file ../../../../Source/UnitTest/ServerTest/Main.cpp for the Release configuration...
-include ../IntermediateDir/ServerTest/gccRelease/Main.d
../IntermediateDir/ServerTest/gccRelease/Main.o: ../../../../Source/UnitTest/ServerTest/Main.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/ServerTest/Main.cpp $(Release_Include_Path) -o ../IntermediateDir/ServerTest/gccRelease/Main.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/ServerTest/Main.cpp $(Release_Include_Path) > ../IntermediateDir/ServerTest/gccRelease/Main.d

# Compiles file ../../../../Source/UnitTest/ServerTest/SSLServer.cpp for the Release configuration...
-include ../IntermediateDir/ServerTest/gccRelease/SSLServer.d
../IntermediateDir/ServerTest/gccRelease/SSLServer.o: ../../../../Source/UnitTest/ServerTest/SSLServer.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/ServerTest/SSLServer.cpp $(Release_Include_Path) -o ../IntermediateDir/ServerTest/gccRelease/SSLServer.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/ServerTest/SSLServer.cpp $(Release_Include_Path) > ../IntermediateDir/ServerTest/gccRelease/SSLServer.d

# Compiles file ../../../../Source/UnitTest/ServerTest/ServerTestPCH.cpp for the Release configuration...
-include ../IntermediateDir/ServerTest/gccRelease/ServerTestPCH.d
../IntermediateDir/ServerTest/gccRelease/ServerTestPCH.o: ../../../../Source/UnitTest/ServerTest/ServerTestPCH.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/ServerTest/ServerTestPCH.cpp $(Release_Include_Path) -o ../IntermediateDir/ServerTest/gccRelease/ServerTestPCH.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/ServerTest/ServerTestPCH.cpp $(Release_Include_Path) > ../IntermediateDir/ServerTest/gccRelease/ServerTestPCH.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p ../IntermediateDir/ServerTest/gccDebug
	mkdir -p ../../../../Binary/gccTools
	mkdir -p ../IntermediateDir/ServerTest/gccRelease
	mkdir -p ../../../../Binary/gccTools

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f ../IntermediateDir/ServerTest/gccDebug/*.o
	rm -f ../IntermediateDir/ServerTest/gccDebug/*.d
	rm -f ../../../../Binary/gccTools/*.a
	rm -f ../../../../Binary/gccTools/*.so
	rm -f ../../../../Binary/gccTools/*.dll
	rm -f ../../../../Binary/gccTools/*.exe
	rm -f ../IntermediateDir/ServerTest/gccRelease/*.o
	rm -f ../IntermediateDir/ServerTest/gccRelease/*.d
	rm -f ../../../../Binary/gccTools/*.a
	rm -f ../../../../Binary/gccTools/*.so
	rm -f ../../../../Binary/gccTools/*.dll
	rm -f ../../../../Binary/gccTools/*.exe

