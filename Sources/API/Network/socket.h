/*
	$Id: socket.h,v 1.21 2001/10/15 09:37:43 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanNetwork="Sockets"
//! header=network.h

#ifndef header_socket
#define header_socket

#include "../signals.h"
#include "ip_address.h"
#include "inputsource_socket.h"
#include "outputsource_socket.h"

class CL_EventTrigger;
class CL_Socket_Generic;

//: This is the lowlevel raw socket interface in clanlib.
//- <p>Its main purpose is to write the C sockets API, providing error handling 
//- via exceptions, and socket waiting (select) via the clanlib event waiting 
//- system (CL_EventListener, CL_EventTrigger).</p>
class CL_Socket
{
public:
	//: Type enum
	enum Type
	{
		tcp,
		udp
	};

	//: ShutdownHow enum
	enum ShutdownHow
	{
		shutdown_receive,
		shutdown_send
	};

//! Construction:
public:
	//: Constructs a null socket.
	CL_Socket();

	//: Constructs an attached socket.
	CL_Socket(int socket);

	//: Constructs a new socket using the specified protocol type.
	CL_Socket(Type type);

	//: Copy constructor.
	CL_Socket(const CL_Socket &copy);

	//: Socket destructor
	virtual ~CL_Socket();

//! Attributes:
public:
	//: Input source that operates on the socket.
	CL_InputSource_Socket input;

	//: Output source that operates on the socket.
	CL_OutputSource_Socket output;

	//: Returns the raw socket handle (for use with the lowlevel socket functions).
	int get_socket() const;

	//: Returns an event trigger that triggers when socket has data ready to be read.
	CL_EventTrigger *get_read_trigger() const;

	//: Returns an event trigger that triggers when socket is ready for additional data.
	CL_EventTrigger *get_write_trigger() const;

	//: Returns an event trigger that triggers when an exception occurs on the socket.
	CL_EventTrigger *get_exception_trigger() const;

	//: Returns the socket name.
	CL_IPAddress get_source_address() const;

	//: Returns the name of the destination socket we are connected to.
	CL_IPAddress get_dest_address() const;

	//: This signal is invoked when the socket has data ready to be read.
	CL_Signal_v0 &sig_read_triggered();

	//: This signal is invoked when the socket has data ready to be written.
	CL_Signal_v0 &sig_write_triggered();

	//: This signal is invoked when an exception occurred on the socket.
	CL_Signal_v0 &sig_exception_triggered();

//! Operations:
public:
	//: Copy operator
	CL_Socket &operator =(const CL_Socket &other);

	//: Sets the socket blocking mode.
	void set_nonblocking(bool nonblocking = true);

	//: If enabled, don't delay send to coalesce packets.
	void set_nodelay(bool nodelay = true);

	//: Writes data to socket. Returns the amount that was written.
	int send(const void *data, int size);

	//: Writes data to socket, using the specified destination host.
	int send(const void *data, int size, const CL_IPAddress &dest);

	int send(const std::string &string);
	//!operations:
	// Writes data to socket. Returns the amount that was written.

	void push(const std::string &string);
	//!operations:
	// Push data back into the received data buffer.

	//: Reads data from the socket. Returns the amount that was read.
	int recv(void *data, int size);

	//: Reads data from the socket, storing the from address in the passed parameter.
	int recv(void *data, int size, CL_IPAddress &from);

	//: Initiate a connection on the socket.
	void connect(const CL_IPAddress &address);

	//: Shut down part of full-duplex connection.
	void shutdown(ShutdownHow how);

	//: Bind the socket to the specified address.
	void bind(const CL_IPAddress &address);

	//: Listen for connections on the socket.
	void listen(int backlog);

	//: Accept a connection on the socket.
	CL_Socket accept();

//! Implementation:
public:
	CL_Socket(class CL_Socket_Generic *impl);
	CL_Socket_Generic *impl;
};

#endif
