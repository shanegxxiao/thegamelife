################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DatabasePCH.cpp \
../src/OtlConnection.cpp \
../src/OtlConnectionMgr.cpp \
../src/OtlDirectExecuteTask.cpp \
../src/OtlTask.cpp \
../src/RedisClient.cpp \
../src/RedisClientMgr.cpp 

OBJS += \
./src/DatabasePCH.o \
./src/OtlConnection.o \
./src/OtlConnectionMgr.o \
./src/OtlDirectExecuteTask.o \
./src/OtlTask.o \
./src/RedisClient.o \
./src/RedisClientMgr.o 

CPP_DEPS += \
./src/DatabasePCH.d \
./src/OtlConnection.d \
./src/OtlConnectionMgr.d \
./src/OtlDirectExecuteTask.d \
./src/OtlTask.d \
./src/RedisClient.d \
./src/RedisClientMgr.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I../../../../../Library/Boost -I../../../../../Library/OpenSSL -I../../../../../Library/hiredis -I../../../../../Library/Otl -I../../../../../Library/Utility -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


