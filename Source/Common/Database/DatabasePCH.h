// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef __DatabasePCH_h__
#define __DatabasePCH_h__

#include "targetver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <string>
#include <list>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

extern "C"
{
#include "hiredis/hiredis.h"
#include "hiredis/async.h"
#include "hiredis/adapters/ae.h"
}

/// Compile OTL 4.0/ODBC
#ifdef WIN32
#include <hash_map>
#define OTL_ODBC
#else
#include<ext/hash_map>
#define OTL_ODBC_UNIX
#endif // WIN32
/// The following #define is required with MyODBC 5.1 and higher
#define OTL_ODBC_SELECT_STM_EXECUTE_BEFORE_DESCRIBE
/// Compile OTL with Unicode 
#define OTL_UNICODE  
/// include the OTL 4.0 header file
#undef close
#include <Otl/otlv4.h>

#endif//__DatabasePCH_h__
