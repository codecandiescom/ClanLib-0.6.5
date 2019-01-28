/*
	$Id: event_trigger_generic.cpp,v 1.2 2001/09/08 19:12:44 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "event_trigger_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Generic construction:

CL_EventTrigger_Generic::CL_EventTrigger_Generic()
: ref_count(0)
{
}

CL_EventTrigger_Generic::~CL_EventTrigger_Generic()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Generic operations:

void CL_EventTrigger_Generic::add_ref()
{
	ref_count++;
}

void CL_EventTrigger_Generic::release_ref()
{
	ref_count--;
	if (ref_count == 0) delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Generic implementation:
