#ifndef __NetworkService_H__
#define __NetworkService_H__

#include "Runtime/Object.h"
#include "Utility/SizeHeadedPacketAssembler.h"

class NetworkService : public Runtime::Object
{
public:
    NetworkService();
    virtual ~NetworkService();
    virtual bool Initialize(boost::property_tree::ptree& kParamList);
    virtual void Running();
    virtual bool Shutdown();

private:
    /// tcp network
    /// @{
    bool InitializeTcpNetwork(boost::property_tree::ptree& kParamList);
    void OnConFunc(Network::ConnectionID uConID);
    void OnDisConFunc(Network::ConnectionID uConID);
    bool OnPacketFunc(Network::ConnectionID uConID, const unsigned char* acData, unsigned int uSize);
    void OnSentFunc(Network::ConnectionID uConID, unsigned int uSize);
    void OnErrorFunc(Network::ConnectionID uConID, const boost::system::error_code& kErrorCode);
    /// @}

private:
    Network::TcpNetwork mkTcpNetwork;
    Utility::SizeHeadedPacketAssembler mkPacketAssembler;
};

#endif//__NetworkService_H__