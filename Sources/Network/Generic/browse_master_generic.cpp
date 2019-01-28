/*
	$Id: browse_master_generic.cpp,v 1.7 2001/09/08 19:24:18 japj Exp $

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
#include "API/Network/inputsource_socket.h"
#include "API/Network/outputsource_socket.h"

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseMaster_Generic construction:

CL_BrowseMaster_Generic::CL_BrowseMaster_Generic(const CL_IPAddress &bind_address)
: accept_socket(CL_Socket::tcp)
{
	accept_socket.bind(bind_address);
	accept_socket.listen(5);
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

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseMaster_Generic implementation:

/////////////////////////////////////////////////////////////////////////////
// CL_ServerIterator

CL_ServerIterator::CL_ServerIterator(CL_BrowseMaster_Generic *doc)
: doc(doc)
{
}

CL_ServerIterator::~CL_ServerIterator()
{
}

bool CL_ServerIterator::next(CL_ServerDescription &desc)
{
	return false;
}

std::list<CL_ServerDescription> CL_ServerIterator::get_added_servers()
{
	std::list<CL_ServerDescription> list;
	return list;
}

std::list<CL_ServerDescription> CL_ServerIterator::get_removed_servers()
{
	std::list<CL_ServerDescription> list;
	return list;
}

std::list<CL_ServerDescription> CL_ServerIterator::get_updated_servers()
{
	std::list<CL_ServerDescription> list;
	return list;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ServerRegistrator

CL_ServerRegistrator::CL_ServerRegistrator(CL_BrowseMaster_Generic *doc)
: doc(doc)
{
}

CL_ServerRegistrator::~CL_ServerRegistrator()
{
}

void CL_ServerRegistrator::register_server(const CL_ServerDescription &desc)
{
}

void CL_ServerRegistrator::update_server(const CL_ServerDescription &desc)
{
}

void CL_ServerRegistrator::deregister_server()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseConnection

CL_BrowseConnection::CL_BrowseConnection(CL_Socket socket, CL_BrowseMaster_Generic *doc)
: thread(this), socket(socket), doc(doc)
{
}

void CL_BrowseConnection::run()
{
	try
	{
		CL_OutputSource_Socket output(socket);
		CL_InputSource_Socket input(socket);

		std::string app_id = input.read_string();
		int connection_type = input.read_int32();

		if (connection_type == 0) // server
		{
		}
		else if (connection_type == 1) // client
		{
			CL_ServerDescription desc;

			// Send initial server list:
			CL_ServerIterator server_it(doc);

			while (server_it.next(desc))
			{
				if (desc.app_id != app_id) continue;

				output.write_int32(0); // added
				output.write_uint32(desc.server.get_address());
				output.write_ushort16(desc.server.get_port());
				output.write_string(desc.desc);
			}

			// Send events:
			while (true)
			{
				server_it.get_trigger().wait();

				std::list<CL_ServerDescription> list;
				std::list<CL_ServerDescription>::iterator desc_it;

				list = server_it.get_added_servers();
				for (desc_it = list.begin(); desc_it != list.end(); desc_it++)
				{
					output.write_int32(0); // added
					output.write_uint32(desc_it->server.get_address());
					output.write_ushort16(desc_it->server.get_port());
					output.write_string(desc_it->desc);
				}

				list = server_it.get_updated_servers();
				for (desc_it = list.begin(); desc_it != list.end(); desc_it++)
				{
					output.write_int32(1); // modified
					output.write_uint32(desc_it->server.get_address());
					output.write_ushort16(desc_it->server.get_port());
					output.write_string(desc_it->desc);
				}

				list = server_it.get_removed_servers();
				for (desc_it = list.begin(); desc_it != list.end(); desc_it++)
				{
					output.write_int32(2); // removed
					output.write_uint32(desc_it->server.get_address());
					output.write_ushort16(desc_it->server.get_port());
					output.write_string(desc_it->desc);
				}
			}
		}
	}
	catch (CL_Error error)
	{
		// communication error with client.
	}
}
