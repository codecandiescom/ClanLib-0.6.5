
#ifndef header_netcomputer_generic
#define header_netcomputer_generic

#include "API/Core/System/mutex.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/event_trigger.h"
#include "API/Network2/Socket/socket.h"
#include "API/Network2/NetSession/netpacket.h"

#include <map>
#include <string>
#include <queue>

class CL_NetStream_Generic;
class CL_NetSession_Generic;
class CL_NetPacket_Generic;
class CL_EventListener;

class CL_NetComputer_Generic
{
//! Construction:
public:
	//: Sync connect constructor.
	CL_NetComputer_Generic(const CL_Socket &sock, CL_NetSession_Generic *netsession);

	//: ASync connect constructor.
	CL_NetComputer_Generic(const std::string &hostname, const std::string &port, CL_NetSession_Generic *netsession);

	virtual ~CL_NetComputer_Generic();

//! Attributes:
public:
	//: Return current reference count.
	int get_ref() const;

	//: Socket connection to the network computer.
	CL_Socket sock;

	//; Cached dest address of remote computer.
	//: (we cache it because it seems linux forget about the dest peer when a
	//: socket is closed)
	CL_IPAddress address;

	//: True if computer has disconnected.
	bool disconnected;

	//: String describing reason why computer was disconnected.
	std::string disconnect_reason;

	//: Pointer to netsession owner of netcomputer.
	CL_NetSession_Generic *netsession;

	//: Syncronized data access to netcomputer.
	CL_Mutex mutex;

//! Operations:
public:
	//: Increase reference count.
	void add_ref();

	//: Decrease reference count. Delete object if reaching zero.
	int release_ref();

	//: Send packet to computer.
	void send_packet(const std::string &packet_channel, CL_NetPacket_Generic *packet);
	
	//: Send stream connect packet to computer. Returns the channel_id of the new connection.
	int send_stream_connect(const std::string &stream_channel, CL_NetStream_Generic *stream);

	//: Send stream message to computer.
	void send_stream_message(unsigned int channel_id, const std::string &data);
	
	//: Send stream close message to computer.
	void send_stream_close(int channel_id);

//! Implementation:
private:
	//: Worker thread for this netcomputer. Basically calls update_socket() on activity.
	void worker_thread();

	//: Updates socket. Dispatches new available data and writes queued data to socket.
	void update_socket();

	//: Bytes left of current data transfer chunk.
	int data_left;

	//: Netstream receiving current data transfer.
	CL_NetStream_Generic *netstream;

	//: Netpacket receiving current data transfer.
	CL_NetPacket netpacket;

	//: Netpacket destination for current data transfer.
	int dest_netpacket_atom;

	//: Atom map used to map from strings to atom ints.
	std::map<std::string, int> local_atoms;

	//: Next free local atom.
	int local_atom_counter;

	//: Atom map used to map from atom ints to strings.
	std::map<int, std::string> remote_atoms;

	//: Queue of messages to be sent across socket.
	std::queue<std::string> send_queue;

	//: Data sent position in topmost send queue message.
	int send_pos;

	//: Reference counter.
	int ref;

	//: Basic message types used over socket.
	enum
	{
		cmd_create_atom       = 0,
		cmd_netstream_connect = 32,
		cmd_netstream_msg,
		cmd_netstream_closed,
		cmd_netpacket_msg     = 64,
	};

	//: Asyncronous connect. If true, the worker thread will do the socket
	//: connect. Otherwise the connect has already been done from the main
	//: thread.
	bool async_boot;

	//: Asyncronous connect. Hostname to connect to.
	std::string async_hostname;

	//: Asyncrounous connect. Port used in connect.
	std::string async_port;

	//: Worker thread handle.
	CL_Thread thread;

	//: Trigger that will wake up sockets worker thread.
	CL_EventTrigger wakeup_trigger;

	//: List of channel connections currently active over this connection.
	std::map<int, CL_NetStream_Generic *> streams;
	
	//: Local stream channel ID counter.
	int channel_id_counter;
	
	//: If true, worker thread is asked to shut itself down.
	volatile bool shutdown;
};

#endif
