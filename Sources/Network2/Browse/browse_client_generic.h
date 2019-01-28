/*
	$Id: browse_client_generic.h,v 1.2 2002/11/02 19:57:05 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_browse_client_generic
#define header_browse_client_generic

#include "API/signals.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/mutex.h"
#include "API/Core/System/keep_alive.h"
#include "API/Network2/Socket/ip_address.h"
#include "API/Network2/Socket/socket.h"
#include "API/Network2/NetSession/netpacket.h"
#include <string>

class CL_NetPacket;

class CL_BrowseClient_Generic : public CL_Runnable, CL_KeepAlive
{
//! Construction:
public:
	//: Connect to browse master and start downloading getting browse list.
	CL_BrowseClient_Generic(const std::string &app_id);

	virtual ~CL_BrowseClient_Generic();

//! Attributes:
public:
	//: Signal that is called when the browse client is notified about a server addition.
	CL_Signal_v2<const CL_IPAddress &, CL_NetPacket &> sig_server_added;

	//: Signal that is called when the browse client is notified about a server update.
	CL_Signal_v2<const CL_IPAddress &, CL_NetPacket &> sig_server_updated;

	//: Signal that is called when the browse client is notified about a server removal.
	CL_Signal_v1<const CL_IPAddress &> sig_server_removed;

//! Operations:
public:
	//: Connect to browse master and start downloading getting browse list.
	void connect(const CL_IPAddress &browse_master);

	//: Broadcast browse query on local area network for browse servers on the specified port.
	void broadcast(int port);

//! Implementation:
private:
	virtual void keep_alive();

	virtual void run();

	CL_IPAddress browse_master;
	
	std::string app_id;

	std::list< std::pair<CL_IPAddress, CL_NetPacket> > added_list;
	
	std::list< std::pair<CL_IPAddress, CL_NetPacket> > updated_list;
	
	std::list<CL_IPAddress> removed_list;

	CL_Socket sock;

	CL_Thread thread;
	
	CL_Mutex mutex;
	
	volatile bool stop_thread;
	
	volatile bool exception_thrown;
	
	std::string exception;
};

#endif
