/*
	$Id: browse_server.h,v 1.1 2002/11/02 19:45:59 mbn Exp $

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
class CL_NetPacket;

//: Browse server to a browse master.
//- <p>The browse service is used to register a server on a browse master.</p>
class CL_BrowseServer
{
//! Construction:
public:
	//: Setup a browsing server for an application with the specified identifier
	//: and the game server running on the specified server address.
	CL_BrowseServer(
		const std::string &app_id,
		const CL_IPAddress &server_address);

	//: Disconnect from browse master.
	virtual ~CL_BrowseServer();

//! Attributes:
public:

//! Operations:
public:
	//: Set/change the server description on the browse master.
	void set_description(const CL_NetPacket &description);

	//: Connect to browser master and register ourselves.
	void connect(const CL_IPAddress &browse_master);

	//: Listen for local area network browse queries on the specified port.
	void listen(int port);

//! Implementation:
private:
	CL_BrowseServer_Generic *impl;
};

#endif
