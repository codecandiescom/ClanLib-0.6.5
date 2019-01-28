/*
	$Id: browse_server.h,v 1.12 2001/10/10 11:40:52 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanNetwork="Browsing"
//! header=network.h

#ifndef header_browse_server
#define header_browse_server

#include <string>

class CL_IPAddress;
class CL_BrowseServer_Generic;

//: Browse server to a browse master.
//- The browse service is used to register a server on a browse master.
class CL_BrowseServer
{
//! Construction:
public:
	//: <p>Connect to browse master and register ourselves with the passed server address
	//: and description.</p>
	CL_BrowseServer(
		const CL_IPAddress &browse_master,
		const std::string &app_id,
		const CL_IPAddress &server_address,
		const std::string &description);
	
	//: Disconnect from browse master.
	virtual ~CL_BrowseServer();

//! Attributes:
public:

//! Operations:
public:
	//: Change the server description on the browse master.
	void set_description(const std::string &description);

//! Implementation:
private:
	CL_BrowseServer_Generic *impl;
};

#endif
