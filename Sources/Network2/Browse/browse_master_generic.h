/*
	$Id: browse_master_generic.h,v 1.2 2002/11/02 19:57:05 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_browse_master_generic
#define header_browse_master_generic

#include "API/Network2/Socket/socket.h"
#include "API/Network2/Socket/ip_address.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/event_trigger.h"
#include "API/Core/System/mutex.h"

#include <string>
#include <map>

class CL_BrowseMaster_Generic;
class CL_ServerDatabase;

class CL_BrowseConnection : public CL_Runnable
{
public:
	CL_BrowseConnection(CL_Socket socket, CL_BrowseMaster_Generic *doc);
	virtual ~CL_BrowseConnection() { }

	CL_Thread &get_thread() { return thread; }

private:
	virtual void run();

	void run_server();

	void run_client();

	CL_Thread thread;
	CL_Socket socket;

	std::string app_id;
	CL_ServerDatabase *database;
	CL_BrowseMaster_Generic *doc;
};

class CL_BrowseMaster_Generic
{
//! Construction:
public:
	CL_BrowseMaster_Generic();
	
	virtual ~CL_BrowseMaster_Generic();

//! Attributes:
public:

//! Operations:
public:
	void run();

	//: Setup port accepting browse clients and browse servers.
	void start_listen(const CL_IPAddress &bind_address);

	//: Stop listening.
	void stop_listen();

	//: Find the database for the application ID.
	CL_ServerDatabase *get_database(const std::string &app_id);

	//: Releases the database.
	//- <p>Must be called when done using database after a get_database call.</p>
	void release_database(const std::string &app_id);

//! Implementation:
private:
	CL_Mutex mutex;

	CL_Socket accept_socket;

	std::map<std::string, CL_ServerDatabase *> databases;
};

#endif
