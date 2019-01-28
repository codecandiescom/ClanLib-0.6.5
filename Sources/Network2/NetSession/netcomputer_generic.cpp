
#include "netcomputer_generic.h"
#include "netstream_generic.h"
#include "netsession_generic.h"
#include "netpacket_generic.h"
#include "API/Core/System/threadfunc_v0.h"
#include "API/Core/System/event_trigger.h"
#include "API/Core/System/event_listener.h"
#include "API/Core/System/error.h"
#include "API/Core/IOData/outputsource_memory.h"
#include <iostream>
#include <cstdio>

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer_Generic construction:

CL_NetComputer_Generic::CL_NetComputer_Generic(const CL_Socket &sock, CL_NetSession_Generic *netsession)
:
	sock(sock), disconnected(false), netsession(netsession), data_left(0), netstream(0), local_atom_counter(0),
	send_pos(0), ref(0), async_boot(false), async_port(""), channel_id_counter(0), shutdown(false)
{
	address = sock.get_dest_address();
	netsession->new_connections.push(CL_NetComputer(this));

	thread = CL_Thread(new CL_ThreadFunc_Runnable_v0<CL_NetComputer_Generic>(this, &CL_NetComputer_Generic::worker_thread), true);
	thread.start();
}

CL_NetComputer_Generic::CL_NetComputer_Generic(const std::string &hostname, const std::string &port, CL_NetSession_Generic *netsession)
:
	sock(CL_Socket::tcp), disconnected(false), netsession(netsession), data_left(0), netstream(0),
	local_atom_counter(0), send_pos(0), ref(0), async_boot(true), async_hostname(hostname), async_port(port),
	channel_id_counter(0), shutdown(false)
{
	netsession->new_connections.push(CL_NetComputer(this));

	thread = CL_Thread(new CL_ThreadFunc_Runnable_v0<CL_NetComputer_Generic>(this, &CL_NetComputer_Generic::worker_thread), true);
	thread.start();
}

CL_NetComputer_Generic::~CL_NetComputer_Generic()
{
	shutdown = true;
	wakeup_trigger.set_flag();
	thread.wait();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer_Generic attributes:

int CL_NetComputer_Generic::get_ref() const
{
	return ref;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer_Generic operations:

void CL_NetComputer_Generic::add_ref()
{
	ref++;
}

int CL_NetComputer_Generic::release_ref()
{
	ref--;
	if (ref == 0)
	{
		delete this;
		return 0;
	}
	return ref;
}

void CL_NetComputer_Generic::send_packet(const std::string &packet_channel, CL_NetPacket_Generic *packet)
{
	CL_MutexSection mutex_section(&mutex);
	CL_OutputSource_Memory cmd;

	std::map<std::string, int>::iterator it = local_atoms.find(packet_channel);
	if (it == local_atoms.end())
	{
		local_atoms[packet_channel] = local_atom_counter++;
		it = local_atoms.find(packet_channel);

		cmd.write_uchar8(cmd_create_atom);
		cmd.write_ushort16(it->second);
		cmd.write_string(it->first);
	}

	cmd.write_uchar8(cmd_netpacket_msg);
	cmd.write_ushort16(it->second);
	cmd.write_ushort16(packet->size);
	cmd.write(packet->data, packet->size);

	bool queue_empty = send_queue.empty();
	send_queue.push(cmd.get_data());
	if (queue_empty) wakeup_trigger.set_flag();
}

void CL_NetComputer_Generic::send_stream_message(unsigned int channel_id, const std::string &data)
{
	CL_OutputSource_Memory cmd;
	cmd.write_uchar8(cmd_netstream_msg);
	cmd.write_ushort16(channel_id);
	cmd.write_ushort16(data.length());
	cmd.write(data.data(), data.length());

	CL_MutexSection mutex_section(&mutex);

	bool queue_empty = send_queue.empty();
	send_queue.push(cmd.get_data());
	if (queue_empty) wakeup_trigger.set_flag();
}

int CL_NetComputer_Generic::send_stream_connect(const std::string &stream_channel, CL_NetStream_Generic *stream)
{
	CL_MutexSection mutex_section(&mutex);
	CL_OutputSource_Memory cmd;

	std::map<std::string, int>::iterator it = local_atoms.find(stream_channel);
	if (it == local_atoms.end())
	{
		local_atoms[stream_channel] = local_atom_counter++;
		it = local_atoms.find(stream_channel);

		cmd.write_uchar8(cmd_create_atom);
		cmd.write_ushort16(it->second);
		cmd.write_string(it->first);
	}

	int channel_id = channel_id_counter++;
	streams[channel_id] = stream;
	
	cmd.write_uchar8(cmd_netstream_connect);
	cmd.write_ushort16(it->second); // channel atom
	cmd.write_ushort16(channel_id);

	bool queue_empty = send_queue.empty();
	send_queue.push(cmd.get_data());
	if (queue_empty) wakeup_trigger.set_flag();
	
	return channel_id;
}

void CL_NetComputer_Generic::send_stream_close(int channel_id)
{
	CL_MutexSection mutex_section(&mutex);
	CL_OutputSource_Memory cmd;

	cmd.write_uchar8(cmd_netstream_closed);
	cmd.write_ushort16(channel_id);
	
	bool queue_empty = send_queue.empty();
	send_queue.push(cmd.get_data());
	if (queue_empty) wakeup_trigger.set_flag();
	
	std::map<int, CL_NetStream_Generic *>::iterator it = streams.find(channel_id);
	if (it != streams.end())
	{
		it->second->closed = true;
		streams.erase(it);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer_Generic implementation:

void CL_NetComputer_Generic::worker_thread()
{
	try
	{
		if (async_boot) // async connect. Do the connect now.
		{
			async_boot = false;
			address = CL_IPAddress(async_hostname, async_port);
			sock.bind(CL_IPAddress());
			sock.connect(address);
			//address = sock.get_dest_address();
		}

		while (!disconnected)
		{
			CL_MutexSection mutex_section(&mutex, false);

			// Wait for activity:
			CL_EventListener listener;
			mutex_section.enter();
			listener.add_trigger(&wakeup_trigger);
			listener.add_trigger(sock.get_read_trigger());
			if (!send_queue.empty()) listener.add_trigger(sock.get_write_trigger());
			mutex_section.leave();

			listener.wait();
			if (shutdown) return;

			// Process activity:
			wakeup_trigger.reset();
			update_socket();
		}
	}
	catch (CL_Error err)
	{
		CL_MutexSection mutex_section(&mutex, false);

		disconnect_reason = err.message;
		disconnected = true;
	}

	// Remove computer from processing queue if disconnected.
	if (disconnected)
	{
		CL_MutexSection mutex_section(&netsession->mutex);
		netsession->computers.remove(this);
		netsession->disconnections.push(CL_NetComputer(this));
	}
}

#define cl_min(a, b) (a < b ? a : b)

void CL_NetComputer_Generic::update_socket()
{
	if (disconnected) return;

	if (sock.get_read_trigger()->get_flag()) // Data available on socket:
	{
		char buffer[16*1024];

		if (data_left > 0) // currently routing message data.
		{
			int received = 0;
			
			received = sock.recv(buffer, cl_min(data_left, 16*1024));
			if (received == 0) throw CL_Error("Connection closed by foreign host");
			
			if (netstream) // Send data to netstream:
			{
				CL_MutexSection mutex_section(&netstream->mutex);
				netstream->receive_queue.push(std::string(buffer, received));
				netstream->read_trigger.set_flag();
			}
			else // Send data to netpacket
			{
				int pos = netpacket.get_size();
				netpacket.resize(pos + received);
				memcpy(netpacket.get_data()+pos, buffer, received);
			}

			data_left -= received;

			if (data_left == 0) // finished reading message data
			{
				if (!netstream)
				{
					CL_MutexSection mutex_section(&netsession->mutex);
					netsession->received_netpacket(remote_atoms[dest_netpacket_atom], netpacket, this);
					netpacket = CL_NetPacket();
				}

				netstream = 0;
			}
		}
		else // hmm, new message.
		{
			unsigned char command = sock.input.read_uchar8();
			switch (command)
			{
			case cmd_create_atom:
				{
					int atom_id = sock.input.read_ushort16();
					std::string atom_str = sock.input.read_string();

					remote_atoms[atom_id] = atom_str;
				}
				break;

			case cmd_netstream_connect:
				{
					std::map< std::string, CL_Signal_v1<CL_NetStream &> >::iterator it_connect;

					std::string channel_name = remote_atoms[sock.input.read_ushort16()];
					int channel_id = sock.input.read_ushort16();
					CL_MutexSection mutex_section(&netsession->mutex);
					it_connect = netsession->map_netstream_connect.find(channel_name);
					if (it_connect == netsession->map_netstream_connect.end())
					{
						// Tried to connect to non-existant netstream.
						mutex_section.leave();
						send_stream_close(channel_id);
					}
					CL_NetStream_Generic *stream = new CL_NetStream_Generic(channel_id, this, netsession);
					streams[channel_id] = stream;
					CL_NetSession_Generic::NewStreamPair pair(stream, channel_name);
					netsession->new_streams.push(pair);
				}
				break;

			case cmd_netstream_msg:
				{
					std::map< int, CL_NetStream_Generic *>::iterator it_channel;

					CL_MutexSection mutex_section(&mutex);
					unsigned int channel_id = sock.input.read_ushort16();
					data_left = sock.input.read_ushort16();
					it_channel = streams.find(channel_id);
					if (it_channel == streams.end())
					{
						// Tried to send data to non-existant netstream connection.
						netstream = 0;
						send_stream_close(channel_id);
					}
					else
					{
						netstream = it_channel->second;
					}
				}
				break;

			case cmd_netstream_closed:
				{
					std::map<int, CL_NetStream_Generic *>::iterator it_channel;

					CL_MutexSection mutex_section(&mutex);
					unsigned int channel_id = sock.input.read_ushort16();
					it_channel = streams.find(channel_id);
					if (it_channel != streams.end())
					{
						CL_MutexSection mutex_section(&it_channel->second->mutex);
						it_channel->second->closed = true;
						it_channel->second->read_trigger.set_flag();
						streams.erase(it_channel);
					}
				}
				break;

			case cmd_netpacket_msg:
				{
					dest_netpacket_atom = sock.input.read_ushort16();
					data_left = sock.input.read_ushort16();
				}
				break;
			}
		}
	}

	CL_MutexSection mutex_section(&mutex);
//		if (sock.get_write_trigger()->get_flag()) // Can write data to socket
	while (!send_queue.empty())
	{
		std::string &msg = send_queue.front();

		int sent = sock.send(msg.substr(send_pos));
		send_pos += sent;
		if (send_pos != msg.size()) break;

		send_queue.pop();
		send_pos = 0;
	}
}
