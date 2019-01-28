/*
	$Id: browse_client_generic.h,v 1.5 2001/09/08 19:24:18 japj Exp $

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
#include "API/Network/ip_address.h"
#include "API/Network/socket.h"
#include <string>

class CL_BrowseClient_Generic : public CL_Runnable, CL_KeepAlive
{
//!Construction:
public:
	CL_BrowseClient_Generic(
		const CL_IPAddress &browse_master,
		const std::string &app_id);
	// Connect to browse master and start downloading getting browse list.

	virtual ~CL_BrowseClient_Generic();

//!Attributes:
public:
	CL_Signal_v2<const CL_IPAddress &, const std::string &> sig_server_added;
	// Signal that is called when the browse client is notified about a server addition.

	CL_Signal_v2<const CL_IPAddress &, const std::string &> sig_server_updated;
	// Signal that is called when the browse client is notified about a server update.

	CL_Signal_v1<const CL_IPAddress &> sig_server_removed;
	// Signal that is called when the browse client is notified about a server removal.

//!Operations:
public:

//!Implementation:
private:
	virtual void keep_alive();
	virtual void run();

	CL_IPAddress browse_master;
	std::string app_id;

	std::list< std::pair<CL_IPAddress, std::string> > added_list;
	std::list< std::pair<CL_IPAddress, std::string> > updated_list;
	std::list<CL_IPAddress> removed_list;

	CL_Socket sock;
	CL_Thread thread;
	CL_Mutex mutex;
	volatile bool stop_thread;
	volatile bool exception_thrown;
	std::string exception;
};

#endif
