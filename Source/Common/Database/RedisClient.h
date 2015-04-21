#ifndef __RedisClient_h__
#define __RedisClient_h__

namespace Database
{
    class RedisClient : public boost::enable_shared_from_this<RedisClient>
    {
    public:
        friend class RedisClientMgr;
        typedef boost::function<void(const redisAsyncContext *c, int status)> ConnectCallback;
        typedef boost::function<void(redisAsyncContext *c, void *reply, void *privdata)> CommandReplyCallback;
        typedef boost::function<void(const redisAsyncContext *c, int status)> DisconnectCallback;

    public:
        redisAsyncContext* GetRedisAsyncContext();
        void AsyncCommand(void *privdata, const char *format, ...);
        ~RedisClient();

    private:
        RedisClient();
        
        bool Open(const std::string& strHost,
            unsigned int uiPort);
        void Close();
        void MarkClosed();
        bool IsClosed();
        static void SetEventLoop(aeEventLoop* eventLoop);
        static void OnConnect(const redisAsyncContext *c, int status);
        static void OnCommandReply(redisAsyncContext *c, void *reply, void *privdata);
        static void OnDisConnect(const redisAsyncContext *c, int status);
        static void BindOnConnectFunc(RedisClient::ConnectCallback cb);
        static void BindOnDisConnectFunc(RedisClient::DisconnectCallback cb);
        static void BindOnCmdReplyFunc(RedisClient::CommandReplyCallback cb);

    private:
        std::string mstrHost;
        unsigned int muiPort;
        std::string mstrPassword;
        redisAsyncContext* mpRedisAsyncContext;
        bool mbClosed;
       
        static aeEventLoop* mspaeEventLoop;
        static ConnectCallback mspfuncConnectCb;
        static CommandReplyCallback mspfuncCmdCb;
        static DisconnectCallback mspfuncDiscntCb;
    };

    typedef boost::shared_ptr<RedisClient> RedisClientPtr;

    inline void RedisClient::MarkClosed()
    {
        mbClosed = true;
    }
    inline bool RedisClient::IsClosed()
    {
        return mbClosed;
    }
}

#endif//__RedisClient_h__