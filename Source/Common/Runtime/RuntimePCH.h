// RuntimePCH.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#ifndef __RuntimePCH_h__
#define __RuntimePCH_h__

#include "targetver.h"

#include <string>
#include <map>
#include <vector>
#include <list>
#include <queue>
#include <fstream>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include <boost/assert.hpp>
#include <boost/locale.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "Utility/Utility.h"
#include "DynCreate.h"

EXTERN_RUNTIME_EXPORTED_MEMBER;

#endif//__RuntimePCH_h__
