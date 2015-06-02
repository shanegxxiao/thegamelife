#include "DatabasePCH.h"
#include "RedisClient.h"

#ifndef WIN32
#include <stdarg.h>
#include "RedisX64/ae.h"
#endif// WIN32

namespace Database
{
    aeEventLoop* RedisClient::mspaeEventLoop = 0;
    RedisClient::ConnectCallback RedisClient::mspfuncConnectCb;
    RedisClient::CommandReplyCallback RedisClient::mspfuncCmdCb;
    RedisClient::DisconnectCallback RedisClient::mspfuncDiscntCb;

    void RedisClient::SetEventLoop(aeEventLoop* eventLoop)
    {
        mspaeEventLoop = eventLoop;
    }
    void RedisClient::OnConnect(const redisAsyncContext *c, int status)
    {
        if (mspfuncConnectCb)
        {
            mspfuncConnectCb(c, status);
        }
    }
    void RedisClient::OnCommandReply(redisAsyncContext *c, void *reply, void *privdata)
    {
        if (mspfuncCmdCb)
        {
            mspfuncCmdCb(c, reply, privdata);
        }
    }
    void RedisClient::OnDisConnect(const redisAsyncContext *c, int status)
    {
        if (mspfuncDiscntCb)
        {
            mspfuncDiscntCb(c, status);
        }
    }
    void RedisClient::BindOnConnectFunc(RedisClient::ConnectCallback cb)
    {
        mspfuncConnectCb = cb;
    }
    void RedisClient::BindOnDisConnectFunc(RedisClient::DisconnectCallback cb)
    {
        mspfuncDiscntCb = cb;
    }
    void RedisClient::BindOnCmdReplyFunc(RedisClient::CommandReplyCallback cb)
    {
        mspfuncCmdCb = cb;
    }
    RedisClient::RedisClient()
        : mpRedisAsyncContext(0)
        , mbClosed(false)
        , muiPort(6379)
    {

    }
    RedisClient::~RedisClient()
    {

    }
    redisAsyncContext* RedisClient::GetRedisAsyncContext()
    {
        return mpRedisAsyncContext;
    }
    void RedisClient::AsyncCommand(void *privdata, const char *format, ...)
    {
        va_list args;
        va_start(args, format);
        redisAsyncCommand(mpRedisAsyncContext, OnCommandReply, 
            privdata, format, args);
		va_end(args);
    }
    bool RedisClient::Open(const std::string& strHost,
        unsigned int uiPort)
    {
        mstrHost = strHost;
        muiPort = uiPort;
        mpRedisAsyncContext = redisAsyncConnect(mstrHost.c_str(), muiPort);
        if (mpRedisAsyncContext->err)
        {
            //TODO: free mpRedisAsyncContext
            printf("Error: %s\n", mpRedisAsyncContext->errstr);
            return false;
        }
        redisAeAttach(mspaeEventLoop, mpRedisAsyncContext);
        redisAsyncSetConnectCallback(mpRedisAsyncContext, OnConnect);
        redisAsyncSetDisconnectCallback(mpRedisAsyncContext, OnDisConnect);
        return true;
    }
    void RedisClient::Close()
    {
        redisAsyncDisconnect(mpRedisAsyncContext);
    }
}
