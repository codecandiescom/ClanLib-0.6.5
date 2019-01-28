/*
	$Id: inputsource_socket.h,v 1.8 2001/11/01 19:19:55 sphair Exp $

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

#ifndef header_inputsource_socket
#define header_inputsource_socket

#include "../Core/IOData/inputsource.h"

class CL_Socket;
class CL_Socket_Generic;

//: InputSource Socket Class
class CL_InputSource_Socket : public CL_InputSource
{
//! Construction:
public:
	//: InputSource Socket Constructor
	CL_InputSource_Socket(const CL_Socket &socket);

	virtual ~CL_InputSource_Socket();

//! Attributes:
public:
	//: Returns current position in input source.
	//- Returns - Current position in input source.
	virtual int tell() const;

	//: Returns the size of the input source
	//- Returns - Size of the input source.
	virtual int size() const;

//! Operations:
public:
	//: Reads larger amounts of data (no endian and 64 bit conversion).
	//- data - Points to an array where the read data is stored.
	//- size - Number of bytes to read.
	//- Returns - Num bytes actually read.
	virtual int read(void *data, int size);
	
	//: Opens the input source. By default, it is open.
	virtual void open();
	
	//: Closes the input source.
	virtual void close();

	//: Make a copy of the current inputsource, standing at the same position.
	//- Returns - The copy of the input source.
	virtual CL_InputSource *clone() const;

	//: Seeks to the specified position in the input source.
	//- pos - Position relative to 'seek_type'.
	//- seek_type - Defines what the 'pos' is relative to. Can be either seek_set, seek_cur og seek_end.
	virtual void seek(int pos, SeekEnum seek_type);

	//: Pushes the current input source position. The position can be restored again with pop_position.
	virtual void push_position();
	
	//: Pops a previous pushed input source position (returns to the position).
	virtual void pop_position();

//! Implementation:
private:
	friend class CL_Socket;

	// Constructor used to initialize internal CL_Socket member variable.
	CL_InputSource_Socket() { socket = 0; ref_count_socket = false; }

	CL_Socket_Generic *socket;
	bool ref_count_socket;
};

#endif
