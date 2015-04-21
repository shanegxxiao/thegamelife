#include "UnitTestPCH.h"
#include <boost/test/unit_test.hpp>
#include "Database/Database.h"

BOOST_AUTO_TEST_SUITE(HiRedisTestSuite)


class RedisClientCallBack
{
public:
    RedisClientCallBack()
        : mnConnectCnt(0)
        , mnCmdReplyCnt(0)
        , mnDisConnectCnt(0)
    {

    }
    void OnConnect(const redisAsyncContext *c, int status)
    {
        if (status != REDIS_OK)
        {
            printf("Error: %s\n", c->errstr);
            return;
        }
        printf("Connected!\n");
        ++mnConnectCnt;
    }
    void OnCommandReply(redisAsyncContext *c, void *reply, void *privdata)
    {
        redisReply *pReply = (redisReply*)reply;
        if (reply == NULL)
        {
            return;
        }
        ++mnCmdReplyCnt;
        printf("reply count: %d\n", mnCmdReplyCnt);
        printf("argv[%s]: %s\n", (char*)privdata, pReply->str);
    }
    void OnDisConnect(const redisAsyncContext *c, int status)
    {
        if (status != REDIS_OK)
        {
            printf("Error: %s\n", c->errstr);
            return;
        }
        printf("Disconnected!\n");
        ++mnDisConnectCnt;
    }

public:
    int mnConnectCnt;
    int mnCmdReplyCnt;
    int mnDisConnectCnt;
};

RedisClientCallBack kRedisClientCallBack;
Database::RedisClientMgr kRedisClientMgr;

BOOST_AUTO_TEST_CASE(RedisClientMgrTest)
{
    kRedisClientMgr.Initialize();
    kRedisClientMgr.BindOnCmdReplyFunc(boost::bind(&RedisClientCallBack::OnCommandReply, &kRedisClientCallBack, _1, _2, _3));
    kRedisClientMgr.BindOnConnectFunc(boost::bind(&RedisClientCallBack::OnConnect, &kRedisClientCallBack, _1, _2));
    kRedisClientMgr.BindOnDisConnectFunc(boost::bind(&RedisClientCallBack::OnDisConnect, &kRedisClientCallBack, _1, _2));
    
    int nCmdCnt = 30000;
    int nConnectCnt = 30;
    Database::RedisClientPtr spClient;
    for (int i = 0; i < nConnectCnt; ++i)
    {
         spClient = kRedisClientMgr.Connect("218.6.70.98", 6379);
    }

    for (int i = 0; i < nCmdCnt; ++i)
    {
        spClient->AsyncCommand((void*)"hello", "get hello");
    }
    
    bool bShuted = false;
    while (true)
    {
        kRedisClientMgr.Running();

        if (kRedisClientCallBack.mnCmdReplyCnt == nCmdCnt && !bShuted)
        {
            bShuted = true;
            kRedisClientMgr.Shutdown();
        }

        if (kRedisClientMgr.IsShutdown())
        {
            kRedisClientMgr.Cleanup();
            break;
        }
    }
    BOOST_CHECK_EQUAL(kRedisClientCallBack.mnConnectCnt, nConnectCnt);
    BOOST_CHECK_EQUAL(kRedisClientCallBack.mnCmdReplyCnt, nCmdCnt);
    BOOST_CHECK_EQUAL(kRedisClientCallBack.mnDisConnectCnt, nConnectCnt);
}

BOOST_AUTO_TEST_SUITE_END()
