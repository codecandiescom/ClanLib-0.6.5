/*
	$Id: browse_client.cpp,v 1.2 2002/11/02 19:57:05 mbn Exp $

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

#include "API/Network2/Browse/browse_client.h"
#include "browse_client_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseClient construction:

CL_BrowseClient::CL_BrowseClient(
	const std::string &app_id)
: impl(NULL)
{
	impl = new CL_BrowseClient_Generic(app_id);
}

CL_BrowseClient::~CL_BrowseClient()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseClient operations:

void CL_BrowseClient::connect(const CL_IPAddress &browse_master)
{
	impl->connect(browse_master);
}

void CL_BrowseClient::broadcast(int port)
{
	impl->broadcast(port);
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseClient signals:

CL_Signal_v2<const CL_IPAddress &, CL_NetPacket &> &CL_BrowseClient::sig_server_added()
{
	return impl->sig_server_added;
}

CL_Signal_v2<const CL_IPAddress &, CL_NetPacket &> &CL_BrowseClient::sig_server_updated()
{
	return impl->sig_server_updated;
}

CL_Signal_v1<const CL_IPAddress &> &CL_BrowseClient::sig_server_removed()
{
	return impl->sig_server_removed;
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseClient implementation:
