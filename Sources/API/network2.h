/*
	$Id: network2.h,v 1.1 2002/11/02 19:45:57 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//: <p>Enables network communication.  Includes low-lever socket access as well
//: as higher-level network objects.</p>
//! Global=Network


#ifndef header_network_api
#define header_network_api

#ifdef WIN32
#pragma warning( disable : 4786)
#endif

#include "Network2/IRC/irc_connection.h"
#include "Network2/IRC/irc_numerics.h"

#include "Network2/Browse/browse_client.h"
#include "Network2/Browse/browse_server.h"
#include "Network2/Browse/browse_master.h"

#include "Network2/NetVariables/netvariables.h"

#include "Network2/NetObjects/netobject_client.h"
#include "Network2/NetObjects/netobject_controller.h"
#include "Network2/NetObjects/netobject_server.h"

#include "Network2/NetSession/inputsource_netpacket.h"
#include "Network2/NetSession/inputsource_netstream.h"
#include "Network2/NetSession/netcomputer.h"
#include "Network2/NetSession/netgroup.h"
#include "Network2/NetSession/netpacket.h"
#include "Network2/NetSession/netsession.h"
#include "Network2/NetSession/netstream.h"
#include "Network2/NetSession/outputsource_netpacket.h"
#include "Network2/NetSession/outputsource_netstream.h"

#include "Network2/Socket/inputsource_socket.h"
#include "Network2/Socket/ip_address.h"
#include "Network2/Socket/outputsource_socket.h"
#include "Network2/Socket/socket.h"

#include "Network2/setupnetwork.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanNetwork.lib")
#else
#pragma comment(lib, "clanNetworkd.lib")
#endif
#endif

#endif
