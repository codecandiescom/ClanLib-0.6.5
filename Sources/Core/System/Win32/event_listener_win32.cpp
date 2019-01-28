/*
	$Id: event_listener_win32.cpp,v 1.5 2001/09/08 19:12:45 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "event_listener_win32.h"
#include "event_trigger_win32.h"
#include "API/Core/System/system.h"

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener_Win32 construction:

CL_EventListener_Win32::CL_EventListener_Win32(CL_EventListener *owner)
:
	CL_EventListener_Generic(owner)
{
}

CL_EventListener_Win32::~CL_EventListener_Win32()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener_Win32 implementation:

bool CL_EventListener_Win32::wait(int timeout)
{
	if (triggers.empty()) // no triggers, just do the sleep.
	{
		CL_System::sleep(timeout);
		return false;
	}

	int num_handles = triggers.size();
	HANDLE *handles = new HANDLE[num_handles];

	int i = 0;
	std::list<CL_EventTrigger_Generic *>::iterator it;
	for (it = triggers.begin(); it != triggers.end(); it++)
	{
		CL_EventTrigger_Win32 *trigger = static_cast<CL_EventTrigger_Win32*>(*it);
		handles[i++] = trigger->wait_handle;
	}

	DWORD result = WaitForMultipleObjects(
		num_handles,
		handles,
		FALSE,
		timeout == -1 ? INFINITE : timeout);

	delete[] handles;

	if (result == WAIT_TIMEOUT) return false;
	return true;
}
