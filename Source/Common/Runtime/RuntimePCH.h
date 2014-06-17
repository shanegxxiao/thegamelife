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
#include <boost/date_time/posix_time/posix_time.hpp>

#ifdef WIN32

#include <boost/detail/interlocked.hpp>

#else

#ifndef BOOST_INTERLOCKED_INCREMENT
#define BOOST_INTERLOCKED_INCREMENT(longPtr) (++(*longPtr))
#endif

#ifndef BOOST_INTERLOCKED_DECREMENT
#define BOOST_INTERLOCKED_DECREMENT(longPtr) (--(*longPtr))
#endif

#endif //WIN32

#include "Utility/Utility.h"
#include "DynCreate.h"

EXTERN_RUNTIME_EXPORTED_MEMBER;

#endif//__RuntimePCH_h__
