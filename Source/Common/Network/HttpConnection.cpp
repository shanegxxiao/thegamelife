#include "NetworkPCH.h"
#include "HttpConnection.h"

namespace Network
{
	HttpConnection::HttpConnection(boost::asio::io_service& io_service,
		HttpNetworkParams& kParam)
		: mkResolver(io_service)
		, muiStatus(Status::closed)
		, mkNetworkParams(kParam)
	{
	}
	HttpConnection::~HttpConnection()
	{
	}
	void HttpConnection::setStatus(unsigned int uiStatus)
	{
		if (uiStatus == Status::network_timeout)
		{
			muiStatus |= uiStatus;
			return;
		}
		if ((muiStatus & Status::network_timeout) > 0)
		{
			clearStatus();
			muiStatus |= uiStatus;
			muiStatus |= Status::network_timeout;
		}
		else if ((muiStatus == HttpConnection::pending_response ||
			muiStatus == waiting_response) &&
			uiStatus == HttpConnection::request_processed)
		{
			muiStatus |= uiStatus;
		}
		else
		{
			muiStatus = uiStatus;
		}
	}
	void HttpConnection::reset()
	{
		mspRequest.reset();
		mspRespone.reset();
		mstrRequest.consume(mstrRequest.size());
		mstrResponse.consume(mstrResponse.size());
		mkLastErrorCode.clear();
		setStatus(Status::closed);
	}
}