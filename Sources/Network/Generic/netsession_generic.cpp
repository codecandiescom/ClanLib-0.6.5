/*
	$Id: netsession_generic.cpp,v 1.4 2001/05/09 18:28:56 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include <Network/Generic/netsession_generic.h>
#include <Network/Generic/network_generic.h>
#include <Network/Generic/network_delivery_impl.h>

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession_Generic construction:

CL_NetSession_Generic::CL_NetSession_Generic(CL_ConnectionProvider *provider)
: provider(provider), ref_count(0)
{
}

CL_NetSession_Generic::~CL_NetSession_Generic()
{
	delete provider;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession_Generic operations:

void CL_NetSession_Generic::add_ref()
{
	ref_count++;
}

void CL_NetSession_Generic::release_ref()
{
	ref_count--;
	if (ref_count == 0) delete this;
}
