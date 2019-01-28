/*
	$Id: browse_server_generic.cpp,v 1.3 2001/09/08 19:24:18 japj Exp $

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
	const CL_IPAddress &browse_master,
	const std::string &app_id,
	const CL_IPAddress &server_address,
	const std::string &description)
: app_id(app_id), sock(CL_Socket::tcp)
{
	sock.connect(browse_master);

	CL_OutputSource_Memory output;
	output.write_string(app_id.c_str());
	output.write_int32(1);      // we are a server
	output.write_int32(server_address.get_address());
	output.write_short16(server_address.get_port());
	output.write_string(description);
	sock.send(output.get_data().data(), output.get_data().size());
}

CL_BrowseServer_Generic::~CL_BrowseServer_Generic()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseServer_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseServer_Generic operations:

void CL_BrowseServer_Generic::set_description(const std::string &description)
{
	CL_OutputSource_Memory output;
	output.write_string(app_id.c_str());
	output.write_string(description);
	sock.send(output.get_data().data(), output.get_data().size());
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseServer_Generic implementation:
