################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Debug.cpp \
../src/Encode.cpp \
../src/FileSystemHelper.cpp \
../src/FrameRate.cpp \
../src/SizeHeadedPacketAssembler.cpp \
../src/StringHelper.cpp \
../src/Typedef.cpp \
../src/UtilityPCH.cpp \
../src/pugixml.cpp \
../src/tinystr.cpp \
../src/tinyxml.cpp \
../src/tinyxmlerror.cpp \
../src/tinyxmlparser.cpp 

OBJS += \
./src/Debug.o \
./src/Encode.o \
./src/FileSystemHelper.o \
./src/FrameRate.o \
./src/SizeHeadedPacketAssembler.o \
./src/StringHelper.o \
./src/Typedef.o \
./src/UtilityPCH.o \
./src/pugixml.o \
./src/tinystr.o \
./src/tinyxml.o \
./src/tinyxmlerror.o \
./src/tinyxmlparser.o 

CPP_DEPS += \
./src/Debug.d \
./src/Encode.d \
./src/FileSystemHelper.d \
./src/FrameRate.d \
./src/SizeHeadedPacketAssembler.d \
./src/StringHelper.d \
./src/Typedef.d \
./src/UtilityPCH.d \
./src/pugixml.d \
./src/tinystr.d \
./src/tinyxml.d \
./src/tinyxmlerror.d \
./src/tinyxmlparser.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I../../../../../Library/Boost -I../../../../../Library/OpenSSL -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


