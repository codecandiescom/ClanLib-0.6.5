/*
	$Id: event_listener_unix.cpp,v 1.8 2001/12/22 11:27:28 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "event_listener_unix.h"
#include "event_trigger_unix.h"
#include "API/Core/System/system.h"

// note: this cannot be replaced by <ctime>! (timeval needs to be defined)
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener_Unix construction:

CL_EventListener_Unix::CL_EventListener_Unix(CL_EventListener *owner)
:
	CL_EventListener_Generic(owner)
{
}

CL_EventListener_Unix::~CL_EventListener_Unix()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener_Unix implementation:

bool CL_EventListener_Unix::wait(int timeout)
{
	if (triggers.empty()) // no triggers, just do the sleep.
	{
		CL_System::sleep(timeout);
		return false;
	}

	fd_set rfds;
	FD_ZERO(&rfds);
	int highest_fd = -1;

	std::list<CL_EventTrigger_Generic *>::iterator it;
	for (it = triggers.begin(); it != triggers.end(); it++)
	{
		CL_EventTrigger_Unix *trigger = static_cast<CL_EventTrigger_Unix*>(*it);
		
		if (trigger->wait_pipe[0] > highest_fd) highest_fd = trigger->wait_pipe[0];
		FD_SET(trigger->wait_pipe[0], &rfds);
	}
	
	timeval tv;
	tv.tv_sec = timeout/1000;
	tv.tv_usec = (timeout%1000)*1000;

	int result = select(
		highest_fd+1,
		&rfds,
		NULL,
		NULL,
		(timeout == -1) ? NULL : &tv);

	if (result <= 0) return false;
	return true;
}
