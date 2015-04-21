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
../src/HttpClientConnection.cpp \
../src/HttpConnection.cpp \
../src/HttpDownloadTask.cpp \
../src/HttpNetwork.cpp \
../src/HttpNetworkErrorCode.cpp \
../src/HttpNetworkParams.cpp \
../src/HttpRequest.cpp \
../src/HttpRespone.cpp \
../src/HttpServerConnection.cpp \
../src/HttpsClientConnection.cpp \
../src/HttpsServerConnection.cpp \
../src/IdealConnectionContainer.cpp \
../src/NetworkCommonDefine.cpp \
../src/NetworkPCH.cpp \
../src/SSLConnection.cpp \
../src/TcpConnection.cpp \
../src/TcpNetwork.cpp \
../src/TcpNetworkErrorCode.cpp \
../src/TcpNetworkParams.cpp 

OBJS += \
./src/ActiveConnectionContainer.o \
./src/Connection.o \
./src/ConnectionIDGenerator.o \
./src/DownloadMgr.o \
./src/DownloadTask.o \
./src/HttpClientConnection.o \
./src/HttpConnection.o \
./src/HttpDownloadTask.o \
./src/HttpNetwork.o \
./src/HttpNetworkErrorCode.o \
./src/HttpNetworkParams.o \
./src/HttpRequest.o \
./src/HttpRespone.o \
./src/HttpServerConnection.o \
./src/HttpsClientConnection.o \
./src/HttpsServerConnection.o \
./src/IdealConnectionContainer.o \
./src/NetworkCommonDefine.o \
./src/NetworkPCH.o \
./src/SSLConnection.o \
./src/TcpConnection.o \
./src/TcpNetwork.o \
./src/TcpNetworkErrorCode.o \
./src/TcpNetworkParams.o 

CPP_DEPS += \
./src/ActiveConnectionContainer.d \
./src/Connection.d \
./src/ConnectionIDGenerator.d \
./src/DownloadMgr.d \
./src/DownloadTask.d \
./src/HttpClientConnection.d \
./src/HttpConnection.d \
./src/HttpDownloadTask.d \
./src/HttpNetwork.d \
./src/HttpNetworkErrorCode.d \
./src/HttpNetworkParams.d \
./src/HttpRequest.d \
./src/HttpRespone.d \
./src/HttpServerConnection.d \
./src/HttpsClientConnection.d \
./src/HttpsServerConnection.d \
./src/IdealConnectionContainer.d \
./src/NetworkCommonDefine.d \
./src/NetworkPCH.d \
./src/SSLConnection.d \
./src/TcpConnection.d \
./src/TcpNetwork.d \
./src/TcpNetworkErrorCode.d \
./src/TcpNetworkParams.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I../../../../../Library/Boost -I../../../../../Library/OpenSSL -I../../../../../Library/Utility -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


