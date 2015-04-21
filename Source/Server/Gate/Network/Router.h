#ifndef __Router_h__
#define __Router_h__

#include "Network/HttpNetwork.h"
#include "Network/Network.h"
#include "Utility/SizeHeadedPacketAssembler.h"

class Router
{
    typedef Utility::RequestID RequestID;
    typedef std::vector<Network::ConnectionID> GameVector;
    typedef std::map<RequestID, Network::HttpRequestPtr> RequestCache;

public:
    Router(Network::HttpNetwork& kHttpNetwork,
        Network::TcpNetwork& kTcpNetwork);
    ~Router();

    void AddGame(Network::ConnectionID id);

    void ClientToGame(Network::HttpRequestPtr& spRequest);
    void GameToClient(Network::ConnectionID uConID,
        const unsigned char* acData, unsigned int uSize);

private:
    RequestID GenerateRequestID();

private:
    RequestID muiRequestIDSeed;
    GameVector mkGameVector;
    Network::HttpNetwork& mkHttpNetwork;
    Network::TcpNetwork& mkTcpNetwork;
    Utility::SizeHeadedPacketAssembler mkPacketAssembler;
    RequestCache mkRequestCache;
};

inline void Router::AddGame(Network::ConnectionID id)
{
    mkGameVector.push_back(id);
}
inline Router::RequestID Router::GenerateRequestID()
{
    return ++muiRequestIDSeed;
}

#endif//__Router_h__