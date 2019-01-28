/*
	$Id: netcomputer_generic.cpp,v 1.4 2001/05/09 18:54:03 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "netcomputer_generic.h"
#include "netsession_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer_Generic construction:

CL_NetComputer_Generic::CL_NetComputer_Generic(
	CL_NetSession_Generic *session)
:
	session(session),
	ref_count(0)
{
}

CL_NetComputer_Generic::~CL_NetComputer_Generic()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer_Generic attributes:

CL_NetSession_Generic *CL_NetComputer_Generic::get_session() const
{
	return session;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer_Generic operations:

void CL_NetComputer_Generic::add_ref()
{
	ref_count++;
}

void CL_NetComputer_Generic::release_ref()
{
	ref_count--;
	if (ref_count == 0) delete this;
}
