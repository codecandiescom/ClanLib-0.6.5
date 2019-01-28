/*
	$Id: system.h,v 1.10 2002/01/06 13:27:48 mbn Exp $

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

#ifndef header_system
#define header_system

class CL_EventListener;
class CL_Signal_v0;

//: Generic functions that didn't fit in other places.
class CL_System
{
public:
//! Operations:
	//: Checks for system events, updates input and other stuff.
	static void keep_alive();

	//: Checks for system events (pump the CL_KeepAlive objects).
	//- Return when 'millis' has elapsed.
	static void keep_alive(int millis);

	//: Checks for system events (pump the CL_KeepAlive objects).
	//- Return when one of the events trigger, or when timeout has elapsed.
	static bool keep_alive(CL_EventListener &events, int timeout = -1);

	//: Sleep for 'millis' milliseconds.
	static void sleep(int millis);

	//: Get the current time (since system boot), in milliseconds.
	static unsigned int get_time();

	//: Suspend the time.
	//: get_time() will continue to return the same value until resume_time() is called.
	static void suspend_time();

	//: Resumes the time.
	//: get_time() will return the current time again.
	static void resume_time();
	
	//: Returns true if MMX technology is available on this CPU.
	static bool detect_mmx();
	
	//: Returns true if 3DNow! technology is available on this CPU.
	static bool detect_3dnow();

	//: Returns true if extended 3DNow! is present on this CPU.
	static bool detect_ext_3dnow();

	//: Returns the signal emitted when WM_QUIT or similar window system quit
	//: message is received.
	static CL_Signal_v0 &sig_quit();
};

#endif
