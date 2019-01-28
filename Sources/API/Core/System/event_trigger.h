/*
	$Id: event_trigger.h,v 1.14 2001/12/05 23:50:51 mbn Exp $

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

#ifndef header_event_trigger
#define header_event_trigger

class CL_EventListener;
class CL_EventTrigger_Generic;

//: ClanLib Event trigger.
//- This class represents an event that can be triggered.
//- <p>Collect several triggers together using the CL_EventListener class, and then
//- call the CL_EventListener::wait function to sleep until one of the events
//- are triggered.</p>
class CL_EventTrigger
{
//! Construction:
public:
	//: Constructs an event trigger object.
	CL_EventTrigger();

	//: Copy constructor.
	CL_EventTrigger(const CL_EventTrigger &copy);

	//: Event Trigger Destructor
	virtual ~CL_EventTrigger();

//! Attributes:
public:
	//: Returns true if the trigger flag has been raised.
	bool get_flag() const;

//! Operations:
public:
	// Assignment operator.
	CL_EventTrigger &operator =(const CL_EventTrigger &copy);

	//: Reset trigger flag.
	void reset() const;

	//: Raise trigger flag.
	void set_flag();

	//: Wait until one of the event triggers.
	//: The timeout is in milliseconds.
	//- If timeout = -1, this function will not timeout.
	//- Returns true if the event triggered, false if it timed out.
	bool wait(int timeout = -1);

//! Implementation:
public:
	CL_EventTrigger_Generic *impl;
};

#endif
