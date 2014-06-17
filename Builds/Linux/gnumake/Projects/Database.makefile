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
Debug: create_folders ../IntermediateDir/Database/gccDebug/Connection.o ../IntermediateDir/Database/gccDebug/DatabaseService.o ../IntermediateDir/Database/gccDebug/DatabasePCH.o ../IntermediateDir/Database/gccDebug/Task.o ../IntermediateDir/Database/gccDebug/DirectExecuteTask.o post_buid_event 
	ar rcs ../../../../Library/Database/gccLib/libDatabase.a ../IntermediateDir/Database/gccDebug/Connection.o ../IntermediateDir/Database/gccDebug/DatabaseService.o ../IntermediateDir/Database/gccDebug/DatabasePCH.o ../IntermediateDir/Database/gccDebug/Task.o ../IntermediateDir/Database/gccDebug/DirectExecuteTask.o  $(Debug_Implicitly_Linked_Objects)

#Compiles PCH ../../../../Source/Common/Database/DatabasePCH.h for the Debug configuration...
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) $(Debug_Include_Path) ../../../../Source/Common/Database/DatabasePCH.h
	
# Compiles file ../../../../Source/Common/Database/Connection.cpp for the Debug configuration...
-include ../IntermediateDir/Database/gccDebug/Connection.d
../IntermediateDir/Database/gccDebug/Connection.o: ../../../../Source/Common/Database/Connection.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Database/Connection.cpp $(Debug_Include_Path) -o ../IntermediateDir/Database/gccDebug/Connection.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Database/Connection.cpp $(Debug_Include_Path) > ../IntermediateDir/Database/gccDebug/Connection.d

# Compiles file ../../../../Source/Common/Database/DatabaseService.cpp for the Debug configuration...
-include ../IntermediateDir/Database/gccDebug/DatabaseService.d
../IntermediateDir/Database/gccDebug/DatabaseService.o: ../../../../Source/Common/Database/DatabaseService.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Database/DatabaseService.cpp $(Debug_Include_Path) -o ../IntermediateDir/Database/gccDebug/DatabaseService.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Database/DatabaseService.cpp $(Debug_Include_Path) > ../IntermediateDir/Database/gccDebug/DatabaseService.d

# Compiles file ../../../../Source/Common/Database/DatabasePCH.cpp for the Debug configuration...
-include ../IntermediateDir/Database/gccDebug/DatabasePCH.d
../IntermediateDir/Database/gccDebug/DatabasePCH.o: ../../../../Source/Common/Database/DatabasePCH.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Database/DatabasePCH.cpp $(Debug_Include_Path) -o ../IntermediateDir/Database/gccDebug/DatabasePCH.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Database/DatabasePCH.cpp $(Debug_Include_Path) > ../IntermediateDir/Database/gccDebug/DatabasePCH.d

# Compiles file ../../../../Source/Common/Database/Task.cpp for the Debug configuration...
-include ../IntermediateDir/Database/gccDebug/Task.d
../IntermediateDir/Database/gccDebug/Task.o: ../../../../Source/Common/Database/Task.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Database/Task.cpp $(Debug_Include_Path) -o ../IntermediateDir/Database/gccDebug/Task.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Database/Task.cpp $(Debug_Include_Path) > ../IntermediateDir/Database/gccDebug/Task.d

# Compiles file ../../../../Source/Common/Database/DirectExecuteTask.cpp for the Debug configuration...
-include ../IntermediateDir/Database/gccDebug/DirectExecuteTask.d
../IntermediateDir/Database/gccDebug/DirectExecuteTask.o: ../../../../Source/Common/Database/DirectExecuteTask.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Database/DirectExecuteTask.cpp $(Debug_Include_Path) -o ../IntermediateDir/Database/gccDebug/DirectExecuteTask.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Database/DirectExecuteTask.cpp $(Debug_Include_Path) > ../IntermediateDir/Database/gccDebug/DirectExecuteTask.d

# Builds the Release configuration...
.PHONY: Release
Release: create_folders ../IntermediateDir/Database/gccRelease/Connection.o ../IntermediateDir/Database/gccRelease/DatabaseService.o ../IntermediateDir/Database/gccRelease/DatabasePCH.o ../IntermediateDir/Database/gccRelease/Task.o ../IntermediateDir/Database/gccRelease/DirectExecuteTask.o 
	ar rcs ../../../../Library/Database/gccLib/libDatabase.a ../IntermediateDir/Database/gccRelease/Connection.o ../IntermediateDir/Database/gccRelease/DatabaseService.o ../IntermediateDir/Database/gccRelease/DatabasePCH.o ../IntermediateDir/Database/gccRelease/Task.o ../IntermediateDir/Database/gccRelease/DirectExecuteTask.o  $(Release_Implicitly_Linked_Objects)

# Compiles file ../../../../Source/Common/Database/Connection.cpp for the Release configuration...
-include ../IntermediateDir/Database/gccRelease/Connection.d
../IntermediateDir/Database/gccRelease/Connection.o: ../../../../Source/Common/Database/Connection.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Database/Connection.cpp $(Release_Include_Path) -o ../IntermediateDir/Database/gccRelease/Connection.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Database/Connection.cpp $(Release_Include_Path) > ../IntermediateDir/Database/gccRelease/Connection.d

# Compiles file ../../../../Source/Common/Database/DatabaseService.cpp for the Release configuration...
-include ../IntermediateDir/Database/gccRelease/DatabaseService.d
../IntermediateDir/Database/gccRelease/DatabaseService.o: ../../../../Source/Common/Database/DatabaseService.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Database/DatabaseService.cpp $(Release_Include_Path) -o ../IntermediateDir/Database/gccRelease/DatabaseService.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Database/DatabaseService.cpp $(Release_Include_Path) > ../IntermediateDir/Database/gccRelease/DatabaseService.d

# Compiles file ../../../../Source/Common/Database/DatabasePCH.cpp for the Release configuration...
-include ../IntermediateDir/Database/gccRelease/DatabasePCH.d
../IntermediateDir/Database/gccRelease/DatabasePCH.o: ../../../../Source/Common/Database/DatabasePCH.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Database/DatabasePCH.cpp $(Release_Include_Path) -o ../IntermediateDir/Database/gccRelease/DatabasePCH.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Database/DatabasePCH.cpp $(Release_Include_Path) > ../IntermediateDir/Database/gccRelease/DatabasePCH.d

# Compiles file ../../../../Source/Common/Database/Task.cpp for the Release configuration...
-include ../IntermediateDir/Database/gccRelease/Task.d
../IntermediateDir/Database/gccRelease/Task.o: ../../../../Source/Common/Database/Task.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Database/Task.cpp $(Release_Include_Path) -o ../IntermediateDir/Database/gccRelease/Task.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Database/Task.cpp $(Release_Include_Path) > ../IntermediateDir/Database/gccRelease/Task.d

# Compiles file ../../../../Source/Common/Database/DirectExecuteTask.cpp for the Release configuration...
-include ../IntermediateDir/Database/gccRelease/DirectExecuteTask.d
../IntermediateDir/Database/gccRelease/DirectExecuteTask.o: ../../../../Source/Common/Database/DirectExecuteTask.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Database/DirectExecuteTask.cpp $(Release_Include_Path) -o ../IntermediateDir/Database/gccRelease/DirectExecuteTask.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Database/DirectExecuteTask.cpp $(Release_Include_Path) > ../IntermediateDir/Database/gccRelease/DirectExecuteTask.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p ../IntermediateDir/Database/gccDebug
	mkdir -p ../../../../Library/Database/gccLib
	mkdir -p ../IntermediateDir/Database/gccRelease
	mkdir -p ../../../../Library/Database/gccLib
	
.PHONY: post_buid_event
post_build_event:
	mkdir -p ../../../../Library/Database/Database
	rm ../../../../Library/Database/Database/*.h
	cp ../../../../Source/Database/*.h ../../../../Library/Database/Database/

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f ../IntermediateDir/Database/gccDebug/*.o
	rm -f ../IntermediateDir/Database/gccDebug/*.d
	rm -f ../../../../Library/Database/gccLib/*.a
	rm -f ../../../../Library/Database/gccLib/*.so
	rm -f ../../../../Library/Database/gccLib/*.dll
	rm -f ../../../../Library/Database/gccLib/*.exe
	rm -f ../IntermediateDir/Database/gccRelease/*.o
	rm -f ../IntermediateDir/Database/gccRelease/*.d
	rm -f ../../../../Library/Database/gccLib/*.a
	rm -f ../../../../Library/Database/gccLib/*.so
	rm -f ../../../../Library/Database/gccLib/*.dll
	rm -f ../../../../Library/Database/gccLib/*.exe

