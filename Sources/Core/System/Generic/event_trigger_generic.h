/*
	$Id: event_trigger_generic.h,v 1.7 2001/09/08 19:12:44 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_event_trigger_generic
#define header_event_trigger_generic

class CL_EventListener;

class CL_EventTrigger_Generic
{
// Construction:
public:
	CL_EventTrigger_Generic();

	virtual ~CL_EventTrigger_Generic();

// Attributes:
public:
	virtual bool get_flag() const=0;

// Operations:
public:
	virtual void reset() const=0;
	// Reset trigger flag.

	virtual void set_flag()=0;
	// Raise trigger flag.

	virtual bool wait(int timeout = -1) const=0;
	// Wait until one of the event triggers. The timeout is in milliseconds.
	// Returns true if the event triggered, false if it timed out.

	void add_ref();
	// Increase reference count.

	void release_ref();
	// Decrease reference count.

// Implementation:
private:
	int ref_count;
};

#endif
