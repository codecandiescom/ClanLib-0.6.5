/*
	$Id: event_listener_unix.h,v 1.3 2001/09/08 19:12:45 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_event_listener_unix
#define header_event_listener_unix

#include "Core/System/Generic/event_listener_generic.h"

class CL_EventListener_Unix : public CL_EventListener_Generic
{
// Construction:
public:
	CL_EventListener_Unix(CL_EventListener *owner);

	virtual ~CL_EventListener_Unix();

// Implementation:
private:
	virtual bool wait(int timeout);
	// Wait until one of the events triggers. The timeout is in milliseconds.
	// Returns true if a event triggered, false if it timed out.
};

#endif
