/*
	$Id: netsession_server.cpp,v 1.16 2001/12/11 20:44:23 mbn Exp $

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
#include <Network/Generic/netsession_server.h>
#include <Network/Generic/netsession_generic.h>
#include <Network/Generic/network_generic.h>
#include <Core/IOData/Generic/outputsource_memory_generic.h>
#include <Core/IOData/Generic/inputsource_memory_generic.h>
#include <iostream>

CL_NetSession_Server::CL_NetSession_Server(
	CL_ConnectionProvider *provider,
	const std::string &game_id,
	int port)
: CL_NetSession_Generic(provider)
{
	mutex = CL_Mutex::create();

	this->game_id = game_id;
	this->port = port;
	
	id_counter = 1; // server is zero, that's why we start at 1.
	
	udp_connection = provider->create_udp_connection(port);
	
	provider->start_accept_on_port(port);

	exit_thread = false;
	thread = CL_Thread::create(this);
	thread->start();
	thread->set_priority(cl_priority_highest); // we need more power on the warp engines!
}

CL_NetSession_Server::~CL_NetSession_Server()
{
	exit_thread = true;
	thread->wait();
	delete thread;

	provider->stop_accept_on_port(port);
	delete mutex;
}

CL_NetComputer &CL_NetSession_Server::get_server()
{
	static CL_NetComputer null_server;
	return null_server;
}

CL_NetGroup &CL_NetSession_Server::get_all()
{
	return all;
}

bool CL_NetSession_Server::peek(int channel) const
{
	CL_MutexSection mutex_section(mutex);

	CL_NetChannelQueue *queue = find_queue(channel);
	if (queue != NULL) return !queue->empty();
	
	return false;
}

CL_NetMessage CL_NetSession_Server::receive(int channel, int timeout)
{
	CL_MutexSection mutex_section(mutex);

	CL_NetChannelQueue *queue = create_queue(channel);

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

	if (queue->empty()) throw CL_Error("No message to receive.");
	CL_NetMessage msg = queue->front(); queue->pop();
	check_trigger();
	return msg;
}

void CL_NetSession_Server::send(
	const int dest_channel,
	const CL_NetComputer &dest,
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

	CL_NetComputer_Host *dest_host = get_computer_host(dest);
/*
	CL_NetChannelQueue *queue = find_queue(dest_channel);
	
	if (queue == NULL)
		return; // nobody has read or write access. Don't send message.

	if (has_read_access(queue, dest_host) == false)
		return; // dest_host is not allowed to read messages on channel.
*/
	// permissions ok, send message:
	CL_OutputSource_MemoryGeneric output;
	output.write_int32(Packet_NetChannel_Message_ToClient);
	output.write_int32(dest_channel);
	output.write_int32(message.data.size());
	output.write(message.data.data(), message.data.size());
	
	CL_ConnectionPacket packet;
	packet.size = output.size();
	packet.data = output.get_data();

	dest_host->connection->send(packet);
}

void CL_NetSession_Server::send(
	const int dest_channel,
	const CL_NetGroup &dest,
	const CL_NetMessage &message,
	bool reliable /*= true*/)
{
	CL_MutexSection mutex_section(mutex);

	const std::list<CL_NetComputer> &computers = dest.get_computers();
	std::list<CL_NetComputer>::const_iterator it;

	for (
		it = computers.begin();
		it != computers.end();
		it++)
	{
		send(
			dest_channel,
			*it,
			message,
			reliable);
	}
}

CL_NetComputer CL_NetSession_Server::receive_computer_leave()
{
	CL_MutexSection mutex_section(mutex);

	if (leave_queue.empty())
	{
		throw CL_Error("Computer leave queue is empty.");
	}

	CL_NetComputer ret = leave_queue.front();
	leave_queue.pop();
	check_trigger();

	// Remove all access rules for this object.
	for (
		std::map<int, CL_NetChannelQueue>::iterator it = netchannels.begin();
		it != netchannels.end();
		it++)
	{
		CL_NetChannelQueue &queue = (*it).second;
		queue.access.erase(get_computer_host(ret)->id);
	}

	return ret;
}

CL_NetComputer CL_NetSession_Server::receive_computer_join()
{
	CL_MutexSection mutex_section(mutex);

	if (join_queue.empty()) throw CL_Error("Computer join queue is empty.");
	CL_NetComputer ret(join_queue.front());
	join_queue.pop();
	check_trigger();

	return ret;
}

CL_NetComputer CL_NetSession_Server::receive_computer_rejoin()
{
	CL_MutexSection mutex_section(mutex);

	if (rejoin_queue.empty())
	{
		throw CL_Error("Computer rejoin queue is empty.");
	}

	CL_NetComputer ret(rejoin_queue.front());
	rejoin_queue.pop();
	check_trigger();

	return ret;
}

bool CL_NetSession_Server::receive_session_closed()
{
	return false; // we are the server - only closed if deleted.
}

int CL_NetSession_Server::access_status(int /*channel*/) const
{
	// server has always full access.
	return ACCESS_CHANNEL_READ|ACCESS_CHANNEL_WRITE;
}

bool CL_NetSession_Server::is_writable(int /*channel*/) const
{
	return true;
}

bool CL_NetSession_Server::is_readable(int /*channel*/) const
{
	return true;
}

// Client side only:
int CL_NetSession_Server::receive_access_changed()
{
	return -1; // we are not a client!
}

// Server side only:
void CL_NetSession_Server::set_access(
	int channel,
	const CL_NetComputer &computer,
	int access_rights)
{
	CL_MutexSection mutex_section(mutex);

	CL_NetComputer_Host *computer_host = get_computer_host(computer);
	CL_NetChannelQueue *queue = create_queue(channel);

	int cur_access = 0;
	std::map<int,int>::iterator access_it = queue->access.find(computer_host->id);
	if (access_it != queue->access.end())
	{
		cur_access = (*access_it).second;
		(*access_it).second = access_rights;
	}
	else
	{
		queue->access.insert(std::pair<int,int>(computer_host->id, access_rights));
	}

	if (cur_access != access_rights)
	{
		CL_OutputSource_MemoryGeneric output;
		output.write_int32(Packet_NetChannel_AccessChange);
		output.write_int32(channel);
		output.write_int32(access_rights);
	
		CL_ConnectionPacket packet;
		packet.size = output.size();
		packet.data = output.get_data();

		computer_host->connection->send(packet);
	}
}

void CL_NetSession_Server::set_access(
	int channel,
	const CL_NetGroup &group,
	int access_rights)
{
	CL_MutexSection mutex_section(mutex);

	for (
		std::list<CL_NetComputer>::const_iterator it = group.get_computers().begin();
		it != group.get_computers().end();
		it++)
	{
		set_access(
			channel,
			*it,
			access_rights);
	}
}

void CL_NetSession_Server::keep_alive()
{
	CL_MutexSection mutex_section(mutex);
	
	if (exit_thread) return;

	// check connections for data:
	std::list<CL_NetComputer>::iterator it_computers = computers.begin();
	while (it_computers != computers.end())
	{
		CL_NetComputer_Host *cur_computer = (CL_NetComputer_Host *) it_computers->impl;
		while (cur_computer->connection->peek())
		{
			CL_ConnectionPacket msg = cur_computer->connection->receive();
			
			CL_InputSource_MemoryGeneric input(msg.data, msg.size, true);
			switch (input.read_int32())
			{
			case Packet_NetChannel_AccessChange:
				cl_info(info_network, "Network Protocol error!");
				cl_assert(false);
				break;
			
			case Packet_NetChannel_Message_ToServer:
				{
					static bool warning = true;
					if (warning)
					{
						cl_info(info_network, "permissions not checked: Not implemented yet.");
						warning = false;
					}
					CL_NetChannelQueue *queue = create_queue(input.read_int32());
					CL_NetMessage game_msg;
					int size = input.read_int32();
					char *data = new char[size];
					try
					{
						game_msg.from = cur_computer;
						input.read(data, size);
						game_msg.data.append(data, size);
						queue->push(game_msg);
						trigger.set_flag();
					}
					catch (...)
					{
						delete[] data;
						throw;
					}
					delete[] data;
				}
				break;

			default:
				cl_info(info_network, "Network Protocol error!");
				cl_assert(false);
			}
		}

		if (cur_computer->connection->connection_lost())
		{
			leave_queue.push(cur_computer);
			all.remove(CL_NetComputer(cur_computer));
			provider->remove_connection(cur_computer->connection);
			it_computers = computers.erase(it_computers);

			trigger.set_flag();
		}
		else
		{
			it_computers++;
		}
	}


	// check for "hello to you too" connections:
	std::list<CL_NetComputer>::iterator it_prejoin = prejoin.begin();
	while (it_prejoin != prejoin.end())
	{
		CL_NetComputer_Host *cur_computer = (CL_NetComputer_Host *) it_prejoin->impl;

		if (cur_computer->connection->connection_lost()) // how rude!
		{
			it_prejoin = prejoin.erase(it_prejoin);
			continue;
		}

		if (cur_computer->connection->peek())
		{
			CL_ConnectionPacket msg = cur_computer->connection->receive();
			
			CL_InputSource_MemoryGeneric input(msg.data, msg.size);
			if (msg.size >= (int) sizeof(int))
			{
				switch (input.read_int32())
				{
				case Packet_Hello_ToYouToo: // ahh - he wants to join.
					computers.push_back(cur_computer);
					all.add(CL_NetComputer(cur_computer));
					join_queue.push(cur_computer);
					it_prejoin = prejoin.erase(it_prejoin);
					trigger.set_flag();
					continue;

				default:
					cl_info(info_network, "Network Protocol error!");
					cl_assert(false);
				}
			}
			
			delete[] ((char*) msg.data);
		}

		it_prejoin++;
	}
	
	// accept incoming clients:
	while (true)
	{
		CL_Connection *new_client = provider->accept();
		if (new_client == NULL) break;
		
		CL_OutputSource_MemoryGeneric msg;
		msg.write_int32(Packet_Hello);
		msg.write_int32(id_counter);
		msg.write_int32(game_id.length());
		msg.write(game_id.c_str(), game_id.length());
		
		CL_ConnectionPacket packet(msg.get_data(), msg.size());
		new_client->send(packet);
		
		prejoin.push_back(new CL_NetComputer_Host(this, new_client, id_counter++));
	}
	
	// check for UDP data:
	while (udp_connection->peek())
	{
		CL_UDPConnectionPacket netmsg = udp_connection->receive();
		
		try
		{
			CL_InputSource_MemoryGeneric message(netmsg.data, netmsg.size);
			
			int msgtype = message.read_int32();
			switch (msgtype)
			{
			case 0: // game ping message. Reply with game_id.
				{
					std::string req_game_id = message.read_string();
					if (req_game_id == game_id)
					{
						CL_OutputSource_MemoryGeneric msg;
						msg.write_int32(1); // game ping reply message.
						msg.write_string(game_id.c_str());
					
						CL_UDPConnectionPacket reply;
						reply.data = msg.get_data();
						reply.size = msg.size();
						reply.ip_addr = netmsg.ip_addr;
						reply.port = netmsg.port;
						udp_connection->send(reply);
					}
				}
				break;
				
			case 1: // game ping reply message.
				// ignore it. server shouldn't get this message!
				break;
				
			case 2: // normal udp game packet.
				// not implemented yet.
				break;
			}
		}
		catch (CL_Error err)
		{
			// protocol error. We need some logging stuff.
		}
	}
}

void CL_NetSession_Server::run()
{
	while (exit_thread == false)
	{
		keep_alive();

		provider->wait_for_connection_data(mutex);
	}

	std::cout << "exiting CL_NetSession_Server thread" << std::endl;
}

CL_NetChannelQueue *CL_NetSession_Server::find_queue(int netchannel) const
{
	std::map<int, CL_NetChannelQueue>::const_iterator it = netchannels.find(netchannel);
	if (it == netchannels.end()) return NULL;
	
	return (CL_NetChannelQueue *) (&(*it).second);
}

CL_NetChannelQueue *CL_NetSession_Server::create_queue(int netchannel)
{
	CL_NetChannelQueue *found = find_queue(netchannel);
	if (found != NULL) return found;

	CL_NetChannelQueue c;
	netchannels.insert(std::pair<int, CL_NetChannelQueue>(netchannel, c));

	return &netchannels[netchannel];
}

bool CL_NetSession_Server::has_read_access(
	CL_NetChannelQueue *channel,
	CL_NetComputer_Host *host)
{
	std::map<int, int>::iterator it = channel->access.find(host->id);
	if (it == channel->access.end()) return false;
	
	return ((*it).second & ACCESS_CHANNEL_READ) == ACCESS_CHANNEL_READ;
}

void CL_NetSession_Server::check_trigger()
{
	bool queues_empty =
		join_queue.empty() &&
		rejoin_queue.empty() &&
		leave_queue.empty();

	std::map<int,CL_NetChannelQueue>::iterator it;
	for (it = netchannels.begin(); it != netchannels.end(); it++)
	{
		if (!it->second.empty()) queues_empty = false;
	}

	if (queues_empty) trigger.reset();
}

// ---------

CL_NetComputer_Host::CL_NetComputer_Host(
	CL_NetSession_Server *session,
	CL_Connection *connection,
	int id)
:
	CL_NetComputer_Generic(session)
{
	this->connection = connection;
	this->id = id;
}

CL_NetComputer_Host::~CL_NetComputer_Host()
{
	delete connection;
}

unsigned long CL_NetComputer_Host::get_address() const
{
	return connection->get_peer_address();
}

unsigned short CL_NetComputer_Host::get_port() const
{
	return connection->get_peer_port();
}

void CL_NetComputer_Host::disconnect()
{
	connection->disconnect();
}
