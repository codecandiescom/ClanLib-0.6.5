/*
	$Id: thread_win32.cpp,v 1.8 2001/12/19 19:23:03 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include "API/Core/System/thread.h"
#include "API/Core/System/error.h"
#include "thread_win32.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Thread 0.4 backward compatibility:

CL_Thread *CL_Thread::create(CL_Runnable *runnable, bool delete_runnable)
{
	return new CL_Thread(runnable, delete_runnable);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Thread win32 implementation:

CL_Thread::CL_Thread()
: impl(0)
{
}

CL_Thread::CL_Thread(CL_Runnable *runnable, bool delete_runnable)
: impl(new CL_Thread_Generic)
{
	impl->runnable = runnable;
	impl->delete_runnable = delete_runnable;
	impl->thread_handle = NULL;
	impl->ref_count = 1;
}

CL_Thread::CL_Thread(const CL_Thread &copy)
: impl(copy.impl)
{
	if (impl) impl->ref_count++;
}

CL_Thread::~CL_Thread()
{
	if (impl)
	{
		impl->ref_count--;
		if (impl->ref_count == 0)
		{
			if (impl->thread_handle != NULL) terminate();
			if (impl->delete_runnable) delete impl->runnable;
			delete impl;
		}
	}
}

unsigned long __stdcall CL_Thread_Generic::func_proxy(void *arg)
{
	CL_Thread_Generic *self = (CL_Thread_Generic *) arg;
	self->runnable->run();

	return 0;
}

CL_Thread &CL_Thread::operator =(const CL_Thread &copy)
{
	if (impl)
	{
		impl->ref_count--;
		if (impl->ref_count == 0)
		{
			if (impl->thread_handle != NULL) terminate();
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
	impl->thread_handle = CreateThread(
		NULL,
		0,
		impl->func_proxy,
		impl,
		0,
		&impl->thread_id);

	if (impl->thread_handle == NULL)
		throw CL_Error("Failed to create thread");
}

void CL_Thread::terminate()
{
	TerminateThread(impl->thread_handle, 0);
	CloseHandle(impl->thread_handle);
	impl->thread_handle = NULL;
}

void CL_Thread::wait()
{
	if (impl->thread_handle == NULL) return;

	WaitForSingleObject(impl->thread_handle, INFINITE);
	CloseHandle(impl->thread_handle);
	impl->thread_handle = NULL;
}

void CL_Thread::set_priority(EThreadPriority priority)
{
	if (impl->thread_handle == NULL) return;

	int prio = THREAD_PRIORITY_NORMAL;
	switch (priority)
	{
	case cl_priority_above_normal:  prio = THREAD_PRIORITY_ABOVE_NORMAL;  break;
	case cl_priority_below_normal:  prio = THREAD_PRIORITY_BELOW_NORMAL;  break;
	case cl_priority_highest:       prio = THREAD_PRIORITY_HIGHEST;       break;
	case cl_priority_idle:          prio = THREAD_PRIORITY_IDLE;          break;
	case cl_priority_lowest:        prio = THREAD_PRIORITY_LOWEST;        break;
	case cl_priority_normal:        prio = THREAD_PRIORITY_NORMAL;        break;
	case cl_priority_time_critical: prio = THREAD_PRIORITY_TIME_CRITICAL; break;
	}

	BOOL result = SetThreadPriority(impl->thread_handle, prio);
	if (result == FALSE) throw CL_Error("Failed to set thread priority");
}
