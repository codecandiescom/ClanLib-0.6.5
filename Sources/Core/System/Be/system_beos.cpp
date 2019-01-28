/*
	$Id: system_beos.cpp,v 1.1 2000/07/18 17:04:27 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		main. Initialization of the BeOS version of ClanLib.

*/

#include "API/Core/System/system.h"
#include <OS.h>

int suspended_time = 0; 
bool time_running = true;
int start_pause;

// int CL_System::suspend_time = 0; 
// bool CL_System::time_running = true;
// int CL_System::start_pause;

void CL_System::suspend_time()
{
	if (time_running)
	{
		time_running = false;
		start_pause = get_time();
	}
}

void CL_System::resume_time()
{
	if (!time_running)
	{
		time_running = true;
		suspended_time += get_time() - start_pause;
	}
}

unsigned int CL_System::get_time()
{
	return (system_time() / 1000) - suspended_time;
}

void CL_System::sleep(int millis)
{
	snooze(millis*1000);
}
