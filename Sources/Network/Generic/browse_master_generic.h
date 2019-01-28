/*
	$Id: browse_master_generic.h,v 1.10 2001/10/14 20:38:51 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_browse_master_generic
#define header_browse_master_generic

#include "API/Network/socket.h"
#include "API/Network/ip_address.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/event_trigger.h"
#include "API/Core/System/mutex.h"
#include <list>
#include <map>

class CL_BrowseMaster_Generic;

class CL_ServerDescription
// server description.
{
public:
	std::string app_id;
	CL_IPAddress server;
	std::string desc;
};

class CL_ServerIterator
// thread safe server iteration interface.
{
public:
	CL_ServerIterator(CL_BrowseMaster_Generic *doc);
	~CL_ServerIterator();

	bool next(CL_ServerDescription &desc);
	// Returns the next server description entry, if any.

	CL_EventTrigger &get_trigger() { return trigger; }
	std::list<CL_ServerDescription> get_added_servers();
	std::list<CL_ServerDescription> get_removed_servers();
	std::list<CL_ServerDescription> get_updated_servers();

private:
	std::list<CL_ServerDescription> added_servers;
	std::list<CL_ServerDescription> removed_servers;
	std::list<CL_ServerDescription> updated_servers;
	CL_EventTrigger trigger;
	CL_BrowseMaster_Generic *doc;
};

class CL_ServerRegistrator
// thread safe server registration interface.
{
public:
	CL_ServerRegistrator(CL_BrowseMaster_Generic *doc);
	~CL_ServerRegistrator();

	void register_server(const CL_ServerDescription &desc);
	void update_server(const CL_ServerDescription &desc);
	void deregister_server();

private:
	CL_BrowseMaster_Generic *doc;
};

class CL_BrowseConnection : public CL_Runnable
{
public:
	CL_BrowseConnection(CL_Socket socket, CL_BrowseMaster_Generic *doc);
	virtual ~CL_BrowseConnection() { }

	CL_Thread &get_thread() { return thread; }

private:
	virtual void run();

	CL_Thread thread;
	CL_Socket socket;
	CL_BrowseMaster_Generic *doc;
};

class CL_BrowseMaster_Generic
{
//!Construction:
public:
	CL_BrowseMaster_Generic(const CL_IPAddress &bind_address);
	
	virtual ~CL_BrowseMaster_Generic();

//!Attributes:
public:

//!Operations:
public:
	// thread unsafe functions: (only call from main thread)
	void run();

	// thread safe functions:
	void add_server(
		const CL_Socket &socket,
		const CL_IPAddress &server,
		const std::string &desc);

	void remove_server(const CL_IPAddress &server);

	CL_ServerDescription get_server_description(CL_BrowseConnection *src);

//!Implementation:
private:
	CL_Mutex mutex;
	CL_Socket accept_socket;

// data retrival helpers:
friend class CL_ServerDescription;
friend class CL_ServerIterator;
friend class CL_ServerRegistrator;
};

#endif
