
#ifndef header_netsession_generic
#define header_netsession_generic

#ifdef WIN32
#pragma warning(disable : 4786)
#endif

#include <string>
#include <map>
#include <list>
#include <queue>
#include "API/signals.h"
#include "API/Core/System/mutex.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/event_trigger.h"
#include "API/Core/System/keep_alive.h"
#include "API/Network2/NetSession/netcomputer.h"
#include "API/Network2/NetSession/netgroup.h"
#include "API/Network2/NetSession/netpacket.h"
#include "API/Network2/Socket/socket.h"

class CL_NetStream;
class CL_NetStream_Generic;
class CL_NetComputer_Generic;

class CL_NetSession_Generic : public CL_KeepAlive
{
//! Construction:
public:
	CL_NetSession_Generic(const std::string &app_id);

	virtual ~CL_NetSession_Generic();

//! Attributes:
public:
	CL_Signal_v1<CL_NetComputer &> sig_computer_connected;

	CL_Signal_v1<CL_NetComputer &> sig_computer_reconnected;

	CL_Signal_v1<CL_NetComputer &> sig_computer_disconnected;

	std::map< std::string, CL_Signal_v1<CL_NetStream &> > map_netstream_connect;

	std::map< std::string, CL_Signal_v2<CL_NetPacket &, CL_NetComputer &> > map_netpacket_receive;

	//: List of all computers, after the connect/disconnect signals have been emitted for them.
	CL_NetGroup all;

	//: List of connected computers.
	std::list<CL_NetComputer_Generic *> computers;

	//: Application Identifier.
	std::string app_id;

	//: Packet and Computer received pair.
	typedef std::pair<CL_NetPacket, CL_NetComputer> PacketCompPair;

	//: List of received netpackets on each channel.
	std::map< std::string, std::queue<PacketCompPair> > received_netpackets;
	
	//: NetStream and channel name pair.
	typedef std::pair<CL_NetStream_Generic *, std::string> NewStreamPair;
	
	//: List of incoming stream connection to be signalled.
	std::queue<NewStreamPair> new_streams;

	//: List of incoming connections to be signalled:
	std::queue<CL_NetComputer> new_connections;

	//: List of incoming connections to be signalled:
	std::queue<CL_NetComputer> reconnections;

	//: List of incoming disconnections to be signalled:
	std::queue<CL_NetComputer> disconnections;

	//: Syncronized access to member variables.
	CL_Mutex mutex;

	//: Show debug output if this is enabled.
	bool show_debug;

//! Operations:
public:
	void add_ref();

	int release_ref();

	CL_NetComputer connect(const CL_IPAddress &address);

	CL_NetComputer connect_async(const std::string &hostname, const std::string &port);

	void start_listen(const std::string &port);

	void stop_listen();

	//: Called by CL_NetComputer_Generic's socket thread when a netpacket is received.
	void received_netpacket(const std::string &channel, const CL_NetPacket &packet, CL_NetComputer_Generic *comp);

//! Implementation:
private:
	virtual void keep_alive();

	void boot();

	void shutdown();

	void accept_thread();

	CL_Socket accept_socket;

	CL_Thread listen_thread;

	CL_EventTrigger accept_shutdown_trigger;

	// Reference counter.
	int ref;
};

#endif
