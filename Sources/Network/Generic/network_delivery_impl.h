/*
	$Id: network_delivery_impl.h,v 1.3 2001/11/01 20:27:57 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Contains interfaces to the implementation dependent code.

*/

//! layer="transport"

#ifndef header_network_delivery_impl
#define header_network_delivery_impl

class CL_ConnectionPacket
//: A packet sent through a CL_Connection.
//!desc: The basic packet type sent 
{
public:
	int size;
	//: Size of the packet data.

	void *data;
	//: The packet data.

	CL_ConnectionPacket() { data = NULL; }
	//: Default constructor

	CL_ConnectionPacket(void *data, int size)
	{
		this->data = data;
		this->size = size;
	}

};

class CL_UDPConnectionPacket
{
public:
	unsigned int ip_addr;
	//: IP address the packet should be sent to - or was received from.

	unsigned int port;
	//: udp port the packet should be sent to - or was received from.
	
	int size;
	//: Size of the packet data.

	void *data;
	//: The packet data.
};

class CL_Connection
//: Class representing a connection to a host or a local process.
//!desc: This class is an abstract interface used to send messages to either 
//!desc: a host (using sockets) or a local process (using pipes).
//!desc: <br>
//!desc: CL_Connection is inherited to platform dependent connection types 
//!desc: such as TCP/IP sockets and pipes. These platform dependent 
//!desc: connections can be obtained through the CL_ConnectionProvider 
//!desc: interface.
{
public:
	virtual ~CL_Connection() {;}

	virtual bool peek()=0;
	//: Returns true if a message is available - but doesn't retrieve it.
	//!retval: True if a message can be received by a call to receive().

	virtual CL_ConnectionPacket receive()=0;
	//: Gets the next net message available from this connection.

	virtual void send(CL_ConnectionPacket message)=0;
	//: Sends some data to the connection.
	//!param: message - Message/data to be sent.

	virtual bool connection_lost()=0;
	//: If the connection is lost in the other end (eg. socket closed), this 
	//: function will return true, otherwise false.
	//!retval: Returns true if connection is lost.

	virtual unsigned long get_peer_address()=0;
	//: Returns the IP address of the peer.

	virtual unsigned short get_peer_port()=0;
	//: Returns the port of the peer.

	virtual void disconnect()=0;
	//: Disconnects connection.
};

class CL_UDPConnection
//: Class representing a UDP socket connection.
//!desc: This class is an abstract interface used to send messages to another
//!desc: host on a UDP port.
{
public:
	virtual bool peek()=0;
	//: Returns true if a message is available - but doesn't retrieve it.
	//!retval: True if a message can received by a call to receive().

	virtual CL_UDPConnectionPacket receive()=0;
	//: Gets the next net message available from this connection.

	virtual void send(CL_UDPConnectionPacket message)=0;
	//: Sends some data from the connection.
	//!param: message - Message/data to be sent.

	virtual void broadcast(CL_UDPConnectionPacket message)=0;
	//: Broadcasts some data from the connection.
	//!param: message - Message/data to be sent.

	virtual unsigned int get_port()=0;
	//: Returns the udp port that this connection listens at.
	//!retval: UDP port used by connection.
};

class CL_Mutex;
class CL_ConnectionProvider
//: Interface to platform dependent connections.
//!desc: This class contain all the platform dependent functions in the 
//!desc: transport layer.
//!desc: <br>
//!desc: This interface is used to create connections to other hosts and the 
//!desc: netsession layer.
{
public:
	virtual ~CL_ConnectionProvider() {;}
	
	virtual CL_UDPConnection *create_udp_connection(unsigned int port)=0;
	//: Creates a udp connection that listens on the udp port 'port'.
	//!param: port - UDP port to listen on (in host byte order).
	//!retval: The created udp connection.

	virtual CL_Connection *get_client()=0;
	//: Returns the connection to the NetSession layer.
	//!retval: Connection to the NetSession layer.

	virtual CL_Connection *create_tcp_connection(
		int ip_addr,
		int port)=0;
	//: Connects to the specified IP address and port number.
	//!param: ip_addr - The IP address in network byte order.
	//!param: port - Port number on that host (in host byte order).
	//!retval: Returns the connection to the host.

	virtual CL_Connection *accept()=0;
	//: Returns a pointer to new connections when someone connects to us.
	//: This function must be implemented as a nonblocking function, which
	//: only accepts on a socket if a peek determines that a connection is waiting -
	//: otherwise, the function must return NULL.
	//!retval: The new connection that appeared.

	virtual void start_accept_on_port(int port)=0;
	//: Accept new connections on the specified port.
	//!param: port - Port connections should be accepted on (in host byte order).

	virtual void stop_accept_on_port(int port)=0;
	//: Stop accepting new connections on the specified port.
	//!param: port - Port to stop accepts (in host byte order).
	
	virtual void wait_for_connection_data(CL_Mutex *mutex)=0;
	//: This function will sleep until one of the connections has any new data
	//: available, or if a connection changes state (eg. a connection is lost).
	//: This is a platform independent way of doing a select() on all the
	//: connection file descriptors.

	virtual void remove_connection(CL_Connection *removed_connection)=0;
	//: This function is called by the generic delivery service, when a connection
	//: is about to be deleted following a connection-lost. The connection provider
	//: should remove any references to 'removed_connection' as it will be deleted.
};

#endif
