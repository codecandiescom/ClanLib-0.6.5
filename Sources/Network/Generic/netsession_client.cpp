/*
	$Id: netsession_client.cpp,v 1.12 2001/11/01 20:27:57 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include <API/Core/System/error.h>
#include <API/Core/System/mutex.h>
#include <API/Network/netcomputer.h>
#include <API/Network/netgroup.h>
#include <API/Network/netmessage.h>
#include <API/Core/System/error.h>
#include <API/Core/System/thread.h>
#include <API/Core/System/cl_assert.h>
#include <API/Core/System/system.h>
#include <Network/Generic/network_delivery_impl.h>
#include <Network/Generic/netsession_client.h>
#include <Network/Generic/netsession_generic.h>
#include <Network/Generic/network_generic.h>
#include <Core/IOData/Generic/outputsource_memory_generic.h>
#include <Core/IOData/Generic/inputsource_memory_generic.h>

CL_NetSession_Client::CL_NetSession_Client(
	int ip_addr,
	int port,
	const std::string &game_id,
	CL_ConnectionProvider *provider)
: CL_NetSession_Generic(provider)
{
	mutex = CL_Mutex::create();

	tcp_connection = provider->create_tcp_connection(
		ip_addr,
		port);

	if (tcp_connection == NULL)
	{
		throw CL_Error("Could not connect to host.");
	}

	udp_connection = NULL; // still no support for udp in client.

	// client code currently only knows about the server.
	server = CL_NetComputer(new CL_NetComputer_Server(this));
	all.add(server);

	// do "hello to you too" handshake.
	CL_ConnectionPacket p = tcp_connection->receive();
	while (p.size == 0) // wait until we get a hello packet.
	{
		provider->wait_for_connection_data(mutex);
		p = tcp_connection->receive();
	}
	
	CL_InputSource_MemoryGeneric input(p.data, p.size, true);
	if (input.read_int32() != Packet_Hello)
	{
		throw CL_Error("Protocol error. Didn't get a Hello.");
	}
	our_id = input.read_int32();

	int size = input.read_int32();
	if (size > 1000)
		throw CL_Error("Protocol error. Game ID size above 1000.");

	char *str = new char[size+1];
	str[size] = 0; // null-terminate.
	input.read(str, size);
	
	if (game_id != str) // not same game_id. probably an other clanlib game.
	{
		delete[] str;
		throw CL_Error("Wrong netsession id.");
	}

	delete[] str;
	
	// say "HelloToYouToo" to officially join the netsession:
	CL_OutputSource_MemoryGeneric output;
	output.write_int32(Packet_Hello_ToYouToo);
	tcp_connection->send(
		CL_ConnectionPacket(
			output.get_data(),
			output.size()));

	exit_thread = false;
	thread = CL_Thread::create(this);
	thread->start();
	thread->set_priority(cl_priority_highest); // we need more power on the warp engines!
}

CL_NetSession_Client::~CL_NetSession_Client()
{
	exit_thread = true;
	thread->wait();
	delete thread;

	delete tcp_connection;
	delete udp_connection;

	// clean up connections to clients:
	for (
		std::list<CL_NetChannelQueue_Client*>::iterator it = netchannels.begin();
		it != netchannels.end();
		it++)
	{
		delete *it;
	}
/*
	should be enabled when the client knows about other clients...

	while (leave_queue.empty() == false)
	{
		delete leave_queue.pop();
	}
*/
	delete mutex;
}

CL_NetComputer &CL_NetSession_Client::get_server()
{
	return server;
}

CL_NetGroup &CL_NetSession_Client::get_all()
{
	return all;
}

bool CL_NetSession_Client::peek(int channel) const
{
	CL_MutexSection mutex_section(mutex);

	CL_NetChannelQueue_Client *queue = find_queue(channel);
	if (queue != NULL)
	{
		return !queue->empty();
	}

	return false;
}

CL_NetMessage CL_NetSession_Client::receive(int channel, int timeout)
{
	CL_MutexSection mutex_section(mutex);

	CL_NetChannelQueue_Client *queue = create_queue(channel);

	while (queue->empty()) 
	{
		int s = timeout;
		if (timeout < 0 || timeout > 20) s = 20;
		mutex->leave();
		CL_System::sleep(s);
		mutex->enter();

		if (timeout != -1)
		{
			timeout -= s;
			if (timeout <= 0) break;
		}
	}

	if (queue->empty()) throw CL_Error("No message to receive!");
	CL_NetMessage msg = queue->front(); queue->pop();
	check_trigger();
	return msg;
}

void CL_NetSession_Client::send(
	const int dest_channel,
	const CL_NetGroup &dest,
	const CL_NetMessage &message,
	bool reliable /*= true*/)
{
	CL_MutexSection mutex_section(mutex);

	static bool warning = true;
	if (warning && reliable == false)
	{
		cl_info(info_network, "cannot send data unreliable (udp): not implemented yet!");
		warning = false;
	}
/*	
	CL_NetChannelQueue_Client *queue = find_queue(dest_channel);

	if (queue == NULL)
		throw CL_Error("No write access to netchannel.");

	if ((queue->access & ACCESS_CHANNEL_WRITE) != ACCESS_CHANNEL_WRITE)
		throw CL_Error("No write access to netchannel.");
*/
	// permissions ok, send message:
	CL_OutputSource_MemoryGeneric output;
	output.write_int32(Packet_NetChannel_Message_ToServer);
	output.write_int32(dest_channel);

	// todo: write the computer id's on destination computers here.

	output.write_int32(message.data.size());
	output.write(message.data.data(), message.data.size());

	CL_ConnectionPacket packet;
	packet.size = output.size();
	packet.data = output.get_data();

	tcp_connection->send(packet);
}

CL_NetComputer CL_NetSession_Client::receive_computer_leave()
{
	throw CL_Error("Computer leave queue empty.");
}

CL_NetComputer CL_NetSession_Client::receive_computer_join()
{
	throw CL_Error("Computer join queue empty.");
}

CL_NetComputer CL_NetSession_Client::receive_computer_rejoin()
{
	throw CL_Error("Computer rejoin queue empty.");
}

bool CL_NetSession_Client::receive_session_closed()
{
	CL_MutexSection mutex_section(mutex);
	return tcp_connection->connection_lost();
}

int CL_NetSession_Client::access_status(int channel) const
{
	CL_MutexSection mutex_section(mutex);

	CL_NetChannelQueue_Client *queue = find_queue(channel);
	return (queue == NULL) ? 0 : queue->access;
}

bool CL_NetSession_Client::is_writable(int channel) const
{
	CL_MutexSection mutex_section(mutex);

	CL_NetChannelQueue_Client *queue = find_queue(channel);
	if (queue == NULL) return false;

	return (queue->access & ACCESS_CHANNEL_WRITE) == ACCESS_CHANNEL_WRITE;
}

bool CL_NetSession_Client::is_readable(int channel) const
{
	CL_MutexSection mutex_section(mutex);

	CL_NetChannelQueue_Client *queue = find_queue(channel);
	if (queue == NULL) return false;

	return (queue->access & ACCESS_CHANNEL_READ) == ACCESS_CHANNEL_READ;
}

// Client side only:
int CL_NetSession_Client::receive_access_changed()
{
	CL_MutexSection mutex_section(mutex);

	if (access_queue.empty()) return -1;
	int id = access_queue.front()->channel_id;
	access_queue.pop();
	check_trigger();
	return id;
}

// Server side only:
void CL_NetSession_Client::set_access(
	int channel,
	const CL_NetComputer &computer,
	int access_rights)
{
	throw CL_Error("We are not the network server. Cannot change access.");
}

void CL_NetSession_Client::set_access(
	int channel,
	const CL_NetGroup &group,
	int access_rights)
{
	throw CL_Error("We are not the network server. Cannot change access.");
}

void CL_NetSession_Client::keep_alive()
{
	CL_MutexSection mutex_section(mutex);

	if (tcp_connection->connection_lost())
	{
		provider->remove_connection(tcp_connection);
		return;
	}

	while (tcp_connection->peek())
	{
		if (exit_thread) return;

		CL_ConnectionPacket msg = tcp_connection->receive();
			
		CL_InputSource_MemoryGeneric input(msg.data, msg.size, true);
		switch (input.read_int32())
		{
		case Packet_NetChannel_AccessChange:
			{
				CL_NetChannelQueue_Client *queue = create_queue(input.read_int32());
				queue->access = input.read_int32();
				access_queue.push(queue);
				trigger.set_flag();
			}
			break;
		
		case Packet_NetChannel_Message_ToClient:
			{
				CL_NetChannelQueue_Client *queue = create_queue(input.read_int32());
				CL_NetMessage game_msg;
				int size = input.read_int32();
				char *data = new char[size];
				game_msg.from = server;
				input.read(data, size);
				game_msg.data.append(data, size);
				queue->push(game_msg);
				trigger.set_flag();
			}
			break;

		default:
			cl_info(info_network, "Network Protocol error!");
			cl_assert(false);
		}
	}
}

CL_NetChannelQueue_Client *CL_NetSession_Client::find_queue(int netchannel) const
{
	CL_MutexSection mutex_section(mutex);

	// Note: This really should be a STL map, not a list. Too lazy to fix it now -- mbn.
	for (
		std::list<CL_NetChannelQueue_Client*>::const_iterator it = netchannels.begin();
		it != netchannels.end();
		it++)
	{
		if ((*it)->channel_id == netchannel) return *it;
	}

	return NULL;
}

CL_NetChannelQueue_Client *CL_NetSession_Client::create_queue(int netchannel)
{
	CL_MutexSection mutex_section(mutex);

	CL_NetChannelQueue_Client *found = find_queue(netchannel);
	if (found != NULL) return found;
	
	CL_NetChannelQueue_Client *c = new CL_NetChannelQueue_Client;
	c->channel_id = netchannel;
	c->access = 0;

	netchannels.push_back(c);

	return c;
}

void CL_NetSession_Client::run()
{
	while (exit_thread == false)
	{
		keep_alive();
		provider->wait_for_connection_data(mutex);
	}
}

void CL_NetSession_Client::check_trigger()
{
	bool queues_empty = access_queue.empty();

	std::list<CL_NetChannelQueue_Client*>::iterator it;
	for (it = netchannels.begin(); it != netchannels.end(); it++)
	{
		if (!(*it)->empty()) queues_empty = false;
	}

	if (queues_empty) trigger.reset();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer_Server

CL_NetComputer_Server::CL_NetComputer_Server(CL_NetSession_Client *session)
: CL_NetComputer_Generic(session)
{
}

CL_NetComputer_Server::~CL_NetComputer_Server()
{
}

unsigned long CL_NetComputer_Server::get_address() const
{
	return get_session()->tcp_connection->get_peer_address();
}

unsigned short CL_NetComputer_Server::get_port() const
{
	return get_session()->tcp_connection->get_peer_port();
}

void CL_NetComputer_Server::disconnect()
{
	get_session()->tcp_connection->disconnect();
}

CL_NetSession_Client *CL_NetComputer_Server::get_session() const
{
	return static_cast<CL_NetSession_Client*>(CL_NetComputer_Generic::get_session());
}
