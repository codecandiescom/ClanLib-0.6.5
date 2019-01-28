/*
	$Id: event_listener_generic.cpp,v 1.7 2001/09/08 19:12:44 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "event_listener_generic.h"
#include "event_trigger_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener_Generic construction:

CL_EventListener_Generic::CL_EventListener_Generic(CL_EventListener *owner)
: owner(owner)
{
}

CL_EventListener_Generic::~CL_EventListener_Generic()
{
	clear();
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener_Generic operations:

void CL_EventListener_Generic::add_trigger(const CL_EventTrigger *trigger)
{
	CL_EventTrigger_Generic *trigger_impl = trigger->impl;
	triggers.push_back(trigger_impl);
	trigger_impl->add_ref();
}

void CL_EventListener_Generic::remove_trigger(const CL_EventTrigger *trigger)
{
	CL_EventTrigger_Generic *trigger_impl = trigger->impl;
	triggers.remove(trigger_impl);
	trigger_impl->release_ref();
}

void CL_EventListener_Generic::clear()
{
	std::list<CL_EventTrigger_Generic *>::iterator it;
	for (it = triggers.begin(); it != triggers.end(); it++) (*it)->release_ref();

	triggers = std::list<CL_EventTrigger_Generic *>();
}

void CL_EventListener_Generic::reset_all()
{
	std::list<CL_EventTrigger_Generic *>::iterator it;
	for (it = triggers.begin(); it != triggers.end(); it++) (*it)->reset();
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener_Generic implementation:
