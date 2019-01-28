/*
	$Id: mutex_win32.h,v 1.4 2001/05/07 08:36:59 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_mutex_win32
#define header_mutex_win32

#include "API/Core/System/mutex.h"
#include "API/Core/System/cl_assert.h"
#include <windows.h>

class CL_Mutex_Generic
{
public:
	CRITICAL_SECTION mutex;
	HANDLE event;
};

#endif