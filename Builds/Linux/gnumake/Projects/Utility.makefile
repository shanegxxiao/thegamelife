# Included makefiles
include Common.makefile

# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_Include_Path=$(Common_Include_Path)
Release_Include_Path=$(Debug_Include_Path)

# Library paths...
Debug_Library_Path=
Release_Library_Path=

# Additional libraries...
Debug_Libraries=
Release_Libraries=

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
Debug: create_folders ../IntermediateDir/Utility/gccDebug/Debug.o ../IntermediateDir/Utility/gccDebug/FileSystemHelper.o ../IntermediateDir/Utility/gccDebug/FrameRate.o ../IntermediateDir/Utility/gccDebug/pugixml.o ../IntermediateDir/Utility/gccDebug/StringHelper.o ../IntermediateDir/Utility/gccDebug/tinystr.o ../IntermediateDir/Utility/gccDebug/tinyxml.o ../IntermediateDir/Utility/gccDebug/tinyxmlerror.o ../IntermediateDir/Utility/gccDebug/tinyxmlparser.o ../IntermediateDir/Utility/gccDebug/UtilityPCH.o post_buid_event 
	ar rcs ../../../../Library/Utility/gccLib/libUtility.a ../IntermediateDir/Utility/gccDebug/Debug.o ../IntermediateDir/Utility/gccDebug/FileSystemHelper.o ../IntermediateDir/Utility/gccDebug/FrameRate.o ../IntermediateDir/Utility/gccDebug/pugixml.o ../IntermediateDir/Utility/gccDebug/StringHelper.o ../IntermediateDir/Utility/gccDebug/tinystr.o ../IntermediateDir/Utility/gccDebug/tinyxml.o ../IntermediateDir/Utility/gccDebug/tinyxmlerror.o ../IntermediateDir/Utility/gccDebug/tinyxmlparser.o ../IntermediateDir/Utility/gccDebug/UtilityPCH.o  $(Debug_Implicitly_Linked_Objects)

#Compiles PCH ../../../../Source/Common/Utility/UtilityPCH.h for the Debug configuration...
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) $(Debug_Include_Path) ../../../../Source/Common/Utility/UtilityPCH.h
	
# Compiles file ../../../../Source/Common/Utility/Debug.cpp for the Debug configuration...
-include ../IntermediateDir/Utility/gccDebug/Debug.d
../IntermediateDir/Utility/gccDebug/Debug.o: ../../../../Source/Common/Utility/Debug.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Utility/Debug.cpp $(Debug_Include_Path) -o ../IntermediateDir/Utility/gccDebug/Debug.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Utility/Debug.cpp $(Debug_Include_Path) > ../IntermediateDir/Utility/gccDebug/Debug.d

# Compiles file ../../../../Source/Common/Utility/FileSystemHelper.cpp for the Debug configuration...
-include ../IntermediateDir/Utility/gccDebug/FileSystemHelper.d
../IntermediateDir/Utility/gccDebug/FileSystemHelper.o: ../../../../Source/Common/Utility/FileSystemHelper.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Utility/FileSystemHelper.cpp $(Debug_Include_Path) -o ../IntermediateDir/Utility/gccDebug/FileSystemHelper.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Utility/FileSystemHelper.cpp $(Debug_Include_Path) > ../IntermediateDir/Utility/gccDebug/FileSystemHelper.d

# Compiles file ../../../../Source/Common/Utility/FrameRate.cpp for the Debug configuration...
-include ../IntermediateDir/Utility/gccDebug/FrameRate.d
../IntermediateDir/Utility/gccDebug/FrameRate.o: ../../../../Source/Common/Utility/FrameRate.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Utility/FrameRate.cpp $(Debug_Include_Path) -o ../IntermediateDir/Utility/gccDebug/FrameRate.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Utility/FrameRate.cpp $(Debug_Include_Path) > ../IntermediateDir/Utility/gccDebug/FrameRate.d

# Compiles file ../../../../Source/Common/Utility/pugixml.cpp for the Debug configuration...
-include ../IntermediateDir/Utility/gccDebug/pugixml.d
../IntermediateDir/Utility/gccDebug/pugixml.o: ../../../../Source/Common/Utility/pugixml.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Utility/pugixml.cpp $(Debug_Include_Path) -o ../IntermediateDir/Utility/gccDebug/pugixml.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Utility/pugixml.cpp $(Debug_Include_Path) > ../IntermediateDir/Utility/gccDebug/pugixml.d

# Compiles file ../../../../Source/Common/Utility/StringHelper.cpp for the Debug configuration...
-include ../IntermediateDir/Utility/gccDebug/StringHelper.d
../IntermediateDir/Utility/gccDebug/StringHelper.o: ../../../../Source/Common/Utility/StringHelper.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Utility/StringHelper.cpp $(Debug_Include_Path) -o ../IntermediateDir/Utility/gccDebug/StringHelper.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Utility/StringHelper.cpp $(Debug_Include_Path) > ../IntermediateDir/Utility/gccDebug/StringHelper.d

# Compiles file ../../../../Source/Common/Utility/tinystr.cpp for the Debug configuration...
-include ../IntermediateDir/Utility/gccDebug/tinystr.d
../IntermediateDir/Utility/gccDebug/tinystr.o: ../../../../Source/Common/Utility/tinystr.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Utility/tinystr.cpp $(Debug_Include_Path) -o ../IntermediateDir/Utility/gccDebug/tinystr.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Utility/tinystr.cpp $(Debug_Include_Path) > ../IntermediateDir/Utility/gccDebug/tinystr.d

# Compiles file ../../../../Source/Common/Utility/tinyxml.cpp for the Debug configuration...
-include ../IntermediateDir/Utility/gccDebug/tinyxml.d
../IntermediateDir/Utility/gccDebug/tinyxml.o: ../../../../Source/Common/Utility/tinyxml.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Utility/tinyxml.cpp $(Debug_Include_Path) -o ../IntermediateDir/Utility/gccDebug/tinyxml.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Utility/tinyxml.cpp $(Debug_Include_Path) > ../IntermediateDir/Utility/gccDebug/tinyxml.d

# Compiles file ../../../../Source/Common/Utility/tinyxmlerror.cpp for the Debug configuration...
-include ../IntermediateDir/Utility/gccDebug/tinyxmlerror.d
../IntermediateDir/Utility/gccDebug/tinyxmlerror.o: ../../../../Source/Common/Utility/tinyxmlerror.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Utility/tinyxmlerror.cpp $(Debug_Include_Path) -o ../IntermediateDir/Utility/gccDebug/tinyxmlerror.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Utility/tinyxmlerror.cpp $(Debug_Include_Path) > ../IntermediateDir/Utility/gccDebug/tinyxmlerror.d

# Compiles file ../../../../Source/Common/Utility/tinyxmlparser.cpp for the Debug configuration...
-include ../IntermediateDir/Utility/gccDebug/tinyxmlparser.d
../IntermediateDir/Utility/gccDebug/tinyxmlparser.o: ../../../../Source/Common/Utility/tinyxmlparser.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Utility/tinyxmlparser.cpp $(Debug_Include_Path) -o ../IntermediateDir/Utility/gccDebug/tinyxmlparser.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Utility/tinyxmlparser.cpp $(Debug_Include_Path) > ../IntermediateDir/Utility/gccDebug/tinyxmlparser.d

# Compiles file ../../../../Source/Common/Utility/UtilityPCH.cpp for the Debug configuration...
-include ../IntermediateDir/Utility/gccDebug/UtilityPCH.d
../IntermediateDir/Utility/gccDebug/UtilityPCH.o: ../../../../Source/Common/Utility/UtilityPCH.cpp
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c ../../../../Source/Common/Utility/UtilityPCH.cpp $(Debug_Include_Path) -o ../IntermediateDir/Utility/gccDebug/UtilityPCH.o
	$(CPP_COMPILER) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -MM ../../../../Source/Common/Utility/UtilityPCH.cpp $(Debug_Include_Path) > ../IntermediateDir/Utility/gccDebug/UtilityPCH.d

# Builds the Release configuration...
.PHONY: Release
Release: create_folders ../IntermediateDir/Utility/gccRelease/Debug.o ../IntermediateDir/Utility/gccRelease/FileSystemHelper.o ../IntermediateDir/Utility/gccRelease/FrameRate.o ../IntermediateDir/Utility/gccRelease/pugixml.o ../IntermediateDir/Utility/gccRelease/StringHelper.o ../IntermediateDir/Utility/gccRelease/tinystr.o ../IntermediateDir/Utility/gccRelease/tinyxml.o ../IntermediateDir/Utility/gccRelease/tinyxmlerror.o ../IntermediateDir/Utility/gccRelease/tinyxmlparser.o ../IntermediateDir/Utility/gccRelease/UtilityPCH.o 
	ar rcs ../../../../Library/Utility/gccLib/libUtility.a ../IntermediateDir/Utility/gccRelease/Debug.o ../IntermediateDir/Utility/gccRelease/FileSystemHelper.o ../IntermediateDir/Utility/gccRelease/FrameRate.o ../IntermediateDir/Utility/gccRelease/pugixml.o ../IntermediateDir/Utility/gccRelease/StringHelper.o ../IntermediateDir/Utility/gccRelease/tinystr.o ../IntermediateDir/Utility/gccRelease/tinyxml.o ../IntermediateDir/Utility/gccRelease/tinyxmlerror.o ../IntermediateDir/Utility/gccRelease/tinyxmlparser.o ../IntermediateDir/Utility/gccRelease/UtilityPCH.o  $(Release_Implicitly_Linked_Objects)

# Compiles file ../../../../Source/Common/Utility/Debug.cpp for the Release configuration...
-include ../IntermediateDir/Utility/gccRelease/Debug.d
../IntermediateDir/Utility/gccRelease/Debug.o: ../../../../Source/Common/Utility/Debug.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Utility/Debug.cpp $(Release_Include_Path) -o ../IntermediateDir/Utility/gccRelease/Debug.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Utility/Debug.cpp $(Release_Include_Path) > ../IntermediateDir/Utility/gccRelease/Debug.d

# Compiles file ../../../../Source/Common/Utility/FileSystemHelper.cpp for the Release configuration...
-include ../IntermediateDir/Utility/gccRelease/FileSystemHelper.d
../IntermediateDir/Utility/gccRelease/FileSystemHelper.o: ../../../../Source/Common/Utility/FileSystemHelper.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Utility/FileSystemHelper.cpp $(Release_Include_Path) -o ../IntermediateDir/Utility/gccRelease/FileSystemHelper.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Utility/FileSystemHelper.cpp $(Release_Include_Path) > ../IntermediateDir/Utility/gccRelease/FileSystemHelper.d

# Compiles file ../../../../Source/Common/Utility/FrameRate.cpp for the Release configuration...
-include ../IntermediateDir/Utility/gccRelease/FrameRate.d
../IntermediateDir/Utility/gccRelease/FrameRate.o: ../../../../Source/Common/Utility/FrameRate.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Utility/FrameRate.cpp $(Release_Include_Path) -o ../IntermediateDir/Utility/gccRelease/FrameRate.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Utility/FrameRate.cpp $(Release_Include_Path) > ../IntermediateDir/Utility/gccRelease/FrameRate.d

# Compiles file ../../../../Source/Common/Utility/pugixml.cpp for the Release configuration...
-include ../IntermediateDir/Utility/gccRelease/pugixml.d
../IntermediateDir/Utility/gccRelease/pugixml.o: ../../../../Source/Common/Utility/pugixml.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Utility/pugixml.cpp $(Release_Include_Path) -o ../IntermediateDir/Utility/gccRelease/pugixml.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Utility/pugixml.cpp $(Release_Include_Path) > ../IntermediateDir/Utility/gccRelease/pugixml.d

# Compiles file ../../../../Source/Common/Utility/StringHelper.cpp for the Release configuration...
-include ../IntermediateDir/Utility/gccRelease/StringHelper.d
../IntermediateDir/Utility/gccRelease/StringHelper.o: ../../../../Source/Common/Utility/StringHelper.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Utility/StringHelper.cpp $(Release_Include_Path) -o ../IntermediateDir/Utility/gccRelease/StringHelper.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Utility/StringHelper.cpp $(Release_Include_Path) > ../IntermediateDir/Utility/gccRelease/StringHelper.d

# Compiles file ../../../../Source/Common/Utility/tinystr.cpp for the Release configuration...
-include ../IntermediateDir/Utility/gccRelease/tinystr.d
../IntermediateDir/Utility/gccRelease/tinystr.o: ../../../../Source/Common/Utility/tinystr.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Utility/tinystr.cpp $(Release_Include_Path) -o ../IntermediateDir/Utility/gccRelease/tinystr.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Utility/tinystr.cpp $(Release_Include_Path) > ../IntermediateDir/Utility/gccRelease/tinystr.d

# Compiles file ../../../../Source/Common/Utility/tinyxml.cpp for the Release configuration...
-include ../IntermediateDir/Utility/gccRelease/tinyxml.d
../IntermediateDir/Utility/gccRelease/tinyxml.o: ../../../../Source/Common/Utility/tinyxml.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Utility/tinyxml.cpp $(Release_Include_Path) -o ../IntermediateDir/Utility/gccRelease/tinyxml.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Utility/tinyxml.cpp $(Release_Include_Path) > ../IntermediateDir/Utility/gccRelease/tinyxml.d

# Compiles file ../../../../Source/Common/Utility/tinyxmlerror.cpp for the Release configuration...
-include ../IntermediateDir/Utility/gccRelease/tinyxmlerror.d
../IntermediateDir/Utility/gccRelease/tinyxmlerror.o: ../../../../Source/Common/Utility/tinyxmlerror.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Utility/tinyxmlerror.cpp $(Release_Include_Path) -o ../IntermediateDir/Utility/gccRelease/tinyxmlerror.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Utility/tinyxmlerror.cpp $(Release_Include_Path) > ../IntermediateDir/Utility/gccRelease/tinyxmlerror.d

# Compiles file ../../../../Source/Common/Utility/tinyxmlparser.cpp for the Release configuration...
-include ../IntermediateDir/Utility/gccRelease/tinyxmlparser.d
../IntermediateDir/Utility/gccRelease/tinyxmlparser.o: ../../../../Source/Common/Utility/tinyxmlparser.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Utility/tinyxmlparser.cpp $(Release_Include_Path) -o ../IntermediateDir/Utility/gccRelease/tinyxmlparser.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Utility/tinyxmlparser.cpp $(Release_Include_Path) > ../IntermediateDir/Utility/gccRelease/tinyxmlparser.d

# Compiles file ../../../../Source/Common/Utility/UtilityPCH.cpp for the Release configuration...
-include ../IntermediateDir/Utility/gccRelease/UtilityPCH.d
../IntermediateDir/Utility/gccRelease/UtilityPCH.o: ../../../../Source/Common/Utility/UtilityPCH.cpp
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c ../../../../Source/Common/Utility/UtilityPCH.cpp $(Release_Include_Path) -o ../IntermediateDir/Utility/gccRelease/UtilityPCH.o
	$(CPP_COMPILER) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -MM ../../../../Source/Common/Utility/UtilityPCH.cpp $(Release_Include_Path) > ../IntermediateDir/Utility/gccRelease/UtilityPCH.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p ../IntermediateDir/Utility/gccDebug
	mkdir -p ../../../../Library/Utility/gccLib
	mkdir -p ../IntermediateDir/Utility/gccRelease
	mkdir -p ../../../../Library/Utility/gccLib

# Copy all header files to library include folder	
.PHONY: post_buid_event
post_build_event:
	mkdir -p ../../../../Library/Utility/Utility
	rm ../../../../Library/Utility/Utility/*.h
	cp ../../../../Source/Utility/*.h ../../../../Library/Utility/Utility/

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f ../IntermediateDir/Utility/gccDebug/*.o
	rm -f ../IntermediateDir/Utility/gccDebug/*.d
	rm -f ../../../../Library/Utility/gccLib/*.a
	rm -f ../../../../Library/Utility/gccLib/*.so
	rm -f ../../../../Library/Utility/gccLib/*.dll
	rm -f ../../../../Library/Utility/gccLib/*.exe
	rm -f ../IntermediateDir/Utility/gccRelease/*.o
	rm -f ../IntermediateDir/Utility/gccRelease/*.d
	rm -f ../../../../Library/Utility/gccLib/*.a
	rm -f ../../../../Library/Utility/gccLib/*.so
	rm -f ../../../../Library/Utility/gccLib/*.dll
	rm -f ../../../../Library/Utility/gccLib/*.exe

