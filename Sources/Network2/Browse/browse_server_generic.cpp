/*
	$Id: browse_server_generic.cpp,v 1.1 2002/11/02 19:46:04 mbn Exp $

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

#include "browse_server_generic.h"
#include "API/Core/IOData/outputsource_memory.h"

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseServer_Generic construction:

CL_BrowseServer_Generic::CL_BrowseServer_Generic(
	const std::string &app_id,
	const CL_IPAddress &server_address)
: app_id(app_id), server_address(server_address), sock(CL_Socket::tcp), connected(false)
{
}

CL_BrowseServer_Generic::~CL_BrowseServer_Generic()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseServer_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseServer_Generic operations:

void CL_BrowseServer_Generic::set_description(const CL_NetPacket &new_description)
{
	description = new_description;

	if (connected) sock.output.write_string(
		std::string((char *) description.get_data(), description.get_size()));
}

void CL_BrowseServer_Generic::connect(const CL_IPAddress &browse_master)
{
	sock.connect(browse_master);

	sock.output.write_string(app_id);
	sock.output.write_int32(1); // we are a server
	sock.output.write_string(server_address.get_port());
	sock.output.write_string(std::string((char *) description.get_data(), description.get_size()));

	connected = true;
}

void CL_BrowseServer_Generic::listen(int port)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseServer_Generic implementation:
