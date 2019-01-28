/*
	$Id: browse_server.cpp,v 1.2 2001/09/08 19:24:18 japj Exp $

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

#include "API/Network/browse_server.h"
#include "browse_server_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseServer construction:

CL_BrowseServer::CL_BrowseServer(
	const CL_IPAddress &browse_master,
	const std::string &app_id,
	const CL_IPAddress &server_address,
	const std::string &description)
: impl(NULL)
{
	impl = new CL_BrowseServer_Generic(
		browse_master,
		app_id,
		server_address,
		description);
}

CL_BrowseServer::~CL_BrowseServer()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseServer attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseServer operations:

void CL_BrowseServer::set_description(const std::string &description)
{
	impl->set_description(description);
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseServer implementation:
