/*
	$Id: thread_beos.cpp,v 1.1 2000/07/18 17:04:27 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include "API/Core/System/cl_assert.h"
#include "thread_beos.h"

CL_Thread *CL_Thread::create(CL_Runnable *runnable)
{
	return new CL_Thread_BeOS(runnable);
}

CL_Thread_BeOS::CL_Thread_BeOS(CL_Runnable *runnable)
{
	this->runnable = runnable;
	running = false;
}

CL_Thread_BeOS::~CL_Thread_BeOS()
{
	terminate();
}

void CL_Thread_BeOS::start()
{
	if (running) return;

//	cl_assert(pthread_create(&thread, NULL, run_init, this)==0);

	running = true;
}

void *CL_Thread_BeOS::run_init(void *_self)
{
	CL_Thread_BeOS *self = (CL_Thread_BeOS *) _self;

	// kill thread immidiately - no cancelation point...
//	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	self->runnable->run();
	return NULL;
}

void CL_Thread_BeOS::terminate()
{
//	if (running) pthread_cancel(thread);
	running = false;
}

void CL_Thread_BeOS::wait()
{
//	if (running) pthread_join(thread, NULL);
	running = false;
}
