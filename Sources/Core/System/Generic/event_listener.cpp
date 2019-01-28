/*
	$Id: event_listener.cpp,v 1.8 2001/09/08 19:12:44 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Core/System/event_listener.h"
#include "event_listener_generic.h"

#ifdef WIN32
#include "Core/System/Win32/event_listener_win32.h"
#else
#include "Core/System/Unix/event_listener_unix.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener construction:

CL_EventListener::CL_EventListener()
: impl(NULL)
{
#ifdef WIN32
	impl = new CL_EventListener_Win32(this);
#else
	impl = new CL_EventListener_Unix(this);
#endif
}

CL_EventListener::CL_EventListener(CL_EventTrigger *trigger)
: impl(NULL)
{
#ifdef WIN32
	impl = new CL_EventListener_Win32(this);
#else
	impl = new CL_EventListener_Unix(this);
#endif

	add_trigger(trigger);
}

CL_EventListener::~CL_EventListener()
{
	if (impl) delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener operations:

void CL_EventListener::add_trigger(CL_EventTrigger *trigger)
{
	if (impl) impl->add_trigger(trigger);
}

void CL_EventListener::remove_trigger(CL_EventTrigger *trigger)
{
	if (impl) impl->remove_trigger(trigger);
}

void CL_EventListener::clear()
{
	if (impl) impl->clear();
}

void CL_EventListener::reset_all()
{
	if (impl) impl->reset_all();
}

bool CL_EventListener::wait(int timeout)
{
	if (impl) return impl->wait(timeout);
	else return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener implementation:
