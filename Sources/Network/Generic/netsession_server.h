/*
	$Id: netsession_server.h,v 1.9 2001/12/11 20:44:23 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_netsession_server
#define header_netsession_server

#include <list>
#include <queue>
#include <string>
#include <map>
#include "API/Core/System/thread.h"
#include "API/Network/netgroup.h"
#include "netsession_generic.h"
#include "netcomputer_generic.h"

class CL_NetComputer_Host;
class CL_NetSession_Server;
class CL_Connection;
class CL_UDPConnection;
class CL_Mutex;

class CL_NetComputer_Host : public CL_NetComputer_Generic
{
public:
	CL_NetComputer_Host(CL_NetSession_Server *session, CL_Connection *connection, int id);
	virtual ~CL_NetComputer_Host();

	virtual unsigned long get_address() const;
	virtual unsigned short get_port() const;
	virtual void disconnect();

	int id;
	CL_Connection *connection;
};

class CL_NetChannelQueue : public std::queue<CL_NetMessage>
{
public:
	std::map<int, int> access;
};


class CL_NetSession_Server : public CL_NetSession_Generic, CL_Runnable
{
// Construction:
public:
	CL_NetSession_Server(
		CL_ConnectionProvider *provider,
		const std::string &game_id,
		int port);

	virtual ~CL_NetSession_Server();

// Attributes:
public:
	virtual CL_NetComputer &get_server();

	virtual CL_NetGroup &get_all();

	virtual bool peek(int channel) const;

	virtual int access_status(int channel) const;

	virtual bool is_writable(int channel) const;

	virtual bool is_readable(int channel) const;

// Operations:
public:
	virtual void keep_alive();

	virtual CL_NetMessage receive(
		int channel,
		int timeout=-1);

	void send(
		const int dest_channel,
		const CL_NetComputer &dest,
		const CL_NetMessage &message,
		bool reliable = true);

	virtual void send(
		const int dest_channel,
		const CL_NetGroup &dest,
		const CL_NetMessage &message,
		bool reliable = true);

	virtual CL_NetComputer receive_computer_leave();

	virtual CL_NetComputer receive_computer_join();

	virtual CL_NetComputer receive_computer_rejoin();

	virtual bool receive_session_closed();

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
		
private: // implementation helper functions:
	CL_NetComputer_Host *get_computer_host(const CL_NetComputer &comp)
	{ return static_cast<CL_NetComputer_Host *>(comp.impl); }
	// Casts comp.impl to CL_NetComputer_Host and returns it.

	CL_NetChannelQueue *find_queue(int netchannel) const;
	// Returns the queue of the specified netchannel. Returns NULL if no queue exist.

	CL_NetChannelQueue *create_queue(int netchannel);
	// Returns the queue of the specified netchannel. Creates a new one if none exist.

	bool has_read_access(
		CL_NetChannelQueue *channel,
		CL_NetComputer_Host *host);

	void check_trigger();
	// Resets the trigger if there is no new data pending.

private: // data vars
	CL_NetGroup all;
	std::list<CL_NetComputer> prejoin;
	std::list<CL_NetComputer> computers;

	std::map<int /*netchannel*/,CL_NetChannelQueue> netchannels;
	
	CL_UDPConnection *udp_connection;

	int id_counter;
	std::string game_id;
	int port;
	
	std::queue<CL_NetComputer> join_queue;
	std::queue<CL_NetComputer> rejoin_queue;
	std::queue<CL_NetComputer> leave_queue;

private: // multithread:
	CL_Mutex *mutex;
	CL_Thread *thread;
	volatile bool exit_thread;

	virtual void run();

friend class CL_NetComputer_Host;
};


#endif
