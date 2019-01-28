/*
	$Id: event_listener_win32.h,v 1.4 2001/09/08 19:12:45 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_event_listener_win32
#define header_event_listener_win32

#include "Core/System/Generic/event_listener_generic.h"

class CL_EventListener_Win32 : public CL_EventListener_Generic
{
// Construction:
public:
	CL_EventListener_Win32(CL_EventListener *owner);

	virtual ~CL_EventListener_Win32();

// Implementation:
private:
	virtual bool wait(int timeout);
	// Wait until one of the events triggers. The timeout is in milliseconds.
	// Returns true if a event triggered, false if it timed out.
};

#endif
