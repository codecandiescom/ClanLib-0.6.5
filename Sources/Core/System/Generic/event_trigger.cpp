/*
	$Id: event_trigger.cpp,v 1.6 2001/12/05 23:50:52 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Core/System/event_trigger.h"
#include "event_trigger_generic.h"

#ifdef WIN32
#include "Core/System/Win32/event_trigger_win32.h"
#else
#include "Core/System/Unix/event_trigger_unix.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger construction:

CL_EventTrigger::CL_EventTrigger()
: impl(NULL)
{
#ifdef WIN32
	impl = new CL_EventTrigger_Win32;
#else
	impl = new CL_EventTrigger_Unix;
#endif
	impl->add_ref();

	reset();
}

CL_EventTrigger::CL_EventTrigger(const CL_EventTrigger &copy)
: impl(copy.impl)
{
	impl->add_ref();
}

CL_EventTrigger::~CL_EventTrigger()
{
	impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger attributes:

bool CL_EventTrigger::get_flag() const
{
	return impl->get_flag();
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger operations:

CL_EventTrigger &CL_EventTrigger::operator =(const CL_EventTrigger &copy)
{
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();

	return *this;
}

void CL_EventTrigger::reset() const
{
	impl->reset();
}

void CL_EventTrigger::set_flag()
{
	impl->set_flag();
}

bool CL_EventTrigger::wait(int timeout)
{
	return impl->wait(timeout);
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger implementation:
