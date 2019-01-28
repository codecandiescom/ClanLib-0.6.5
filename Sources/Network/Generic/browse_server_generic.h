/*
	$Id: browse_server_generic.h,v 1.3 2001/09/08 19:24:18 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_browse_server_generic
#define header_browse_server_generic

#include <string>
#include "API/Network/socket.h"
class CL_IPAddress;

class CL_BrowseServer_Generic
{
//!Construction:
public:
	CL_BrowseServer_Generic(
		const CL_IPAddress &browse_master,
		const std::string &app_id,
		const CL_IPAddress &server_address,
		const std::string &description);
	// Connect to browse master and register ourselves with the passed server address
	// and description.
	
	virtual ~CL_BrowseServer_Generic();
	// Disconnect from browse master.

//!Attributes:
public:

//!Operations:
public:
	void set_description(const std::string &description);
	// Change the server description on the browse master.

//!Implementation:
private:
	std::string app_id;
	CL_Socket sock;
};

#endif
