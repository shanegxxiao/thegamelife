################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Connection.cpp \
../src/DatabasePCH.cpp \
../src/DatabaseService.cpp \
../src/DirectExecuteTask.cpp \
../src/Task.cpp 

OBJS += \
./src/Connection.o \
./src/DatabasePCH.o \
./src/DatabaseService.o \
./src/DirectExecuteTask.o \
./src/Task.o 

CPP_DEPS += \
./src/Connection.d \
./src/DatabasePCH.d \
./src/DatabaseService.d \
./src/DirectExecuteTask.d \
./src/Task.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../../../../Library/Boost -I../../../../../Library/Otl -I../../../../../Library/OpenSSL -I../../../../../Library/Runtime -I../../../../../Library/Utility -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


