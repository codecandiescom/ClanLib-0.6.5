/*
	$Id: browse_client.h,v 1.1 2002/11/02 19:45:59 mbn Exp $

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

#ifndef header_browse_client
#define header_browse_client

#include "../../signals.h"
#include <string>

class CL_IPAddress;
class CL_BrowseClient_Generic;
class CL_NetPacket;

//: Browse client to a browse master.
//- <p>The browse client is used to connect to a browse master and get a list of registered
//- applications. Using this information, the application can generate a list of available
//- servers that the user can connect to.</p>
class CL_BrowseClient
{
//! Construction:
public:
	//: Construct browse client to browse applications with the specified identifier.
	CL_BrowseClient(const std::string &app_id);

	//: Browse Client Destructor
	virtual ~CL_BrowseClient();

//! Operations:
public:
	//: Connect to browse master and start downloading getting browse list.
	void connect(const CL_IPAddress &browse_master);

	//: Broadcast browse query on local area network for browse servers on the specified port.
	void broadcast(int port);

//! Signals:
public:
	//: Signal that is called when the browse client is notified about a server addition.
	CL_Signal_v2<const CL_IPAddress &, CL_NetPacket &> &sig_server_added();

	//: Signal that is called when the browse client is notified about a server update.
	CL_Signal_v2<const CL_IPAddress &, CL_NetPacket &> &sig_server_updated();

	//: Signal that is called when the browse client is notified about a server removal.
	CL_Signal_v1<const CL_IPAddress &> &sig_server_removed();

//! Implementation:
private:
	CL_BrowseClient_Generic *impl;
};

#endif
