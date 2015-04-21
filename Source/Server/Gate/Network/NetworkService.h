#ifndef __NetworkService_H__
#define __NetworkService_H__

#include "Runtime/Object.h"
#include "Network/Network.h"
#include "Router.h"

class NetworkService : public Runtime::Object
{
public:
    NetworkService();
    virtual ~NetworkService();
    virtual bool Initialize(boost::property_tree::ptree& kParamList);
    virtual void Running();
    virtual bool Shutdown();

private:
    /// http network
    /// @{
    bool InitializeHttpNetwork(boost::property_tree::ptree& kParamList);
    void HttpRequestHandler(Network::HttpRequestPtr spRequest);
    void HttpResponeHandler(Network::HttpResponePtr spRespone);
    void HttpClientErrorHandler(Network::HttpConnectionPtr spConnection);
    void HttpServerErrorHandler(Network::HttpConnectionPtr spConnection);
    /// @}

    /// tcp network
    /// @{
    bool InitializeTcpNetwork(boost::property_tree::ptree& kParamList);
    void OnConFunc(Network::ConnectionID uConID);
    void OnDisConFunc(Network::ConnectionID uConID);
    bool OnPacketFunc(Network::ConnectionID uConID, const unsigned char* acData, unsigned int uSize);
    void OnSentFunc(Network::ConnectionID uConID, unsigned int uSize);
    void OnErrorFunc(Network::ConnectionID uConID, const boost::system::error_code& kErrorCode);
    /// @}

    /// Router
    /// @{
    void ForwardToGame(Network::HttpRequestPtr& spRequest);
    void ReplyClient(Network::ConnectionID uConID,
        const unsigned char* acData, unsigned int uSize);
    /// @}

private:
    Network::HttpNetwork mkHttpNetwork;
    Network::TcpNetwork mkTcpNetwork;

    Network::ConnectionID mGameConID;
    Router mkRouter;
};

#endif//__NetworkService_H__