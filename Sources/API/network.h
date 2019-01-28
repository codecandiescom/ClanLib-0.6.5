/*
	$Id: network.h,v 1.12 2002/03/01 22:00:16 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//: <p>ClanLib Networking. Includes the references for all levels of networking
//: available in clanlib, from the low level socket API, the middle level
//: netsessions, and the high level netobject replication and world
//: template.</p>
//! Global=Network

#ifndef header_network_api
#define header_network_api

#ifdef WIN32
#pragma warning( disable : 4786)
#endif

#include "Network/setupnetwork.h"
#include "Network/network.h"
#include "Network/netcomputer.h"
#include "Network/netgroup.h"
#include "Network/netmessage.h"
#include "Network/netsession.h"
#include "Network/subchannel.h"
#include "Network/netobject.h"
#include "Network/netobject_channel.h"
#include "Network/netvariables.h"
#include "Network/world_template.h"
#include "Network/ip_address.h"
#include "Network/browse_client.h"
#include "Network/browse_server.h"
#include "Network/browse_master.h"
#include "Network/socket.h"
#include "Network/inputsource_socket.h"
#include "Network/outputsource_socket.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanNetwork.lib")
#else
#pragma comment(lib, "clanNetworkd.lib")
#endif
#endif

#endif
