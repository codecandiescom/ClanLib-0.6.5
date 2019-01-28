/*
	$Id: buffered_socket.h,v 1.13 2001/10/10 11:40:52 sphair Exp $

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

#ifndef header_buffered_socket
#define header_buffered_socket

#include "socket.h"

class CL_BufferedSocket_Generic;

//: Non-blocked, buffered socket transmission interface.
//- The buffered socket class works like a socket in non-blocking mode, with a few exceptions:
//- 
//- <p>any write operation will always write the full data. Any data not immidiately writeable
//- to the socket will be buffered and written in a seperate thread as the socket can receive
//- them. When all the data has been written, the write event will trigger.</p>
//- 
//- <p>incoming data is also buffered. Each time the input buffer has been filled (which
//- can be changed with set_input_buffer_size(int size), the read event will trigger.</p>
//- 
//- Using this class, you can easilly write and read data in the chunk sizes you prefer.
class CL_BufferedSocket : public CL_Socket
{
//! Construction:
public:
	//: Constructs an attached socket.
	CL_BufferedSocket(int socket);

	//: Constructs a new socket using the specified protocol type.
	CL_BufferedSocket(Type type);

	//: Create socket that is attached to the passed socket.
	//: <p>Note that since the buffered socket class will read data from the socket (buffering it),
	//: it will no longer be safe to use the original socket until the buffered socket instance
	//: has been destroyed.</p>
	CL_BufferedSocket(const CL_Socket &socket);

	//: Copy constructor.
	CL_BufferedSocket(const CL_BufferedSocket &copy);

	//: Buffered Socket Destructor
	virtual ~CL_BufferedSocket();

//! Attributes:
public:
	//: <p>Returns the size of the buffered input buffer. Each time it is full,
	//: the read event will trigger.</p>
	int get_input_buffer_size() const;

//! Operations:
public:
	//: <p>Sets the size of the buffered input buffer. Each time it is full,
	//: the read event will trigger.</p>
	void set_input_buffer_size(int size);

//! Implementation:
private:
	CL_BufferedSocket_Generic *impl;
};

#endif
