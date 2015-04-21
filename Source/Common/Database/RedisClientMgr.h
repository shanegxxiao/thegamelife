#ifndef __RedisService_h__
#define __RedisService_h__

#include "RedisClient.h"

namespace Database
{
    class RedisClientMgr
    {
    private:
        typedef std::list<RedisClientPtr> RedisClientList;

    public:
        RedisClientMgr();
        ~RedisClientMgr();

        bool Initialize(int setsize = 1024 * 10);
        void Running();
        bool Shutdown();
        bool IsShutdown();
        void Cleanup();

        RedisClientPtr Connect(const std::string& strHost,
            const unsigned int port);
        int CloseConnection(RedisClientPtr spClient);

        void BindOnConnectFunc(RedisClient::ConnectCallback cb);
        void BindOnDisConnectFunc(RedisClient::DisconnectCallback cb);
        void BindOnCmdReplyFunc(RedisClient::CommandReplyCallback cb);

    private:
        void OnConnect(const redisAsyncContext *c, int status);
        void OnCommandReply(redisAsyncContext *c, void *reply, void *privdata);
        void OnDisConnect(const redisAsyncContext *c, int status);

    private:
        bool mbShutdownCalled;
        bool mbShuted;
        aeEventLoop* mpaeEventLoop;
        RedisClient::ConnectCallback mpfuncConnectCb;
        RedisClient::CommandReplyCallback mpfuncCmdCb;
        RedisClient::DisconnectCallback mpfuncDiscntCb;
        RedisClientList mkClientList;
    };

    inline void RedisClientMgr::BindOnConnectFunc(RedisClient::ConnectCallback cb)
    {
        mpfuncConnectCb = cb;
    }
    inline void RedisClientMgr::BindOnDisConnectFunc(RedisClient::DisconnectCallback cb)
    {
        mpfuncDiscntCb = cb;
    }
    inline void RedisClientMgr::BindOnCmdReplyFunc(RedisClient::CommandReplyCallback cb)
    {
        mpfuncCmdCb = cb;
    }
    inline bool RedisClientMgr::IsShutdown()
    {
        return mbShuted;
    }
}

#endif//__RedisService_h__