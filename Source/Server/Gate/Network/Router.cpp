#include "GatePCH.h"
#include "Router.h"
#include <boost/lexical_cast.hpp>
#include "Utility/SerializableStream.hpp"

Router::Router(Network::HttpNetwork& kHttpNetwork,
    Network::TcpNetwork& kTcpNetwork)
    : mkHttpNetwork(kHttpNetwork)
    , mkTcpNetwork(kTcpNetwork)
    , mkPacketAssembler(Utility::RECOMMENDED_MAX_PACKET_SIZE)
    , muiRequestIDSeed(0)
{

}
Router::~Router()
{

}
void Router::ClientToGame(Network::HttpRequestPtr& spRequest)
{
    BOOST_ASSERT(mkGameVector.size() > 0);
    RequestID requestID = GenerateRequestID();
    mkPacketAssembler.reset();
    mkPacketAssembler & requestID &
        spRequest->method &
        spRequest->uri &
        spRequest->content;
    BOOST_ASSERT(mkRequestCache.find(requestID) == mkRequestCache.end());
    mkRequestCache[requestID] = spRequest;
    mkTcpNetwork.send(mkGameVector[0], (unsigned char*)mkPacketAssembler.Data(), mkPacketAssembler.Size());
}
void Router::GameToClient(Network::ConnectionID uConID,
    const unsigned char* acData, unsigned int uSize)
{
    RequestID requestID;
    float requestPriority;
    std::string contentType;
    unsigned int uiStatusCode;
    std::string strContent;
    Utility::SerializeInStream kStream((char*)(acData + sizeof(unsigned int)), uSize - sizeof(unsigned int));
    try
    {
        kStream & requestID & requestPriority & contentType &
            uiStatusCode & strContent;
    }
    catch (...)
    {
        LOG_ERROR("error packet from game.");
        return;
    }

    RequestCache::iterator iter = mkRequestCache.find(requestID);
    if (iter == mkRequestCache.end())
    {
        LOG_ERROR("cannot found this request with id %llu from game server.", requestID);
        return;
    }

    Network::HttpRequestPtr spRequest = iter->second;
    Network::HttpResponePtr spRespone = spRequest->GenerateRespone();
    spRespone->setPriority(requestPriority);
    spRespone->status_code = uiStatusCode;
    spRespone->putHeaderField("Content-Type", contentType);
    spRespone->putHeaderField("Content-Length", boost::lexical_cast<std::string>(strContent.length()));
    spRespone->content = strContent;
    mkHttpNetwork.sendRespone(spRespone);
    mkRequestCache.erase(iter);
}