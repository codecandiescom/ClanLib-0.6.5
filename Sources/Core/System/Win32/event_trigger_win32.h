/*
	$Id: event_trigger_win32.h,v 1.6 2001/12/05 21:33:41 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_event_trigger_generic_win32
#define header_event_trigger_generic_win32

#include "../Generic/event_trigger_generic.h"
#include "event_listener_win32.h"

class CL_EventTrigger_Win32 : public CL_EventTrigger_Generic
{
// Construction:
public:
	CL_EventTrigger_Win32();

	virtual ~CL_EventTrigger_Win32();

// Attributes:
public:
	bool get_flag() const;

	HANDLE get_handle() const { return wait_handle; }

// Operations:
public:
	// Reset trigger flag.
	virtual void reset() const;

	// Raise trigger flag.
	virtual void set_flag();

	// Wait until one of the event triggers. The timeout is in milliseconds.
	// Returns true if the event triggered, false if it timed out.
	virtual bool wait(int timeout = -1) const;

	friend class CL_EventListener_Win32;

// Implementation:
private:
	// Win32 event handle. Used to wait with WaitForSingleObject().
	HANDLE wait_handle;
};

#endif
