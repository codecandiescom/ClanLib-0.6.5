/*
	$Id: event_trigger_win32.cpp,v 1.8 2001/12/17 20:11:40 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "event_trigger_win32.h"

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Win32 construction:

CL_EventTrigger_Win32::CL_EventTrigger_Win32()
:
	wait_handle(NULL)
{
	wait_handle = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CL_EventTrigger_Win32::~CL_EventTrigger_Win32()
{
	CloseHandle(wait_handle);
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Win32 attributes:

bool CL_EventTrigger_Win32::get_flag() const
{
	return WaitForSingleObject(wait_handle, 0) != WAIT_TIMEOUT;
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Win32 operations:

void CL_EventTrigger_Win32::reset() const
{
	ResetEvent(wait_handle);
}

void CL_EventTrigger_Win32::set_flag()
{
	SetEvent(wait_handle);
}

bool CL_EventTrigger_Win32::wait(int timeout) const
{
	DWORD result = WaitForSingleObject(
		wait_handle,
		(timeout == -1) ? INFINITE : timeout);

	return (result != WAIT_TIMEOUT);
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Win32 implementation:
