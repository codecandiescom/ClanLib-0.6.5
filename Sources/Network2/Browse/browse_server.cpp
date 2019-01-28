/*
	$Id: browse_server.cpp,v 1.2 2002/11/02 19:57:05 mbn Exp $

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

#include "API/Network2/Browse/browse_server.h"
#include "browse_server_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseServer construction:

CL_BrowseServer::CL_BrowseServer(
	const std::string &app_id,
	const CL_IPAddress &server_address)
: impl(NULL)
{
	impl = new CL_BrowseServer_Generic(
		app_id,
		server_address);
}

CL_BrowseServer::~CL_BrowseServer()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseServer attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseServer operations:

void CL_BrowseServer::set_description(const CL_NetPacket &description)
{
	impl->set_description(description);
}

void CL_BrowseServer::connect(const CL_IPAddress &browse_master)
{
	impl->connect(browse_master);
}

void CL_BrowseServer::listen(int port)
{
	impl->listen(port);
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseServer implementation:
