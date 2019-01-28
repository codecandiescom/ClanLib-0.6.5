/*
	$Id: event_trigger_unix.cpp,v 1.5 2001/12/22 11:25:19 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "event_trigger_unix.h"

// note: this cannot be replaced by <ctime>! (timeval needs to be defined)
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Unix construction:

CL_EventTrigger_Unix::CL_EventTrigger_Unix()
{
	pipe(wait_pipe);
}

CL_EventTrigger_Unix::~CL_EventTrigger_Unix()
{
	close(wait_pipe[0]);
	close(wait_pipe[1]);
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Unix attributes:

bool CL_EventTrigger_Unix::get_flag() const
{
	return wait(0);
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Unix operations:

void CL_EventTrigger_Unix::reset() const
{
	char v = 0;
	while (wait(0)) read(wait_pipe[0], &v, 1);
}

void CL_EventTrigger_Unix::set_flag()
{
	char v = 1;
	if (!wait(0)) write(wait_pipe[1], &v, 1);
}

bool CL_EventTrigger_Unix::wait(int timeout) const
{
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(wait_pipe[0], &rfds);

	timeval tv;
	tv.tv_sec = timeout/1000;
	tv.tv_usec = (timeout%1000)*1000;

	int result = select(wait_pipe[0]+1, &rfds, NULL, NULL, (timeout == -1) ? NULL : &tv);
	if (result <= 0) return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Unix implementation:
