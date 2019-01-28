/*
	$Id: thread_win32.h,v 1.5 2001/12/19 19:23:03 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_thread_win32
#define header_thread_win32

#include "API/Core/System/thread.h"
#include <windows.h>

class CL_Thread_Generic
{
//!Attributes:
public:
	static unsigned long __stdcall func_proxy(void *arg);

	HANDLE thread_handle;

	DWORD thread_id;
	
	CL_Runnable *runnable;
	
	bool delete_runnable;
	
	int ref_count;
};

#endif
