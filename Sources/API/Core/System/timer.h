/*
	$Id: timer.h,v 1.13 2002/01/23 11:47:54 sphair Exp $

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

#ifndef header_timer
#define header_timer

#include "keep_alive.h"
#include "../../signals.h"

//: Timer signal emitter.
//- Use this class to emit a signal as a timer event.
class CL_Timer : public CL_KeepAlive
{
//! Construction:
public:
	//: Constructs and registers the timer as a keep_alive listener.
	//: Note that it is disabled by default, so you need to enable() it.
	//- interval - Interval in millisecs between each signal emit.
	CL_Timer(unsigned long interval = 1000);
	
//! Attributes:
public:
	//: Returns the current time interval.
	//- Returns - The current time interval.
	unsigned long get_interval();

	//: Returns true if the timer is enabled, false otherwise.
	bool is_enabled();

//! Operations:
public:
	//: Sets a new interval for the timer.
	//- interval - Interval in millisecs between each signal emit.
	void set_interval(unsigned long interval);

	//: Call this function to enable the timer.
	void enable();
	
	//: Call this function to disable the timer.
	void disable();
	
//! Signals:
public:
	//: Signal emitted everytime the interval has elapsed.
	CL_Signal_v0 &sig_timer();
	
//! Implementation:
private:
	virtual void keep_alive();

	unsigned long interval;
	unsigned long last_time;
	bool enabled;

	CL_Signal_v0 signal_timer;
};

#endif
