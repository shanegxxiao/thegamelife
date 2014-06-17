################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ActiveConnectionContainer.cpp \
../src/Connection.cpp \
../src/ConnectionIDGenerator.cpp \
../src/DownloadMgr.cpp \
../src/DownloadTask.cpp \
../src/HttpDownloadTask.cpp \
../src/IdealConnectionContainer.cpp \
../src/NetworkDefine.cpp \
../src/NetworkPCH.cpp \
../src/SSLConnection.cpp \
../src/TcpConnection.cpp \
../src/TcpNetwork.cpp 

OBJS += \
./src/ActiveConnectionContainer.o \
./src/Connection.o \
./src/ConnectionIDGenerator.o \
./src/DownloadMgr.o \
./src/DownloadTask.o \
./src/HttpDownloadTask.o \
./src/IdealConnectionContainer.o \
./src/NetworkDefine.o \
./src/NetworkPCH.o \
./src/SSLConnection.o \
./src/TcpConnection.o \
./src/TcpNetwork.o 

CPP_DEPS += \
./src/ActiveConnectionContainer.d \
./src/Connection.d \
./src/ConnectionIDGenerator.d \
./src/DownloadMgr.d \
./src/DownloadTask.d \
./src/HttpDownloadTask.d \
./src/IdealConnectionContainer.d \
./src/NetworkDefine.d \
./src/NetworkPCH.d \
./src/SSLConnection.d \
./src/TcpConnection.d \
./src/TcpNetwork.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../../../../Library/Boost -I../../../../../Library/OpenSSL -I../../../../../Library/Utility -I../../../../../Library/Runtime -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


