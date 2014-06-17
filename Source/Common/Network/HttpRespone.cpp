#include "NetworkPCH.h"
#include "HttpRespone.h"
#include <ostream>
#include "HttpConnection.h"

namespace Network
{
	namespace status_type
	{
		const unsigned int ok = 200;
		const unsigned int created = 201;
		const unsigned int accepted = 202;
		const unsigned int no_content = 204;
		const unsigned int multiple_choices = 300;
		const unsigned int moved_permanently = 301;
		const unsigned int moved_temporarily = 302;
		const unsigned int not_modified = 304;
		const unsigned int bad_request = 400;
		const unsigned int unauthorized = 401;
		const unsigned int forbidden = 403;
		const unsigned int not_found = 404;
		const unsigned int internal_server_error = 500;
		const unsigned int not_implemented = 501;
		const unsigned int bad_gateway = 502;
		const unsigned int service_unavailable = 503;
	};

	namespace status_strings
	{
		const std::string ok =
			"200 OK\r\n";
		const std::string created =
			"201 Created\r\n";
		const std::string accepted =
			"202 Accepted\r\n";
		const std::string no_content =
			"204 No Content\r\n";
		const std::string multiple_choices =
			"300 Multiple Choices\r\n";
		const std::string moved_permanently =
			"301 Moved Permanently\r\n";
		const std::string moved_temporarily =
			"302 Moved Temporarily\r\n";
		const std::string not_modified =
			"304 Not Modified\r\n";
		const std::string bad_request =
			"400 Bad Request\r\n";
		const std::string unauthorized =
			"401 Unauthorized\r\n";
		const std::string forbidden =
			"403 Forbidden\r\n";
		const std::string not_found =
			"404 Not Found\r\n";
		const std::string internal_server_error =
			"500 Internal Server Error\r\n";
		const std::string not_implemented =
			"501 Not Implemented\r\n";
		const std::string bad_gateway =
			"502 Bad Gateway\r\n";
		const std::string service_unavailable =
			"503 Service Unavailable\r\n";

		std::string to_string(std::string strHttpVersion, unsigned int status)
		{
			switch (status)
			{
			case status_type::ok:
				return std::string(strHttpVersion + " " + ok);
			case status_type::created:
				return std::string(strHttpVersion + " " + created);
			case status_type::accepted:
				return std::string(strHttpVersion + " " + accepted);
			case status_type::no_content:
				return std::string(strHttpVersion + " " + no_content);
			case status_type::multiple_choices:
				return std::string(strHttpVersion + " " + multiple_choices);
			case status_type::moved_permanently:
				return std::string(strHttpVersion + " " + moved_permanently);
			case status_type::moved_temporarily:
				return std::string(strHttpVersion + " " + moved_temporarily);
			case status_type::not_modified:
				return std::string(strHttpVersion + " " + not_modified);
			case status_type::bad_request:
				return std::string(strHttpVersion + " " + bad_request);
			case status_type::unauthorized:
				return std::string(strHttpVersion + " " + unauthorized);
			case status_type::forbidden:
				return std::string(strHttpVersion + " " + forbidden);
			case status_type::not_found:
				return std::string(strHttpVersion + " " + not_found);
			case status_type::internal_server_error:
				return std::string(strHttpVersion + " " + internal_server_error);
			case status_type::not_implemented:
				return std::string(strHttpVersion + " " + not_implemented);
			case status_type::bad_gateway:
				return std::string(strHttpVersion + " " + bad_gateway);
			case status_type::service_unavailable:
				return std::string(strHttpVersion + " " + service_unavailable);
			default:
				return std::string(strHttpVersion + " " + internal_server_error);
			}
		}

	} // namespace status_strings
	HttpRespone::HttpRespone()
	{
		http_version = Network::HttpMiscStrings::http;
		http_version += "1.1";
	}
	HttpRespone::~HttpRespone()
	{

	}
	std::string HttpRespone::toString()
	{
		mstrReply.consume(mstrReply.size());
		std::ostream respone_stream(&mstrReply);
		respone_stream << status_strings::to_string(http_version, status_code);
		for (Headers::iterator kIter = headers.begin();
			kIter != headers.end(); ++kIter)
		{
			respone_stream << kIter->first;
			respone_stream << HttpMiscStrings::name_value_separator;
			respone_stream << kIter->second;
			respone_stream << HttpMiscStrings::crlf;
		}
		respone_stream << HttpMiscStrings::crlf;
		respone_stream << content;

		return std::string(boost::asio::buffers_begin(mstrReply.data()),
			boost::asio::buffers_begin(mstrReply.data()) + mstrReply.size());
	}
	unsigned int HttpRespone::getContentLength()
	{
		Headers::iterator kIter = headers.find("Content-Length");
		if (kIter == headers.end())
		{
			return 0;
		}
		return boost::lexical_cast<unsigned int>(kIter->second);
	}
	void HttpRespone::send()
	{
		HttpRequestPtr spRequest = getRequest();
		HttpConnectionPtr spConnection = spRequest->getServerConnection();
		BOOST_ASSERT(spConnection);
		spConnection->sendRespone(shared_from_this());
	}
}