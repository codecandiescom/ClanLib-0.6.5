/*
	$Id: browse_master.cpp,v 1.3 2001/09/08 19:24:18 japj Exp $

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

#include "API/Network/browse_master.h"
#include "browse_master_generic.h"
#include <stdlib.h>

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseMaster construction:

CL_BrowseMaster::CL_BrowseMaster(const CL_IPAddress &bind_address)
: impl(NULL)
{
	impl = new CL_BrowseMaster_Generic(bind_address);
	impl->run();
}
	
CL_BrowseMaster::~CL_BrowseMaster()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseMaster attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseMaster operations:

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseMaster implementation:
