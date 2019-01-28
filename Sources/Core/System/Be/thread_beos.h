/*
	$Id: thread_beos.h,v 1.1 2000/07/18 17:04:27 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_thread_beos
#define header_thread_beos

#include "API/Core/System/thread.h"

class CL_Thread_BeOS : public CL_Thread
{
public:
	CL_Thread_BeOS(CL_Runnable *runnable);
	virtual ~CL_Thread_BeOS();

	virtual void start();
	virtual void terminate();
	virtual void wait();

private:
	CL_Runnable *runnable;
	bool running;

//	pthread_t thread;

	static void *run_init(void *self);
};

#endif
