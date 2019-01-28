/*
	$Id: netsession_client.h,v 1.7 2001/05/09 18:54:03 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_netsession_client
#define header_netsession_client

#include <string>
#include <list>
#include <queue>

#include "API/Core/System/thread.h"
#include "API/Network/netgroup.h"
#include "netsession_generic.h"
#include "netcomputer_generic.h"

class CL_NetChannelQueue_Client;
class CL_NetSession_Client;

class CL_NetComputer_Server : public CL_NetComputer_Generic
{
public:
	CL_NetComputer_Server(CL_NetSession_Client *session);
	virtual ~CL_NetComputer_Server();

	virtual unsigned long get_address() const;
	virtual unsigned short get_port() const;
	virtual void disconnect();

	CL_NetSession_Client *get_session() const;
};

class CL_NetSession_Client : public CL_NetSession_Generic, CL_Runnable
{
public:
	CL_NetSession_Client(
		int ip_addr,
		int port,
		const std::string &game_id,
		CL_ConnectionProvider *provider);
	virtual ~CL_NetSession_Client();

	virtual CL_NetComputer &get_server();
	virtual CL_NetGroup &get_all();
	
	virtual bool peek(int channel) const;
	virtual CL_NetMessage receive(int channel, int timeout=-1);

	virtual void send(
		const int dest_channel,
		const CL_NetGroup &dest,
		const CL_NetMessage &message,
		bool reliable = true);

	virtual CL_NetComputer receive_computer_leave();
	virtual CL_NetComputer receive_computer_join();
	virtual CL_NetComputer receive_computer_rejoin();
	virtual bool receive_session_closed();
	virtual int access_status(int channel) const;
	virtual bool is_writable(int channel) const;
	virtual bool is_readable(int channel) const;

	// Client side only:
	virtual int receive_access_changed();

	// Server side only:
	virtual void set_access(
		int channel,
		const CL_NetComputer &computer,
		int access_rights=ACCESS_CHANNEL_READ|ACCESS_CHANNEL_WRITE);
	virtual void set_access(
		int channel,
		const CL_NetGroup &group,
		int access_rights=ACCESS_CHANNEL_READ|ACCESS_CHANNEL_WRITE);

protected: // from CL_NetSession_Generic
	virtual void keep_alive();

private: // implementation helper functions:
	CL_NetChannelQueue_Client *find_queue(int netchannel) const;
	CL_NetChannelQueue_Client *create_queue(int netchannel);
	void check_trigger();

private:
	CL_Connection *tcp_connection;
	CL_UDPConnection *udp_connection;
	std::list<CL_NetChannelQueue_Client*> netchannels;
	std::queue<CL_NetChannelQueue_Client*> access_queue;
	int our_id;
	
	CL_NetComputer server;
	CL_NetGroup all;
	
private: // multithread:
	CL_Mutex *mutex;
	CL_Thread *thread;
	volatile bool exit_thread;

	virtual void run();

friend class CL_NetComputer_Server;
};

class CL_NetChannelQueue_Client : public std::queue<CL_NetMessage>
{
public:
	int channel_id;
	int access;
};

#endif
