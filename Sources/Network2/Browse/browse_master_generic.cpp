/*
	$Id: browse_master_generic.cpp,v 1.2 2002/11/02 19:57:05 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifdef WIN32
#pragma warning (disable:4786) // stl debug string too long<tm>
#pragma warning (disable:4355) // 'this' used in base member initializer list
#endif

#include "browse_master_generic.h"
#include "API/Core/System/event_listener.h"
#include "API/Core/System/event_trigger.h"
#include "API/Core/System/error.h"
#include "API/Core/System/system.h"
#include "API/Network2/Socket/inputsource_socket.h"
#include "API/Network2/Socket/outputsource_socket.h"
#include "server_database.h"

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseMaster_Generic construction:

CL_BrowseMaster_Generic::CL_BrowseMaster_Generic()
: accept_socket(CL_Socket::tcp)
{
}
	
CL_BrowseMaster_Generic::~CL_BrowseMaster_Generic()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseMaster_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseMaster_Generic operations:

void CL_BrowseMaster_Generic::run()
{
	while (true)
	{
		accept_socket.get_read_trigger()->wait();

		CL_Socket new_sock = accept_socket.accept();
		CL_BrowseConnection *new_connection = new CL_BrowseConnection(new_sock, this);
		new_connection->get_thread().start();
	}
}

void CL_BrowseMaster_Generic::start_listen(const CL_IPAddress &bind_address)
{
	accept_socket.bind(bind_address);
	accept_socket.listen(5);
}

void CL_BrowseMaster_Generic::stop_listen()
{
}

CL_ServerDatabase *CL_BrowseMaster_Generic::get_database(const std::string &app_id)
{
	CL_MutexSection mutex_lock(&mutex);

	CL_ServerDatabase *database;
	std::map<std::string, CL_ServerDatabase *>::iterator it;

	it = databases.find(app_id);
	if (it == databases.end())
	{
		database = new CL_ServerDatabase;
		databases[app_id] = database;
	}
	else
	{
		database = it->second;
	}

	database->add_ref();
	return database;
}

void CL_BrowseMaster_Generic::release_database(const std::string &app_id)
{
	CL_MutexSection mutex_lock(&mutex);
	std::map<std::string, CL_ServerDatabase *>::iterator it;

	it = databases.find(app_id);
	if (it != databases.end())
	{
		if (it->second->release_ref() == 0) databases.erase(it);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseMaster_Generic implementation:


/////////////////////////////////////////////////////////////////////////////
// CL_BrowseConnection

CL_BrowseConnection::CL_BrowseConnection(CL_Socket socket, CL_BrowseMaster_Generic *doc)
: thread(this), socket(socket), database(0), doc(doc)
{
}

void CL_BrowseConnection::run()
{
	try
	{
		app_id = socket.input.read_string();
		database = doc->get_database(app_id);

		int connection_type = socket.input.read_int32();
		if (connection_type == 0) // server
		{
			run_server();
		}
		else if (connection_type == 1) // client
		{
			run_client();
		}
	}
	catch (CL_Error error)
	{
		// communication error with client.
	}

	if (database) doc->release_database(app_id);
}

void CL_BrowseConnection::run_server()
{
	std::string port = socket.input.read_string();
	CL_IPAddress address(socket.get_dest_address().get_address(), port);

	try
	{
		while (true)
		{
			std::string desc = socket.input.read_string();
			database->update_server(address, desc);
		}
	}
	catch (...)
	{
		database->remove_server(address);
		throw;
	}
}

void CL_BrowseConnection::run_client()
{
	CL_ServerChanges changes = database->get_changes();

	while (true)
	{
		std::map<CL_IPAddress, CL_ServerDescription *>::iterator it;
		for (it = changes.servers.begin(); it != changes.servers.end(); ++it)
		{
			const CL_IPAddress &address = it->first;
			CL_ServerDescription *desc = it->second;

			socket.output.write_int32(0); // updated
			socket.output.write_string(address.get_address());
			socket.output.write_string(address.get_port());
			socket.output.write_string(desc->desc);
		}

		std::list<CL_IPAddress>::iterator it2;
		for (it2 = changes.removed_servers.begin(); it2 != changes.removed_servers.end(); ++it2)
		{
			CL_IPAddress &address = *it2;
			socket.output.write_int32(1); // removed
			socket.output.write_string(address.get_address());
			socket.output.write_string(address.get_port());
		}

		database->release_changes(changes);
		CL_System::sleep(15000);
	}
}
