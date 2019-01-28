#ifdef WIN32
#pragma warning(disable : 4355)
#pragma warning(disable : 4503)
#endif

#include "netsession_generic.h"
#include "netcomputer_generic.h"
#include "API/Network2/NetSession/netstream.h"
#include "API/Core/System/threadfunc_v0.h"
#include "API/Core/System/event_listener.h"
#include "API/Core/System/error.h"
#include "API/Core/System/clanstring.h"
#include <iostream>
#include <cstdio>

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession_Generic construction:

CL_NetSession_Generic::CL_NetSession_Generic(const std::string &app_id)
:
	app_id(app_id),
	listen_thread(new CL_ThreadFunc_Runnable_v0<CL_NetSession_Generic>(this, &CL_NetSession_Generic::accept_thread), true),
	ref(0),
	show_debug(false)
{
	boot();
}

CL_NetSession_Generic::~CL_NetSession_Generic()
{
	shutdown();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession_Generic operations:

void CL_NetSession_Generic::add_ref()
{
	ref++;
}

int CL_NetSession_Generic::release_ref()
{
	ref--;
	if (ref == 0)
	{
		delete this;
		return 0;
	}
	return ref;
}

CL_NetComputer CL_NetSession_Generic::connect(const CL_IPAddress &address)
{
	// Connect to server:
	CL_Socket sock(CL_Socket::tcp);
//	sock.bind(address);
	sock.connect(address);

	// Establish clanlib netcomputer protocol communication over it:
	CL_MutexSection mutex_section(&mutex);
	CL_NetComputer_Generic *comp = new CL_NetComputer_Generic(sock, this);
	comp->add_ref();
	computers.push_back(comp);

	return CL_NetComputer(comp);
}

CL_NetComputer CL_NetSession_Generic::connect_async(const std::string &hostname, const std::string &port)
{
	// Establish clanlib netcomputer protocol communication over it:
	CL_MutexSection mutex_section(&mutex);
	CL_NetComputer_Generic *comp = new CL_NetComputer_Generic(hostname, port, this);
	comp->add_ref();
	computers.push_back(comp);

	return CL_NetComputer(comp);
}

void CL_NetSession_Generic::start_listen(const std::string &port)
{
	accept_shutdown_trigger.reset();
	accept_socket = CL_Socket(CL_Socket::tcp);
	accept_socket.set_nonblocking(false);

	
//	accept_socket.bind(CL_IPAddress((std::string)foo.get_string());
	accept_socket.bind(CL_IPAddress(port));

	accept_socket.listen(5);
	listen_thread.start();
}

void CL_NetSession_Generic::stop_listen()
{
	accept_shutdown_trigger.set_flag();
	listen_thread.wait();
}

void CL_NetSession_Generic::received_netpacket(const std::string &channel, const CL_NetPacket &packet, CL_NetComputer_Generic *comp)
{
	if(show_debug)
		std::cout << "Netpacket arrived in " << channel << std::endl;
	received_netpackets[channel].push(PacketCompPair(packet, CL_NetComputer(comp)));
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession_Generic implementation:

void CL_NetSession_Generic::boot()
{
}

void CL_NetSession_Generic::shutdown()
{
	stop_listen();

	CL_MutexSection mutex_section(&mutex);

	// Disconenct all computers:
	all = CL_NetGroup();
	std::list<CL_NetComputer_Generic *>::iterator it;
	for (it = computers.begin(); it != computers.end(); it++) delete *it;
	computers = std::list<CL_NetComputer_Generic *>();

	// Empty list of received netpackets.
	received_netpackets = std::map< std::string, std::queue<PacketCompPair> >();
}

void CL_NetSession_Generic::keep_alive()
{
	CL_MutexSection mutex_section(&mutex);

	// Signal received netpackets on each channel.
	std::map< std::string, std::queue<PacketCompPair> >::iterator it;
	for (it = received_netpackets.begin(); it != received_netpackets.end(); it++)
	{
		CL_Signal_v2<CL_NetPacket &, CL_NetComputer &> &chan_sig = map_netpacket_receive[it->first];
		std::queue<PacketCompPair> &packets = it->second;

		while (!packets.empty())
		{
			chan_sig(packets.front().first, packets.front().second);
			packets.pop();
		}
	}

	// Signal incoming connections:
	while (!new_connections.empty())
	{
		all.push_back(new_connections.front());
		sig_computer_connected(new_connections.front());
		new_connections.pop();
	}

	// Signal incoming reconnections:
	while (!reconnections.empty())
	{
		all.push_back(reconnections.front());
		sig_computer_reconnected(reconnections.front());
		reconnections.pop();
	}
	
	// Signal incoming netstreams:
	while (!new_streams.empty())
	{
		NewStreamPair &pair = new_streams.front();
		CL_Signal_v1<CL_NetStream &> &chan_sig = map_netstream_connect[pair.second];
		CL_NetStream stream(pair.first);
		chan_sig(stream);
		new_streams.pop();
	}

	// Signal incoming disconnections:
	while (!disconnections.empty())
	{
		all.remove(disconnections.front());
		sig_computer_disconnected(disconnections.front());
		disconnections.pop();
	}
}

void CL_NetSession_Generic::accept_thread()
{
	try
	{
		CL_EventListener listener;
		listener.add_trigger(accept_socket.get_read_trigger());
		listener.add_trigger(&accept_shutdown_trigger);

		while (true)
		{
			listener.wait();
			if (accept_shutdown_trigger.get_flag()) break; // stop_listen was called.

			CL_Socket sock = accept_socket.accept();
			CL_MutexSection mutex_section(&mutex);

			CL_NetComputer_Generic *comp = new CL_NetComputer_Generic(sock, this);
			comp->add_ref();
			computers.push_back(comp);
		}
	}
	catch (CL_Error err)
	{
		std::cout << "Error: " << err.message.c_str() << std::endl;
	}
}
