/*
	$Id: outputsource_socket.h,v 1.8 2001/11/01 19:19:55 sphair Exp $

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

#ifndef header_outputsource_socket
#define header_outputsource_socket

#include "../Core/IOData/outputsource.h"

class CL_Socket;
class CL_Socket_Generic;

//: Output Source Socket Class
class CL_OutputSource_Socket : public CL_OutputSource
{
//! Construction:
public:
	//: Output Source Socket Constructor
	CL_OutputSource_Socket(const CL_Socket &socket);

	virtual ~CL_OutputSource_Socket();

//! Attributes:
public:
	//: Returns current position in output source.
	//- Returns - Current position in source.
	virtual int tell() const;

	//: Returns the size of the output source
	//- Returns - Size of the output source.
	virtual int size() const;

//! Operations:
public:
	//: Writes larger amounts of data (no endian and 64 bit conversion):
	//- data - Points to the array from which to write.
	//- size - Number of bytes to write.
	//- Returns - Num bytes actually written.
	virtual int write(const void *data, int size);
	
	//: Opens the output source. By default, it is open.
	virtual void open();
	
	//: Closes the output source.
	virtual void close();

	//: Make a copy of the current outputsource, standing at the same position.
	//- Returns - The copy of the output source.
	virtual CL_OutputSource *clone();

//! Implementation:
private:
	friend class CL_Socket;

	// Constructor used to initialize internal CL_Socket member variable.
	CL_OutputSource_Socket() { socket = 0; ref_count_socket = false; }

	CL_Socket_Generic *socket;
	bool ref_count_socket;
};

#endif
