/*
	$Id: event_listener.h,v 1.17 2001/10/10 11:50:27 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanCore="System"
//! header=core.h

#ifndef header_event_listener
#define header_event_listener

class CL_EventTrigger;
class CL_EventListener_Generic;

//: Event Listener Class
//- <p>The event listener is used to collect a set of events, identified as CL_EventTrigger,
//- and wait for one of them to occur. This is analog to the lowlevel C socket select()
//- function, except that this one operates on clanlib objects.</p>
class CL_EventListener
{
//! Construction:
public:
	//: Constructs an empty event listener object.
	CL_EventListener();

	//: Constructs an event listener with one trigger in it.
	CL_EventListener(CL_EventTrigger *trigger);

	//: Event Listener Destructor
	virtual ~CL_EventListener();

//! Attributes:
public:

//! Operations:
public:
	//: Adds an event trigger to the listener.
	void add_trigger(CL_EventTrigger *trigger);

	//: Removes an event trigger from the listener.
	void remove_trigger(CL_EventTrigger *trigger);

	//: Removes all event triggers from the listener.
	void clear();

	//: Resets all event triggers added to the listener.
	void reset_all();

	//: Wait until one of the events triggers.
	//: The timeout is in milliseconds.
	//- If timeout = -1, this function will not timeout.
	//- Returns true if an event triggered, false if it timed out.
	bool wait(int timeout = -1);

//! Implementation:
private:
	CL_EventListener_Generic *impl;
};

#endif
