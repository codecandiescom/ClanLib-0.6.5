/*
	$Id: browse_client.h,v 1.14 2001/10/10 11:40:52 sphair Exp $

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

#include "../signals.h"
#include <string>

class CL_IPAddress;
class CL_BrowseClient_Generic;

//: Browse client to a browse master.
//- <p>The browse client is used to connect to a browse master and get a list of registered
//- applications. Using this information, the application can generate a list of available
//- servers that the user can connect to.</p>
class CL_BrowseClient
{
//! Construction:
public:
	//: Connect to browse master and start downloading getting browse list.
	CL_BrowseClient(
		const CL_IPAddress &browse_master,
		const std::string &app_id);

	//: Browse Client Destructor
	virtual ~CL_BrowseClient();

//! Attributes:
public:
	//: Signal that is called when the browse client is notified about a server addition.
	CL_Signal_v2<const CL_IPAddress &, const std::string &> &sig_server_added();

	//: Signal that is called when the browse client is notified about a server update.
	CL_Signal_v2<const CL_IPAddress &, const std::string &> &sig_server_updated();

	//: Signal that is called when the browse client is notified about a server removal.
	CL_Signal_v1<const CL_IPAddress &> &sig_server_removed();

//! Operations:
public:

//! Implementation:
private:
	CL_BrowseClient_Generic *impl;
};

#endif
