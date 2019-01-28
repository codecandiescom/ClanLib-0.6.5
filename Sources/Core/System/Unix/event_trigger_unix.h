/*
	$Id: event_trigger_unix.h,v 1.2 2001/09/08 19:12:45 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_event_trigger_unix
#define header_event_trigger_unix

#include "../Generic/event_trigger_generic.h"

class CL_EventTrigger_Unix : public CL_EventTrigger_Generic
{
// Construction:
public:
	CL_EventTrigger_Unix();

	virtual ~CL_EventTrigger_Unix();

// Attributes:
public:
	virtual bool get_flag() const;

// Operations:
public:
	virtual void reset() const;
	// Reset trigger flag.

	virtual void set_flag();
	// Raise trigger flag.

	virtual bool wait(int timeout = -1) const;
	// Wait until one of the event triggers. The timeout is in milliseconds.
	// Returns true if the event triggered, false if it timed out.

// Implementation:
public:
	int wait_pipe[2];
	// Waiting pipe.
};

#endif
