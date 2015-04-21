#include "GamePCH.h"
#include "NetworkService.h"
#include "Runtime/LogService.h"
#include "Utility/Typedef.h"
#include "Utility/SerializableStream.hpp"
#include <boost/property_tree/json_parser.hpp>

RUNTIME_CREATE_SERVICE(NetworkService, Runtime::Object, 0)
SERVICE_INIT_AFTER(NetworkService, LogService)
SERVICE_SHUT_BEFORE(NetworkService, LogService)

NetworkService::NetworkService()
: mkPacketAssembler(Utility::RECOMMENDED_MAX_PACKET_SIZE)
{

}
NetworkService::~NetworkService()
{

}
bool NetworkService::Initialize(boost::property_tree::ptree& kParamList)
{
    if (!InitializeTcpNetwork(kParamList))
    {
        return false;
    }
    EnableAutoRunning(Runtime::RTP_NORMAL);
    return true;
}
void NetworkService::Running()
{
    mkTcpNetwork.running();
}
bool NetworkService::Shutdown()
{
    mkTcpNetwork.shutdown();
    return true;
}
bool NetworkService::InitializeTcpNetwork(boost::property_tree::ptree& kParamList)
{
    Network::TcpNetworkParams kParams = Network::TcpNetworkParams::Default();
    kParams.uiSendBuffSize = kParamList.get<unsigned int>("Game.TcpServer.SendBuffSize", 8 * 1024 * 1024);
    kParams.uiRecvBuffSize = kParamList.get<unsigned int>("Game.TcpServer.RecvBuffSize", 8 * 1024 * 1024);
    kParams.uiIOThreadCount = kParamList.get<unsigned int>("Game.TcpServer.IOThreadCount", 0);
    kParams.uiListenConnectionNum = kParamList.get<unsigned int>("Game.TcpServer.ListenConnectionNum", 5);
    kParams.uiMaxConnectionWriteSize = kParamList.get<unsigned int>("Game.TcpServer.MaxConnectionWriteSize", 8 * 1024);
    kParams.uiExtraBufferCount = kParamList.get<unsigned int>("Game.TcpServer.ExtraBufferCount", 0);
    kParams.uiPreCreateConCnt = kParamList.get<unsigned int>("Game.TcpServer.PreCreateConCnt", 1);
    kParams.uiMaxPoolConCnt = kParamList.get<unsigned int>("Game.TcpServer.MaxPoolConCnt", 10);
    kParams.bCloseOnBufferOverflow = kParamList.get<bool>("Game.TcpServer.CloseOnBufferOverflow", false);
    kParams.eDataProcessMethod = Network::DPM_Packet;

    mkTcpNetwork.bindOnConnected(boost::bind(&NetworkService::OnConFunc, this, _1));
    mkTcpNetwork.bindOnDisconnected(boost::bind(&NetworkService::OnDisConFunc, this, _1));
    mkTcpNetwork.bindOnPacket(boost::bind(&NetworkService::OnPacketFunc, this, _1, _2, _3));
    mkTcpNetwork.bindOnSent(boost::bind(&NetworkService::OnSentFunc, this, _1, _2));
    mkTcpNetwork.bindOnError(boost::bind(&NetworkService::OnErrorFunc, this, _1, _2));

    if (!mkTcpNetwork.startup(kParams))
    {
        return false;
    }

    if (!mkTcpNetwork.listen(kParamList.get<std::string>("Game.TcpServer.ListenIP", "0.0.0.0").c_str(),
        kParamList.get<unsigned int>("Game.TcpServer.ListenPort", 9000)))
    {
        return false;
    }

    return true;
}
void NetworkService::OnConFunc(Network::ConnectionID uConID)
{
    LOG_TRACE("gate connected!");
}
void NetworkService::OnDisConFunc(Network::ConnectionID uConID)
{
    LOG_TRACE("gate disconnected!");
}
bool NetworkService::OnPacketFunc(Network::ConnectionID uConID,
    const unsigned char* acData, unsigned int uSize)
{
    Utility::RequestID requestID;
    std::string method;
    std::string uri;
    std::string content;

    Utility::SerializeInStream kStream(Utility::PacketDataPtr(acData), Utility::PacketDataSize(uSize));
    try
    {
        kStream & requestID & method & uri & content;
    }
    catch (...)
    {
        LOG_ERROR("error packet from game.");
        return true;
    }

    LOG_TRACE("receive a packet:");
    LOG_TRACE("request id: %llu", requestID);
    LOG_TRACE("method: %s", method.c_str());
    LOG_TRACE("uri: %s", uri.c_str());
    LOG_TRACE("content: %s", content.c_str());

    try
    {
        boost::property_tree::ptree json;
        std::stringstream ss;
        json.add("method", method);
        json.add("uri", uri);
        json.add("content", content);
        boost::property_tree::write_json(ss, json, false);

        float requestPriority = 1.f;
        std::string contentType = "text/x-json";
        unsigned int uiStatusCode = 200;
        std::string strContent = ss.str();
        mkPacketAssembler.reset();
        mkPacketAssembler & requestID & requestPriority & contentType &
            uiStatusCode & strContent;
        mkTcpNetwork.send(uConID, (unsigned char*)mkPacketAssembler.Data(), mkPacketAssembler.Size());
    }
    catch (...)
    {
        LOG_ERROR("error packet from game.");
        return true;
    }
    return true;
}
void NetworkService::OnSentFunc(Network::ConnectionID uConID, unsigned int uSize)
{

}
void NetworkService::OnErrorFunc(Network::ConnectionID uConID,
    const boost::system::error_code& kErrorCode)
{
    LOG_TRACE("gate connection error!");
}
