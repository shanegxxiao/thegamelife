#include "GatePCH.h"
#include "NetworkService.h"
#include "Runtime/LogService.h"

RUNTIME_CREATE_SERVICE(NetworkService, Runtime::Object, 0)
SERVICE_INIT_AFTER(NetworkService, LogService)
SERVICE_SHUT_BEFORE(NetworkService, LogService)

NetworkService::NetworkService()
: mGameConID(Network::INVALID_CONNECTION_ID)
, mkRouter(mkHttpNetwork, mkTcpNetwork)
{

}
NetworkService::~NetworkService()
{

}
bool NetworkService::Initialize(boost::property_tree::ptree& kParamList)
{
    if (!InitializeHttpNetwork(kParamList))
    {
        return false;
    }
    if (!InitializeTcpNetwork(kParamList))
    {
        return false;
    }
    EnableAutoRunning(Runtime::RTP_NORMAL);
    return true;
}
void NetworkService::Running()
{
    mkHttpNetwork.running();
    mkTcpNetwork.running();
}
bool NetworkService::Shutdown()
{
    mkHttpNetwork.shutdown();
    mkTcpNetwork.shutdown();
    return true;
}
bool NetworkService::InitializeHttpNetwork(boost::property_tree::ptree& kParamList)
{
    if (!mkHttpNetwork.startup())
    {
        return false;
    }
    if (!mkHttpNetwork.listen(
        kParamList.get<std::string>("Gate.HttpNetwork.HttpListenIP", "0.0.0.0").c_str(),
        kParamList.get<unsigned int>("Gate.HttpNetwork.HttpListenPort", 8000)))
    {
        return false;
    }

    mkHttpNetwork.bindRequestHandler(boost::bind(&NetworkService::HttpRequestHandler,
        this, _1));
    mkHttpNetwork.bindResponeHandler(boost::bind(&NetworkService::HttpResponeHandler,
        this, _1));
    mkHttpNetwork.bindClientErrorHandler(boost::bind(&NetworkService::HttpClientErrorHandler,
        this, _1));
    mkHttpNetwork.bindServerErrorHandler(boost::bind(&NetworkService::HttpServerErrorHandler,
        this, _1));
    return true;
}
void NetworkService::HttpRequestHandler(Network::HttpRequestPtr spRequest)
{
    LOG_TRACE("%s : %s", spRequest->method.c_str(), spRequest->uri.c_str());
    LOG_TRACE(spRequest->http_version.c_str());
    for (Network::Headers::iterator kIter = spRequest->headers.begin();
        kIter != spRequest->headers.end(); ++kIter)
    {
        LOG_TRACE("%s : %s", kIter->first.c_str(), kIter->second.c_str());
    }
    LOG_TRACE(spRequest->content.c_str());

    spRequest->setNeedWaittingRespone();

    ForwardToGame(spRequest);
}
void NetworkService::HttpResponeHandler(Network::HttpResponePtr spRespone)
{
}
void NetworkService::HttpClientErrorHandler(Network::HttpConnectionPtr spConnection)
{
}
void NetworkService::HttpServerErrorHandler(Network::HttpConnectionPtr spConnection)
{
    ///TODO: handle server side error occurred while receiving request or sending respond
    LOG_ERROR("error occurred while receiving request or sending respond.");
    LOG_ERROR("%d:%s", spConnection->getLastError().value(),
        spConnection->getLastError().message().c_str());
    LOG_ERROR(spConnection->getRespondString().c_str());
}
bool NetworkService::InitializeTcpNetwork(boost::property_tree::ptree& kParamList)
{
    Network::TcpNetworkParams kParams = Network::TcpNetworkParams::Default();
    kParams.uiSendBuffSize = kParamList.get<unsigned int>("Gate.TcpClient.SendBuffSize", 8 * 1024 * 1024);
    kParams.uiRecvBuffSize = kParamList.get<unsigned int>("Gate.TcpClient.RecvBuffSize", 8 * 1024 * 1024);
    kParams.uiIOThreadCount = kParamList.get<unsigned int>("Gate.TcpClient.IOThreadCount", 0);
    kParams.uiListenConnectionNum = kParamList.get<unsigned int>("Gate.TcpClient.ListenConnectionNum", 0);
    kParams.uiMaxConnectionWriteSize = kParamList.get<unsigned int>("Gate.TcpClient.MaxConnectionWriteSize", 8 * 1024);
    kParams.uiExtraBufferCount = kParamList.get<unsigned int>("Gate.TcpClient.ExtraBufferCount", 0);
    kParams.uiPreCreateConCnt = kParamList.get<unsigned int>("Gate.TcpClient.PreCreateConCnt", 1);
    kParams.uiMaxPoolConCnt = kParamList.get<unsigned int>("Gate.TcpClient.MaxPoolConCnt", 10);
    kParams.bCloseOnBufferOverflow = kParamList.get<bool>("Gate.TcpClient.CloseOnBufferOverflow", false);
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

    if (Network::INVALID_CONNECTION_ID == mkTcpNetwork.asyncConnect(
        kParamList.get<std::string>("Gate.GameIP", "127.0.0.1").c_str(),
        kParamList.get<unsigned int>("Gate.GamePort", 9000)))
    {
        return false;
    }

    return true;
}
void NetworkService::OnConFunc(Network::ConnectionID uConID)
{
    LOG_TRACE("connected to game!");
    mkRouter.AddGame(uConID);
}
void NetworkService::OnDisConFunc(Network::ConnectionID uConID)
{
    /// TODO: if it is unexpected disconnection, reconnect it.
    LOG_ERROR("disconnect from a game.");
}
bool NetworkService::OnPacketFunc(Network::ConnectionID uConID,
    const unsigned char* acData, unsigned int uSize)
{
    ReplyClient(uConID, acData, uSize);
    return true;
}
void NetworkService::OnSentFunc(Network::ConnectionID uConID, unsigned int uSize)
{

}
void NetworkService::OnErrorFunc(Network::ConnectionID uConID,
    const boost::system::error_code& kErrorCode)
{
    /// TODO: if it is unexpected error, reconnect it.
    LOG_ERROR("error occurred from a connection with game.");
}
void NetworkService::ForwardToGame(Network::HttpRequestPtr& spRequest)
{
    mkRouter.ClientToGame(spRequest);
}
void NetworkService::ReplyClient(Network::ConnectionID uConID,
    const unsigned char* acData, unsigned int uSize)
{
    mkRouter.GameToClient(uConID, acData, uSize);
}
