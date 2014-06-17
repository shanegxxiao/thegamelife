#include <list>
#include <vector>
#include <map>
#include <string>

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/assert.hpp>
#include <boost/program_options.hpp>
#include <boost/asio/ssl.hpp>

#include "ConnectionIDGenerator.h"
#include "TcpNetwork.h"
#include "DownloadMgr.h"
#include "DownloadTask.h"
#include "HttpDownloadTask.h"