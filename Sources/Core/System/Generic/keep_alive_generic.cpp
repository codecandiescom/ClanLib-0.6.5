/*
	$Id: keep_alive_generic.cpp,v 1.5 2002/01/06 13:27:49 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <list>

#include "API/Core/System/keep_alive.h"
#include "API/Core/System/system.h"
#include "API/Core/System/event_listener.h"

std::list<CL_KeepAlive*> keep_alives;

CL_KeepAlive::CL_KeepAlive()
{
	keep_alives.push_back(this);
}

CL_KeepAlive::~CL_KeepAlive()
{
	keep_alives.remove(this);
}

void CL_System::keep_alive()
{
	for (
		std::list<CL_KeepAlive*>::iterator it = keep_alives.begin();
		it != keep_alives.end();
		it++)
	{
		(*it)->keep_alive();
	}
}

void CL_System::keep_alive(int millis)
{
	CL_EventListener events;
	keep_alive(events, millis);
}

bool CL_System::keep_alive(CL_EventListener &events, int timeout)
{
	// TODO: Add triggers for keep alive objects.

	if (timeout == -1)
	{
		do CL_System::keep_alive(); while (!events.wait(10));

		return true;
	}
	else
	{
		while (timeout > 0)
		{
			CL_System::keep_alive();
			if (events.wait(timeout > 10 ? 10 : timeout)) return true;
			timeout -= 10;
		}

		return false;
	}
}

CL_Signal_v0 &CL_System::sig_quit()
{
	static CL_Signal_v0 signal_quit;
	return signal_quit;
}

