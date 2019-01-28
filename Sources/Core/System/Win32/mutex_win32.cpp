/*
	$Id: mutex_win32.cpp,v 1.3 2001/05/07 08:36:59 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include "mutex_win32.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Mutex 0.4 backward compatibility

CL_Mutex *CL_Mutex::create()
{
	return new CL_Mutex;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Mutex win32 implementation

CL_Mutex::CL_Mutex()
: impl(new CL_Mutex_Generic)
{
	InitializeCriticalSection(&impl->mutex);
	impl->event = CreateEvent(NULL, FALSE, FALSE, NULL);
}

CL_Mutex::CL_Mutex(const CL_Mutex &copy)
{
	cl_assert(false);
}

void CL_Mutex::operator =(const CL_Mutex &copy)
{
	cl_assert(false);
}

CL_Mutex::~CL_Mutex()
{
	DeleteCriticalSection(&impl->mutex);
	CloseHandle(impl->event);
	delete impl;
}

void CL_Mutex::enter()
{
	EnterCriticalSection(&impl->mutex);
}

void CL_Mutex::leave()
{
	LeaveCriticalSection(&impl->mutex);
}

void CL_Mutex::wait() 
{ 
	WaitForSingleObject(impl->event, INFINITE);
}

void CL_Mutex::notify() 
{ 
	SetEvent(impl->event);
}

void CL_Mutex::notify_all() 
{ 
	cl_assert(false); 
}
