/*
	$Id: browse_server_generic.h,v 1.2 2002/11/02 19:57:05 mbn Exp $

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
#include "API/Network2/Socket/socket.h"
#include "API/Network2/Socket/ip_address.h"
#include "API/Network2/NetSession/netpacket.h"

class CL_BrowseServer_Generic
{
//! Construction:
public:
	//: Setup a browsing server for an application with the specified identifier
	//: and the game server running on the specified server address.
	CL_BrowseServer_Generic(
		const std::string &app_id,
		const CL_IPAddress &server_address);
	
	//: Disconnect from browse master.
	virtual ~CL_BrowseServer_Generic();

//! Attributes:
public:

//! Operations:
public:
	//: Change the server description on the browse master.
	void set_description(const CL_NetPacket &description);

	//: Connect to browser master and register ourselves.
	void connect(const CL_IPAddress &browse_master);

	//: Listen for local area network browse queries on the specified port.
	void listen(int port);

//! Implementation:
private:
	std::string app_id;
	CL_NetPacket description;
	CL_IPAddress server_address;
	CL_Socket sock;
	bool connected;
};

#endif
