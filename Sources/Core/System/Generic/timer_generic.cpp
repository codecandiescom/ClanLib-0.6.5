/*
	$Id: timer_generic.cpp,v 1.10 2002/01/23 22:51:15 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Core/System/timer.h"
#include "API/Core/System/system.h"

CL_Timer::CL_Timer(unsigned long _interval)
: interval(_interval), last_time(0), enabled(false)
{
}

void CL_Timer::keep_alive()
{
	if (!enabled)
		return;

	unsigned long cur = CL_System::get_time();
	if (last_time > cur)
		last_time = cur; // The counter has overflowed the variable - reset it
		
	while (cur >= last_time + interval)
	{
		sig_timer()();
		last_time += interval;
	}
}

void CL_Timer::enable()
{
	enabled = true;
	last_time = CL_System::get_time();
}

void CL_Timer::disable()
{
	enabled = false;
}

bool CL_Timer::is_enabled()
{
	return enabled;
}

void CL_Timer::set_interval(unsigned long interval)
{
	this->interval = interval;
	last_time = CL_System::get_time();
}

unsigned long CL_Timer::get_interval()
{
	return interval;
}

CL_Signal_v0 &CL_Timer::sig_timer()
{
	return signal_timer;
}
