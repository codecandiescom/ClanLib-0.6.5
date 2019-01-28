/*
	$Id: efence.h,v 1.4 2001/09/06 18:52:04 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

	Electric fence, C++ enable code.

	If you need to test for buffer overruns, add -lefence to your linking
	and include this header in one of your sourcefiles.
*/

#ifndef header_efence
#define header_efence

#ifndef WIN32

#include <stdlib.h>

void *operator new(size_t size)
{
	return malloc(size);
}

void operator delete(void *ptr)
{
	free(ptr);
}

void operator delete[](void *ptr)
{
	free(ptr);
}

#endif

#endif
