/*
	$Id: browse_client.cpp,v 1.3 2001/09/08 19:24:18 japj Exp $

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

#include "API/Network/browse_client.h"
#include "browse_client_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseClient construction:

CL_BrowseClient::CL_BrowseClient(
	const CL_IPAddress &browse_master,
	const std::string &app_id)
: impl(NULL)
{
	impl = new CL_BrowseClient_Generic(browse_master, app_id);
}

CL_BrowseClient::~CL_BrowseClient()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseClient attributes:

CL_Signal_v2<const CL_IPAddress &, const std::string &> &CL_BrowseClient::sig_server_added()
{
	return impl->sig_server_added;
}

CL_Signal_v2<const CL_IPAddress &, const std::string &> &CL_BrowseClient::sig_server_updated()
{
	return impl->sig_server_updated;
}

CL_Signal_v1<const CL_IPAddress &> &CL_BrowseClient::sig_server_removed()
{
	return impl->sig_server_removed;
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseClient operations:

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseClient implementation:
