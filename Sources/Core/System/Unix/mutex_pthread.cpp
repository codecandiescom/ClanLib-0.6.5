/*
	$Id: mutex_pthread.cpp,v 1.5 2002/06/22 12:58:04 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Core/System/cl_assert.h"
#include <pthread.h>
#include "mutex_pthread.h"

// We need to do this because the posix threads library under linux obviously
// suck:
extern "C"
{
	int pthread_mutexattr_setkind_np(pthread_mutexattr_t *attr, int kind);
}

CL_Mutex *CL_Mutex::create()
{
	return new CL_Mutex;
}
////////////////////////////////////////////////////////////////////////////////
// CL_Mutex Posix implementation

CL_Mutex::CL_Mutex()
: impl(new CL_Mutex_Generic)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
#ifndef linux
	pthread_mutexattr_setkind_np(&attr, PTHREAD_MUTEX_RECURSIVE);
#else
	pthread_mutexattr_setkind_np(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
#endif
	pthread_mutex_init(&impl->mutex, &attr);
	pthread_mutexattr_destroy(&attr);

	pthread_cond_init(&impl->cond,0);
}

CL_Mutex::CL_Mutex(const CL_Mutex &copy)
{
	cl_assert(false);
}

void CL_Mutex::operator=(const CL_Mutex &copy)
{
	cl_assert(false);
}

CL_Mutex::~CL_Mutex()
{
	pthread_mutex_destroy(&impl->mutex);
	pthread_cond_destroy(&impl->cond);
	delete impl;
}
	
void CL_Mutex::enter()
{
	pthread_mutex_lock(&impl->mutex);
}

void CL_Mutex::leave()
{
	pthread_mutex_unlock(&impl->mutex);
}


void CL_Mutex::wait()
{
	pthread_cond_wait(&impl->cond,&impl->mutex);
}


void CL_Mutex::notify()
{
	pthread_cond_signal(&impl->cond);
}

void CL_Mutex::notify_all()
{
	pthread_cond_broadcast(&impl->cond);
}
