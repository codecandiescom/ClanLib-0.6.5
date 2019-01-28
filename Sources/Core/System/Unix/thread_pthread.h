/*
	$Id: thread_pthread.h,v 1.4 2001/10/11 15:19:56 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_thread_pthread
#define header_thread_pthread

#include "API/Core/System/thread.h"

class CL_Thread_Generic
{
public:
	CL_Runnable *runnable;
	pthread_t thread;
	bool running;
	bool delete_runnable;
	int ref_count;

	static void *run_init(void *self);
};

#endif
