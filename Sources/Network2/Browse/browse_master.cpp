/*
	$Id: browse_master.cpp,v 1.2 2002/11/02 19:57:05 mbn Exp $

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

#include "API/Network2/Browse/browse_master.h"
#include "browse_master_generic.h"
#include <stdlib.h>

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseMaster construction:

CL_BrowseMaster::CL_BrowseMaster()
: impl(NULL)
{
	impl = new CL_BrowseMaster_Generic();
}
	
CL_BrowseMaster::~CL_BrowseMaster()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseMaster attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseMaster operations:

void CL_BrowseMaster::start_listen(const CL_IPAddress &bind_address)
{
	impl->start_listen(bind_address);
}

void CL_BrowseMaster::stop_listen()
{
	impl->stop_listen();
}

void CL_BrowseMaster::run()
{
	impl->run();
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseMaster implementation:
