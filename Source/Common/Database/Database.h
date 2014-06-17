#ifndef __Database_h__
#define __Database_h__

#include <string>
#include <hash_map>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

/// Compile OTL 4.0/ODBC
#define OTL_ODBC 
/// The following #define is required with MyODBC 5.1 and higher
#define OTL_ODBC_SELECT_STM_EXECUTE_BEFORE_DESCRIBE
/// Compile OTL with Unicode 
#define OTL_UNICODE  
/// include the OTL 4.0 header file
#include <otl/otlv4.h>

#include "Connection.h"
#include "Task.h"
#include "DirectExecuteTask.h"
#include "DatabaseService.h"

#endif//__Database_h__