################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BoostLogTarget.cpp \
../src/ConsoleSystem.cpp \
../src/LogHtmlFile.cpp \
../src/LogService.cpp \
../src/LogSystem.cpp \
../src/LogTarget.cpp \
../src/LogToVSOutput.cpp \
../src/Object.cpp \
../src/ObjectCreator.cpp \
../src/ObjectSystem.cpp \
../src/RuntimePCH.cpp 

OBJS += \
./src/BoostLogTarget.o \
./src/ConsoleSystem.o \
./src/LogHtmlFile.o \
./src/LogService.o \
./src/LogSystem.o \
./src/LogTarget.o \
./src/LogToVSOutput.o \
./src/Object.o \
./src/ObjectCreator.o \
./src/ObjectSystem.o \
./src/RuntimePCH.o 

CPP_DEPS += \
./src/BoostLogTarget.d \
./src/ConsoleSystem.d \
./src/LogHtmlFile.d \
./src/LogService.d \
./src/LogSystem.d \
./src/LogTarget.d \
./src/LogToVSOutput.d \
./src/Object.d \
./src/ObjectCreator.d \
./src/ObjectSystem.d \
./src/RuntimePCH.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I../../../../../Library/Boost -I../../../../../Library/OpenSSL -I../../../../../Library/Utility -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


