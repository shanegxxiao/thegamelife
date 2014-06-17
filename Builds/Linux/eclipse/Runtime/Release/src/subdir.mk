################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ConsoleSystem.cpp \
../src/LogFile.cpp \
../src/LogService.cpp \
../src/LogSystem.cpp \
../src/LogTarget.cpp \
../src/LogToVSOutput.cpp \
../src/Object.cpp \
../src/ObjectCreator.cpp \
../src/ObjectSystem.cpp \
../src/Param.cpp \
../src/ParamList.cpp \
../src/RefObject.cpp \
../src/RuntimePCH.cpp 

OBJS += \
./src/ConsoleSystem.o \
./src/LogFile.o \
./src/LogService.o \
./src/LogSystem.o \
./src/LogTarget.o \
./src/LogToVSOutput.o \
./src/Object.o \
./src/ObjectCreator.o \
./src/ObjectSystem.o \
./src/Param.o \
./src/ParamList.o \
./src/RefObject.o \
./src/RuntimePCH.o 

CPP_DEPS += \
./src/ConsoleSystem.d \
./src/LogFile.d \
./src/LogService.d \
./src/LogSystem.d \
./src/LogTarget.d \
./src/LogToVSOutput.d \
./src/Object.d \
./src/ObjectCreator.d \
./src/ObjectSystem.d \
./src/Param.d \
./src/ParamList.d \
./src/RefObject.d \
./src/RuntimePCH.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../../../../Library/Boost -I../../../../../Library/OpenSSL -I../../../../../Library/Utility -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


