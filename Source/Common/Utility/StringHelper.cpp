#include "UtilityPCH.h"
#include "StringHelper.h"
#include <string.h>

namespace Utility
{
	char* Stristr( char* str1, const char* str2 )
	{
		if( !str1 || !str2 )
			return 0;

		// Slow but at least we have it.

		unsigned int str2len = strlen( str2 );
		while( *str1 )
		{
#ifdef WIN32
			if(_strnicmp( str1, str2, str2len ) == 0)
#else
			if (strcasecmp(str1, str2) == 0)
#endif // WIN32
			{
				return str1;
			}
			++ str1;
		}

		return 0;
	}

}
