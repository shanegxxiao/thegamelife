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
Debug: create_folders ../IntermediateDir/ClientTest/gccDebug/SSLClient.o ../IntermediateDir/ClientTest/gccDebug/ClientTestPCH.o ../IntermediateDir/ClientTest/gccDebug/ConCallback.o ../IntermediateDir/ClientTest/gccDebug/Main.o 
	g++ ../IntermediateDir/ClientTest/gccDebug/SSLClient.o ../IntermediateDir/ClientTest/gccDebug/ClientTestPCH.o ../IntermediateDir/ClientTest/gccDebug/ConCallback.o ../IntermediateDir/ClientTest/gccDebug/Main.o  $(Debug_Library_Path) $(Debug_Libraries) -Wl,-rpath,./ -o ../../../../Binary/gccTools/ClientTest.exe

#Compiles PCH ../../../../Source/UnitTest/ClientTest/ClientTestPCH.h for the Debug configuration...
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) $(Debug_Include_Path) ../../../../Source/UnitTest/ClientTest/ClientTestPCH.h
	
# Compiles file ../../../../Source/UnitTest/ClientTest/SSLClient.cpp for the Debug configuration...
-include ../IntermediateDir/ClientTest/gccDebug/SSLClient.d
../IntermediateDir/ClientTest/gccDebug/SSLClient.o: ../../../../Source/UnitTest/ClientTest/SSLClient.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/ClientTest/SSLClient.cpp $(Debug_Include_Path) -o ../IntermediateDir/ClientTest/gccDebug/SSLClient.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/ClientTest/SSLClient.cpp $(Debug_Include_Path) > ../IntermediateDir/ClientTest/gccDebug/SSLClient.d

# Compiles file ../../../../Source/UnitTest/ClientTest/ClientTestPCH.cpp for the Debug configuration...
-include ../IntermediateDir/ClientTest/gccDebug/ClientTestPCH.d
../IntermediateDir/ClientTest/gccDebug/ClientTestPCH.o: ../../../../Source/UnitTest/ClientTest/ClientTestPCH.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/ClientTest/ClientTestPCH.cpp $(Debug_Include_Path) -o ../IntermediateDir/ClientTest/gccDebug/ClientTestPCH.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/ClientTest/ClientTestPCH.cpp $(Debug_Include_Path) > ../IntermediateDir/ClientTest/gccDebug/ClientTestPCH.d

# Compiles file ../../../../Source/UnitTest/ClientTest/ConCallback.cpp for the Debug configuration...
-include ../IntermediateDir/ClientTest/gccDebug/ConCallback.d
../IntermediateDir/ClientTest/gccDebug/ConCallback.o: ../../../../Source/UnitTest/ClientTest/ConCallback.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/ClientTest/ConCallback.cpp $(Debug_Include_Path) -o ../IntermediateDir/ClientTest/gccDebug/ConCallback.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/ClientTest/ConCallback.cpp $(Debug_Include_Path) > ../IntermediateDir/ClientTest/gccDebug/ConCallback.d

# Compiles file ../../../../Source/UnitTest/ClientTest/Main.cpp for the Debug configuration...
-include ../IntermediateDir/ClientTest/gccDebug/Main.d
../IntermediateDir/ClientTest/gccDebug/Main.o: ../../../../Source/UnitTest/ClientTest/Main.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/UnitTest/ClientTest/Main.cpp $(Debug_Include_Path) -o ../IntermediateDir/ClientTest/gccDebug/Main.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/UnitTest/ClientTest/Main.cpp $(Debug_Include_Path) > ../IntermediateDir/ClientTest/gccDebug/Main.d

# Builds the Release configuration...
.PHONY: Release
Release: create_folders ../IntermediateDir/ClientTest/gccRelease/SSLClient.o ../IntermediateDir/ClientTest/gccRelease/ClientTestPCH.o ../IntermediateDir/ClientTest/gccRelease/ConCallback.o ../IntermediateDir/ClientTest/gccRelease/Main.o 
	g++ ../IntermediateDir/ClientTest/gccRelease/SSLClient.o ../IntermediateDir/ClientTest/gccRelease/ClientTestPCH.o ../IntermediateDir/ClientTest/gccRelease/ConCallback.o ../IntermediateDir/ClientTest/gccRelease/Main.o  $(Release_Library_Path) $(Release_Libraries) -Wl,-rpath,./ -o ../../../../Binary/gccTools/ClientTest.exe

# Compiles file ../../../../Source/UnitTest/ClientTest/SSLClient.cpp for the Release configuration...
-include ../IntermediateDir/ClientTest/gccRelease/SSLClient.d
../IntermediateDir/ClientTest/gccRelease/SSLClient.o: ../../../../Source/UnitTest/ClientTest/SSLClient.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/ClientTest/SSLClient.cpp $(Release_Include_Path) -o ../IntermediateDir/ClientTest/gccRelease/SSLClient.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/ClientTest/SSLClient.cpp $(Release_Include_Path) > ../IntermediateDir/ClientTest/gccRelease/SSLClient.d

# Compiles file ../../../../Source/UnitTest/ClientTest/ClientTestPCH.cpp for the Release configuration...
-include ../IntermediateDir/ClientTest/gccRelease/ClientTestPCH.d
../IntermediateDir/ClientTest/gccRelease/ClientTestPCH.o: ../../../../Source/UnitTest/ClientTest/ClientTestPCH.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/ClientTest/ClientTestPCH.cpp $(Release_Include_Path) -o ../IntermediateDir/ClientTest/gccRelease/ClientTestPCH.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/ClientTest/ClientTestPCH.cpp $(Release_Include_Path) > ../IntermediateDir/ClientTest/gccRelease/ClientTestPCH.d

# Compiles file ../../../../Source/UnitTest/ClientTest/ConCallback.cpp for the Release configuration...
-include ../IntermediateDir/ClientTest/gccRelease/ConCallback.d
../IntermediateDir/ClientTest/gccRelease/ConCallback.o: ../../../../Source/UnitTest/ClientTest/ConCallback.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/ClientTest/ConCallback.cpp $(Release_Include_Path) -o ../IntermediateDir/ClientTest/gccRelease/ConCallback.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/ClientTest/ConCallback.cpp $(Release_Include_Path) > ../IntermediateDir/ClientTest/gccRelease/ConCallback.d

# Compiles file ../../../../Source/UnitTest/ClientTest/Main.cpp for the Release configuration...
-include ../IntermediateDir/ClientTest/gccRelease/Main.d
../IntermediateDir/ClientTest/gccRelease/Main.o: ../../../../Source/UnitTest/ClientTest/Main.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/UnitTest/ClientTest/Main.cpp $(Release_Include_Path) -o ../IntermediateDir/ClientTest/gccRelease/Main.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/UnitTest/ClientTest/Main.cpp $(Release_Include_Path) > ../IntermediateDir/ClientTest/gccRelease/Main.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p ../IntermediateDir/ClientTest/gccDebug
	mkdir -p ../../../../Binary/gccTools
	mkdir -p ../IntermediateDir/ClientTest/gccRelease
	mkdir -p ../../../../Binary/gccTools

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f ../IntermediateDir/ClientTest/gccDebug/*.o
	rm -f ../IntermediateDir/ClientTest/gccDebug/*.d
	rm -f ../../../../Binary/gccTools/*.a
	rm -f ../../../../Binary/gccTools/*.so
	rm -f ../../../../Binary/gccTools/*.dll
	rm -f ../../../../Binary/gccTools/*.exe
	rm -f ../IntermediateDir/ClientTest/gccRelease/*.o
	rm -f ../IntermediateDir/ClientTest/gccRelease/*.d
	rm -f ../../../../Binary/gccTools/*.a
	rm -f ../../../../Binary/gccTools/*.so
	rm -f ../../../../Binary/gccTools/*.dll
	rm -f ../../../../Binary/gccTools/*.exe

