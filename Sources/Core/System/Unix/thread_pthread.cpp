/*
	$Id: thread_pthread.cpp,v 1.6 2001/12/19 19:23:03 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <pthread.h>
#include "API/Core/System/cl_assert.h"
#include "thread_pthread.h"

CL_Thread *CL_Thread::create(CL_Runnable *runnable, bool delete_runnable)
{
	return new CL_Thread(runnable, delete_runnable);
}

CL_Thread::CL_Thread(CL_Runnable *runnable, bool delete_runnable)
: impl(new CL_Thread_Generic)
{
	impl->ref_count = 1;
	impl->runnable = runnable;
	impl->delete_runnable = delete_runnable;
	impl->running = false;
}

CL_Thread::CL_Thread(const CL_Thread &copy)
: impl(copy.impl)
{
	if (impl) impl->ref_count++;
}

CL_Thread::CL_Thread()
: impl(0)
{
}

CL_Thread::~CL_Thread()
{
	if (impl)
	{
		impl->ref_count--;
		if (impl->ref_count == 0)
		{
			terminate();
			if (impl->delete_runnable) delete impl->runnable;
			delete impl;
		}
	}
}

CL_Thread &CL_Thread::operator =(const CL_Thread &copy)
{
	if (impl)
	{
		impl->ref_count--;
		if (impl->ref_count == 0)
		{
			terminate();
			if (impl->delete_runnable) delete impl->runnable;
			delete impl;
		}
	}

	impl = copy.impl;
	if (impl) impl->ref_count++;
	return *this;
}

void CL_Thread::start()
{
	if (impl->running) return;

	cl_assert(
		pthread_create(
			&impl->thread,
			NULL,
			impl->run_init,
			impl)==0);

	impl->running = true;
}

void *CL_Thread_Generic::run_init(void *_self)
{
	CL_Thread_Generic *self = (CL_Thread_Generic *) _self;

	// kill thread immidiately - no cancelation point...
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	self->runnable->run();
	return NULL;
}

void CL_Thread::terminate()
{
	if (impl->running) pthread_cancel(impl->thread);
	impl->running = false;
}

void CL_Thread::wait()
{
	if (impl->running) pthread_join(impl->thread, NULL);
	impl->running = false;
}

void CL_Thread::set_priority(EThreadPriority priority)
{
	static bool warning = true;

	if (warning)
	{
		cl_info(info_network, "CL_Thread_Posix::set_priority not implemented yet.");
		warning = false;
	}
}
