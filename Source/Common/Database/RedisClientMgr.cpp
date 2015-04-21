#include "DatabasePCH.h"
#include "RedisClientMgr.h"

namespace Database
{
    RedisClientMgr::RedisClientMgr()
        : mpaeEventLoop(0)
        , mbShutdownCalled(false)
        , mbShuted(false)
    {

    }
    RedisClientMgr::~RedisClientMgr()
    {

    }
    bool RedisClientMgr::Initialize(int setsize/* = 1024 * 10*/)
    {
#ifdef _WIN32
        /* For Win32_IOCP the event loop must be created before the async connect */
        mpaeEventLoop = aeCreateEventLoop(setsize);
#else
        signal(SIGPIPE, SIG_IGN);
        mpaeEventLoop = aeCreateEventLoop(setsize);
#endif
        mpaeEventLoop->stop = 0;
        mbShutdownCalled = false;
        mbShuted = false;
        RedisClient::BindOnConnectFunc(boost::bind(&RedisClientMgr::OnConnect, this, _1, _2));
        RedisClient::BindOnCmdReplyFunc(boost::bind(&RedisClientMgr::OnCommandReply, this, _1, _2, _3));
        RedisClient::BindOnDisConnectFunc(boost::bind(&RedisClientMgr::OnDisConnect, this, _1, _2));

        return true;
    }
    void RedisClientMgr::Running()
    {
        if (!mpaeEventLoop->stop)
        {
            if (mpaeEventLoop->beforesleep != NULL)
            {
                mpaeEventLoop->beforesleep(mpaeEventLoop);
            }
            aeProcessEvents(mpaeEventLoop, AE_ALL_EVENTS);
        }
        for (RedisClientList::iterator iter = mkClientList.begin();
            iter != mkClientList.end(); ++iter)
        {
            if ((*iter)->IsClosed())
            {
                iter = mkClientList.erase(iter);
                if (iter == mkClientList.end())
                {
                    break;
                }
            }
        }
        if (mbShutdownCalled && mkClientList.size() == 0)
        {
            aeStop(mpaeEventLoop);
            mbShuted = true;
        }
    }
    bool RedisClientMgr::Shutdown()
    {
        for (RedisClientList::iterator iter = mkClientList.begin();
            iter != mkClientList.end(); ++iter)
        {
            if (*iter)
            {
                (*iter)->Close();
            }
        }
        mbShutdownCalled = true;
        return true;
    }
    void RedisClientMgr::Cleanup()
    {
        aeDeleteEventLoop(mpaeEventLoop);
    }
    RedisClientPtr RedisClientMgr::Connect(const std::string& strHost,
        const unsigned int port)
    {
        RedisClientPtr redisClient(new RedisClient());
        redisClient->SetEventLoop(mpaeEventLoop);
        if (!redisClient->Open(strHost, port))
        {
            return RedisClientPtr();
        }
        mkClientList.push_back(redisClient);
        return redisClient;
    }
    int RedisClientMgr::CloseConnection(RedisClientPtr spClient)
    {
        int nResult = 0;
        for (RedisClientList::iterator iter = mkClientList.begin();
            iter != mkClientList.end(); ++iter)
        {
            if (*iter == spClient)
            {
                (*iter)->Close();
                ++nResult;
            }
        }
        return nResult;
    }
    void RedisClientMgr::OnConnect(const redisAsyncContext *c, int status)
    {
        if (mpfuncConnectCb)
        {
            mpfuncConnectCb(c, status);
        }
    }
    void RedisClientMgr::OnCommandReply(redisAsyncContext *c, void *reply, void *privdata)
    {
        if (mpfuncCmdCb)
        {
            mpfuncCmdCb(c, reply, privdata);
        }
    }
    void RedisClientMgr::OnDisConnect(const redisAsyncContext *c, int status)
    {
        if (mpfuncDiscntCb)
        {
            mpfuncDiscntCb(c, status);
        }
        for (RedisClientList::iterator iter = mkClientList.begin();
            iter != mkClientList.end(); ++iter)
        {
            if ((*iter)->GetRedisAsyncContext() == c)
            {
                (*iter)->MarkClosed();
            }
        }
    }
}