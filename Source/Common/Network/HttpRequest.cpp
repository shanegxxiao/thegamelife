#include "NetworkPCH.h"
#include "HttpRequest.h"
#include <ostream>
#include <string>
#include "HttpRespone.h"
#include "HttpConnection.h"

namespace Network
{
	HttpRequest::HttpRequest()
		: priority(0.f)
		, userData(0)
		, mbUseSSL(false)
	{
		http_version = HttpMiscStrings::http;
		http_version += HttpMiscStrings::version;
	}
	HttpRequest::~HttpRequest()
	{

	}
	std::string HttpRequest::toString()
	{
		mstrHttpRequest.consume(mstrHttpRequest.size());
		std::ostream request_stream(&mstrHttpRequest);
		request_stream << method << HttpMiscStrings::space;
		request_stream << uri << HttpMiscStrings::space;
		request_stream << http_version << HttpMiscStrings::crlf;
		for (Headers::iterator kIter = headers.begin();
			kIter != headers.end(); ++kIter)
		{
			request_stream << kIter->first;
			request_stream << HttpMiscStrings::name_value_separator;
			request_stream << kIter->second;
			request_stream << HttpMiscStrings::crlf;
		}
		request_stream << HttpMiscStrings::crlf;
		request_stream << content;

		return std::string(boost::asio::buffers_begin(mstrHttpRequest.data()),
			boost::asio::buffers_begin(mstrHttpRequest.data()) + mstrHttpRequest.size());
	}
	void HttpRequest::putHeaderField(std::string strName, std::string strValue)
	{
		headers[strName] = strValue;
	}
	unsigned int HttpRequest::getContentLength()
	{
		Headers::iterator kIter = headers.find("Content-Length");
		if (kIter == headers.end())
		{
			return 0;
		}
		return boost::lexical_cast<unsigned int>(kIter->second);
	}
	HttpResponePtr HttpRequest::GenerateRespone()
	{
		HttpResponePtr spRespone;
		spRespone.reset(new HttpRespone);
		spRespone->originalRequestPtr = shared_from_this();
		mspServerConnection->mspRespone = spRespone;
		return spRespone;
	}
	void HttpRequest::setNeedWaittingRespone()
	{
		if (mspServerConnection)
		{
			mspServerConnection->setStatus(HttpConnection::waiting_response);
		}
	}
}