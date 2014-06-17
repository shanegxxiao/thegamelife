# Included makefiles
include Common.makefile

# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_Include_Path=$(Common_Include_Path)
Release_Include_Path=$(Debug_Include_Path)

# Library paths...
Debug_Library_Path=-L"../../../../Library/Utility/gccLib"
Release_Library_Path=$(Debug_Library_Path)

# Additional libraries...
Debug_Libraries=-Wl,--start-group -lUtility  -Wl,--end-group
Release_Libraries=-Wl,--start-group -lUtility  -Wl,--end-group

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
Debug: create_folders ../IntermediateDir/Runtime/gccDebug/ConsoleSystem.o ../IntermediateDir/Runtime/gccDebug/LogFile.o ../IntermediateDir/Runtime/gccDebug/LogService.o ../IntermediateDir/Runtime/gccDebug/LogSystem.o ../IntermediateDir/Runtime/gccDebug/LogTarget.o ../IntermediateDir/Runtime/gccDebug/LogToVSOutput.o ../IntermediateDir/Runtime/gccDebug/Object.o ../IntermediateDir/Runtime/gccDebug/ObjectCreator.o ../IntermediateDir/Runtime/gccDebug/ObjectSystem.o ../IntermediateDir/Runtime/gccDebug/Param.o ../IntermediateDir/Runtime/gccDebug/ParamList.o ../IntermediateDir/Runtime/gccDebug/RefObject.o ../IntermediateDir/Runtime/gccDebug/RuntimePCH.o post_buid_event 
	ar rcs ../../../../Library/Runtime/gccLib/libRuntime.a ../IntermediateDir/Runtime/gccDebug/ConsoleSystem.o ../IntermediateDir/Runtime/gccDebug/LogFile.o ../IntermediateDir/Runtime/gccDebug/LogService.o ../IntermediateDir/Runtime/gccDebug/LogSystem.o ../IntermediateDir/Runtime/gccDebug/LogTarget.o ../IntermediateDir/Runtime/gccDebug/LogToVSOutput.o ../IntermediateDir/Runtime/gccDebug/Object.o ../IntermediateDir/Runtime/gccDebug/ObjectCreator.o ../IntermediateDir/Runtime/gccDebug/ObjectSystem.o ../IntermediateDir/Runtime/gccDebug/Param.o ../IntermediateDir/Runtime/gccDebug/ParamList.o ../IntermediateDir/Runtime/gccDebug/RefObject.o ../IntermediateDir/Runtime/gccDebug/RuntimePCH.o  $(Debug_Implicitly_Linked_Objects)

#Compiles PCH ../../../../Source/Common/Runtime/RuntimePCH.h for the Debug configuration...
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) $(Debug_Include_Path) ../../../../Source/Common/Runtime/RuntimePCH.h
	
# Compiles file ../../../../Source/Common/Runtime/ConsoleSystem.cpp for the Debug configuration...
-include ../IntermediateDir/Runtime/gccDebug/ConsoleSystem.d
../IntermediateDir/Runtime/gccDebug/ConsoleSystem.o: ../../../../Source/Common/Runtime/ConsoleSystem.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Runtime/ConsoleSystem.cpp $(Debug_Include_Path) -o ../IntermediateDir/Runtime/gccDebug/ConsoleSystem.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Runtime/ConsoleSystem.cpp $(Debug_Include_Path) > ../IntermediateDir/Runtime/gccDebug/ConsoleSystem.d

# Compiles file ../../../../Source/Common/Runtime/LogFile.cpp for the Debug configuration...
-include ../IntermediateDir/Runtime/gccDebug/LogFile.d
../IntermediateDir/Runtime/gccDebug/LogFile.o: ../../../../Source/Common/Runtime/LogFile.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Runtime/LogFile.cpp $(Debug_Include_Path) -o ../IntermediateDir/Runtime/gccDebug/LogFile.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Runtime/LogFile.cpp $(Debug_Include_Path) > ../IntermediateDir/Runtime/gccDebug/LogFile.d

# Compiles file ../../../../Source/Common/Runtime/LogService.cpp for the Debug configuration...
-include ../IntermediateDir/Runtime/gccDebug/LogService.d
../IntermediateDir/Runtime/gccDebug/LogService.o: ../../../../Source/Common/Runtime/LogService.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Runtime/LogService.cpp $(Debug_Include_Path) -o ../IntermediateDir/Runtime/gccDebug/LogService.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Runtime/LogService.cpp $(Debug_Include_Path) > ../IntermediateDir/Runtime/gccDebug/LogService.d

# Compiles file ../../../../Source/Common/Runtime/LogSystem.cpp for the Debug configuration...
-include ../IntermediateDir/Runtime/gccDebug/LogSystem.d
../IntermediateDir/Runtime/gccDebug/LogSystem.o: ../../../../Source/Common/Runtime/LogSystem.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Runtime/LogSystem.cpp $(Debug_Include_Path) -o ../IntermediateDir/Runtime/gccDebug/LogSystem.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Runtime/LogSystem.cpp $(Debug_Include_Path) > ../IntermediateDir/Runtime/gccDebug/LogSystem.d

# Compiles file ../../../../Source/Common/Runtime/LogTarget.cpp for the Debug configuration...
-include ../IntermediateDir/Runtime/gccDebug/LogTarget.d
../IntermediateDir/Runtime/gccDebug/LogTarget.o: ../../../../Source/Common/Runtime/LogTarget.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Runtime/LogTarget.cpp $(Debug_Include_Path) -o ../IntermediateDir/Runtime/gccDebug/LogTarget.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Runtime/LogTarget.cpp $(Debug_Include_Path) > ../IntermediateDir/Runtime/gccDebug/LogTarget.d

# Compiles file ../../../../Source/Common/Runtime/LogToVSOutput.cpp for the Debug configuration...
-include ../IntermediateDir/Runtime/gccDebug/LogToVSOutput.d
../IntermediateDir/Runtime/gccDebug/LogToVSOutput.o: ../../../../Source/Common/Runtime/LogToVSOutput.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Runtime/LogToVSOutput.cpp $(Debug_Include_Path) -o ../IntermediateDir/Runtime/gccDebug/LogToVSOutput.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Runtime/LogToVSOutput.cpp $(Debug_Include_Path) > ../IntermediateDir/Runtime/gccDebug/LogToVSOutput.d

# Compiles file ../../../../Source/Common/Runtime/Object.cpp for the Debug configuration...
-include ../IntermediateDir/Runtime/gccDebug/Object.d
../IntermediateDir/Runtime/gccDebug/Object.o: ../../../../Source/Common/Runtime/Object.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Runtime/Object.cpp $(Debug_Include_Path) -o ../IntermediateDir/Runtime/gccDebug/Object.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Runtime/Object.cpp $(Debug_Include_Path) > ../IntermediateDir/Runtime/gccDebug/Object.d

# Compiles file ../../../../Source/Common/Runtime/ObjectCreator.cpp for the Debug configuration...
-include ../IntermediateDir/Runtime/gccDebug/ObjectCreator.d
../IntermediateDir/Runtime/gccDebug/ObjectCreator.o: ../../../../Source/Common/Runtime/ObjectCreator.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Runtime/ObjectCreator.cpp $(Debug_Include_Path) -o ../IntermediateDir/Runtime/gccDebug/ObjectCreator.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Runtime/ObjectCreator.cpp $(Debug_Include_Path) > ../IntermediateDir/Runtime/gccDebug/ObjectCreator.d

# Compiles file ../../../../Source/Common/Runtime/ObjectSystem.cpp for the Debug configuration...
-include ../IntermediateDir/Runtime/gccDebug/ObjectSystem.d
../IntermediateDir/Runtime/gccDebug/ObjectSystem.o: ../../../../Source/Common/Runtime/ObjectSystem.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Runtime/ObjectSystem.cpp $(Debug_Include_Path) -o ../IntermediateDir/Runtime/gccDebug/ObjectSystem.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Runtime/ObjectSystem.cpp $(Debug_Include_Path) > ../IntermediateDir/Runtime/gccDebug/ObjectSystem.d

# Compiles file ../../../../Source/Common/Runtime/Param.cpp for the Debug configuration...
-include ../IntermediateDir/Runtime/gccDebug/Param.d
../IntermediateDir/Runtime/gccDebug/Param.o: ../../../../Source/Common/Runtime/Param.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Runtime/Param.cpp $(Debug_Include_Path) -o ../IntermediateDir/Runtime/gccDebug/Param.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Runtime/Param.cpp $(Debug_Include_Path) > ../IntermediateDir/Runtime/gccDebug/Param.d

# Compiles file ../../../../Source/Common/Runtime/ParamList.cpp for the Debug configuration...
-include ../IntermediateDir/Runtime/gccDebug/ParamList.d
../IntermediateDir/Runtime/gccDebug/ParamList.o: ../../../../Source/Common/Runtime/ParamList.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Runtime/ParamList.cpp $(Debug_Include_Path) -o ../IntermediateDir/Runtime/gccDebug/ParamList.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Runtime/ParamList.cpp $(Debug_Include_Path) > ../IntermediateDir/Runtime/gccDebug/ParamList.d

# Compiles file ../../../../Source/Common/Runtime/RefObject.cpp for the Debug configuration...
-include ../IntermediateDir/Runtime/gccDebug/RefObject.d
../IntermediateDir/Runtime/gccDebug/RefObject.o: ../../../../Source/Common/Runtime/RefObject.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Runtime/RefObject.cpp $(Debug_Include_Path) -o ../IntermediateDir/Runtime/gccDebug/RefObject.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Runtime/RefObject.cpp $(Debug_Include_Path) > ../IntermediateDir/Runtime/gccDebug/RefObject.d

# Compiles file ../../../../Source/Common/Runtime/RuntimePCH.cpp for the Debug configuration...
-include ../IntermediateDir/Runtime/gccDebug/RuntimePCH.d
../IntermediateDir/Runtime/gccDebug/RuntimePCH.o: ../../../../Source/Common/Runtime/RuntimePCH.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Runtime/RuntimePCH.cpp $(Debug_Include_Path) -o ../IntermediateDir/Runtime/gccDebug/RuntimePCH.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Runtime/RuntimePCH.cpp $(Debug_Include_Path) > ../IntermediateDir/Runtime/gccDebug/RuntimePCH.d

# Builds the Release configuration...
.PHONY: Release
Release: create_folders ../IntermediateDir/Runtime/gccRelease/ConsoleSystem.o ../IntermediateDir/Runtime/gccRelease/LogFile.o ../IntermediateDir/Runtime/gccRelease/LogService.o ../IntermediateDir/Runtime/gccRelease/LogSystem.o ../IntermediateDir/Runtime/gccRelease/LogTarget.o ../IntermediateDir/Runtime/gccRelease/LogToVSOutput.o ../IntermediateDir/Runtime/gccRelease/Object.o ../IntermediateDir/Runtime/gccRelease/ObjectCreator.o ../IntermediateDir/Runtime/gccRelease/ObjectSystem.o ../IntermediateDir/Runtime/gccRelease/Param.o ../IntermediateDir/Runtime/gccRelease/ParamList.o ../IntermediateDir/Runtime/gccRelease/RefObject.o ../IntermediateDir/Runtime/gccRelease/RuntimePCH.o 
	ar rcs ../../../../Library/Runtime/gccLib/libRuntime.a ../IntermediateDir/Runtime/gccRelease/ConsoleSystem.o ../IntermediateDir/Runtime/gccRelease/LogFile.o ../IntermediateDir/Runtime/gccRelease/LogService.o ../IntermediateDir/Runtime/gccRelease/LogSystem.o ../IntermediateDir/Runtime/gccRelease/LogTarget.o ../IntermediateDir/Runtime/gccRelease/LogToVSOutput.o ../IntermediateDir/Runtime/gccRelease/Object.o ../IntermediateDir/Runtime/gccRelease/ObjectCreator.o ../IntermediateDir/Runtime/gccRelease/ObjectSystem.o ../IntermediateDir/Runtime/gccRelease/Param.o ../IntermediateDir/Runtime/gccRelease/ParamList.o ../IntermediateDir/Runtime/gccRelease/RefObject.o ../IntermediateDir/Runtime/gccRelease/RuntimePCH.o  $(Release_Implicitly_Linked_Objects)

# Compiles file ../../../../Source/Common/Runtime/ConsoleSystem.cpp for the Release configuration...
-include ../IntermediateDir/Runtime/gccRelease/ConsoleSystem.d
../IntermediateDir/Runtime/gccRelease/ConsoleSystem.o: ../../../../Source/Common/Runtime/ConsoleSystem.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Runtime/ConsoleSystem.cpp $(Release_Include_Path) -o ../IntermediateDir/Runtime/gccRelease/ConsoleSystem.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Runtime/ConsoleSystem.cpp $(Release_Include_Path) > ../IntermediateDir/Runtime/gccRelease/ConsoleSystem.d

# Compiles file ../../../../Source/Common/Runtime/LogFile.cpp for the Release configuration...
-include ../IntermediateDir/Runtime/gccRelease/LogFile.d
../IntermediateDir/Runtime/gccRelease/LogFile.o: ../../../../Source/Common/Runtime/LogFile.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Runtime/LogFile.cpp $(Release_Include_Path) -o ../IntermediateDir/Runtime/gccRelease/LogFile.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Runtime/LogFile.cpp $(Release_Include_Path) > ../IntermediateDir/Runtime/gccRelease/LogFile.d

# Compiles file ../../../../Source/Common/Runtime/LogService.cpp for the Release configuration...
-include ../IntermediateDir/Runtime/gccRelease/LogService.d
../IntermediateDir/Runtime/gccRelease/LogService.o: ../../../../Source/Common/Runtime/LogService.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Runtime/LogService.cpp $(Release_Include_Path) -o ../IntermediateDir/Runtime/gccRelease/LogService.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Runtime/LogService.cpp $(Release_Include_Path) > ../IntermediateDir/Runtime/gccRelease/LogService.d

# Compiles file ../../../../Source/Common/Runtime/LogSystem.cpp for the Release configuration...
-include ../IntermediateDir/Runtime/gccRelease/LogSystem.d
../IntermediateDir/Runtime/gccRelease/LogSystem.o: ../../../../Source/Common/Runtime/LogSystem.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Runtime/LogSystem.cpp $(Release_Include_Path) -o ../IntermediateDir/Runtime/gccRelease/LogSystem.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Runtime/LogSystem.cpp $(Release_Include_Path) > ../IntermediateDir/Runtime/gccRelease/LogSystem.d

# Compiles file ../../../../Source/Common/Runtime/LogTarget.cpp for the Release configuration...
-include ../IntermediateDir/Runtime/gccRelease/LogTarget.d
../IntermediateDir/Runtime/gccRelease/LogTarget.o: ../../../../Source/Common/Runtime/LogTarget.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Runtime/LogTarget.cpp $(Release_Include_Path) -o ../IntermediateDir/Runtime/gccRelease/LogTarget.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Runtime/LogTarget.cpp $(Release_Include_Path) > ../IntermediateDir/Runtime/gccRelease/LogTarget.d

# Compiles file ../../../../Source/Common/Runtime/LogToVSOutput.cpp for the Release configuration...
-include ../IntermediateDir/Runtime/gccRelease/LogToVSOutput.d
../IntermediateDir/Runtime/gccRelease/LogToVSOutput.o: ../../../../Source/Common/Runtime/LogToVSOutput.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Runtime/LogToVSOutput.cpp $(Release_Include_Path) -o ../IntermediateDir/Runtime/gccRelease/LogToVSOutput.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Runtime/LogToVSOutput.cpp $(Release_Include_Path) > ../IntermediateDir/Runtime/gccRelease/LogToVSOutput.d

# Compiles file ../../../../Source/Common/Runtime/Object.cpp for the Release configuration...
-include ../IntermediateDir/Runtime/gccRelease/Object.d
../IntermediateDir/Runtime/gccRelease/Object.o: ../../../../Source/Common/Runtime/Object.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Runtime/Object.cpp $(Release_Include_Path) -o ../IntermediateDir/Runtime/gccRelease/Object.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Runtime/Object.cpp $(Release_Include_Path) > ../IntermediateDir/Runtime/gccRelease/Object.d

# Compiles file ../../../../Source/Common/Runtime/ObjectCreator.cpp for the Release configuration...
-include ../IntermediateDir/Runtime/gccRelease/ObjectCreator.d
../IntermediateDir/Runtime/gccRelease/ObjectCreator.o: ../../../../Source/Common/Runtime/ObjectCreator.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Runtime/ObjectCreator.cpp $(Release_Include_Path) -o ../IntermediateDir/Runtime/gccRelease/ObjectCreator.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Runtime/ObjectCreator.cpp $(Release_Include_Path) > ../IntermediateDir/Runtime/gccRelease/ObjectCreator.d

# Compiles file ../../../../Source/Common/Runtime/ObjectSystem.cpp for the Release configuration...
-include ../IntermediateDir/Runtime/gccRelease/ObjectSystem.d
../IntermediateDir/Runtime/gccRelease/ObjectSystem.o: ../../../../Source/Common/Runtime/ObjectSystem.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Runtime/ObjectSystem.cpp $(Release_Include_Path) -o ../IntermediateDir/Runtime/gccRelease/ObjectSystem.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Runtime/ObjectSystem.cpp $(Release_Include_Path) > ../IntermediateDir/Runtime/gccRelease/ObjectSystem.d

# Compiles file ../../../../Source/Common/Runtime/Param.cpp for the Release configuration...
-include ../IntermediateDir/Runtime/gccRelease/Param.d
../IntermediateDir/Runtime/gccRelease/Param.o: ../../../../Source/Common/Runtime/Param.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Runtime/Param.cpp $(Release_Include_Path) -o ../IntermediateDir/Runtime/gccRelease/Param.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Runtime/Param.cpp $(Release_Include_Path) > ../IntermediateDir/Runtime/gccRelease/Param.d

# Compiles file ../../../../Source/Common/Runtime/ParamList.cpp for the Release configuration...
-include ../IntermediateDir/Runtime/gccRelease/ParamList.d
../IntermediateDir/Runtime/gccRelease/ParamList.o: ../../../../Source/Common/Runtime/ParamList.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Runtime/ParamList.cpp $(Release_Include_Path) -o ../IntermediateDir/Runtime/gccRelease/ParamList.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Runtime/ParamList.cpp $(Release_Include_Path) > ../IntermediateDir/Runtime/gccRelease/ParamList.d

# Compiles file ../../../../Source/Common/Runtime/RefObject.cpp for the Release configuration...
-include ../IntermediateDir/Runtime/gccRelease/RefObject.d
../IntermediateDir/Runtime/gccRelease/RefObject.o: ../../../../Source/Common/Runtime/RefObject.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Runtime/RefObject.cpp $(Release_Include_Path) -o ../IntermediateDir/Runtime/gccRelease/RefObject.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Runtime/RefObject.cpp $(Release_Include_Path) > ../IntermediateDir/Runtime/gccRelease/RefObject.d

# Compiles file ../../../../Source/Common/Runtime/RuntimePCH.cpp for the Release configuration...
-include ../IntermediateDir/Runtime/gccRelease/RuntimePCH.d
../IntermediateDir/Runtime/gccRelease/RuntimePCH.o: ../../../../Source/Common/Runtime/RuntimePCH.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Runtime/RuntimePCH.cpp $(Release_Include_Path) -o ../IntermediateDir/Runtime/gccRelease/RuntimePCH.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Runtime/RuntimePCH.cpp $(Release_Include_Path) > ../IntermediateDir/Runtime/gccRelease/RuntimePCH.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p ../IntermediateDir/Runtime/gccDebug
	mkdir -p ../../../../Library/Runtime/gccLib
	mkdir -p ../IntermediateDir/Runtime/gccRelease
	mkdir -p ../../../../Library/Runtime/gccLib
	
# Copy all header files to library include folder	
.PHONY: post_buid_event
post_build_event:
	mkdir -p ../../../../Library/Runtime/Runtime
	rm ../../../../Library/Runtime/Runtime/*.h
	cp ../../../../Source/Runtime/*.h ../../../../Library/Runtime/Runtime/

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f ../IntermediateDir/Runtime/gccDebug/*.o
	rm -f ../IntermediateDir/Runtime/gccDebug/*.d
	rm -f ../../../../Library/Runtime/gccLib/*.a
	rm -f ../../../../Library/Runtime/gccLib/*.so
	rm -f ../../../../Library/Runtime/gccLib/*.dll
	rm -f ../../../../Library/Runtime/gccLib/*.exe
	rm -f ../IntermediateDir/Runtime/gccRelease/*.o
	rm -f ../IntermediateDir/Runtime/gccRelease/*.d
	rm -f ../../../../Library/Runtime/gccLib/*.a
	rm -f ../../../../Library/Runtime/gccLib/*.so
	rm -f ../../../../Library/Runtime/gccLib/*.dll
	rm -f ../../../../Library/Runtime/gccLib/*.exe

