/*
	$Id: event_listener_generic.h,v 1.8 2001/09/08 19:12:44 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_event_listener_generic
#define header_event_listener_generic

#include "API/Core/System/event_listener.h"
#include "API/Core/System/event_trigger.h"

class CL_EventTrigger_Generic;

class CL_EventListener_Generic
{
// Construction:
public:
	CL_EventListener_Generic(CL_EventListener *owner);

	virtual ~CL_EventListener_Generic();

// Attributes:
public:

// Operations:
public:
	void add_trigger(const CL_EventTrigger *trigger);
	// Adds an event trigger to the listener.

	void remove_trigger(const CL_EventTrigger *trigger);
	// Removes an event trigger from the listener.

	void clear();
	// Removes all event triggers from the listener.

	void reset_all();
	// Resets all event triggers added to the listener.

	virtual bool wait(int timeout)=0;
	// Wait until one of the events triggers. The timeout is in milliseconds.
	// Returns true if a event triggered, false if it timed out.

	std::list<CL_EventTrigger_Generic *> triggers;
// Implementation:
private:
	CL_EventListener *owner;
};

#endif
