/*
	$Id: inputsource_netstream.h,v 1.1 2002/11/02 19:46:01 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanNetwork="NetSessions"
//! header=network.h

#ifndef header_inputsource_netstream
#define header_inputsource_netstream

#include "../../Core/IOData/inputsource.h"

class CL_NetStream;
class CL_NetStream_Generic;

//: InputSource NetStream Class
class CL_InputSource_NetStream : public CL_InputSource
{
//! Construction:
public:
	//: InputSource Netstream Constructor
	CL_InputSource_NetStream(const CL_NetStream &netstream);

	virtual ~CL_InputSource_NetStream();

//! Attributes:
public:
	//: Returns current position in input source.
	//return: Current position in input source.
	virtual int tell() const;

	//: Returns the size of the input source
	//return: Size of the input source.
	virtual int size() const;

//! Operations:
public:
	//: Reads larger amounts of data (no endian and 64 bit conversion).
	//param data: Points to an array where the read data is stored.
	//param size: Number of bytes to read.
	//return: Num bytes actually read.
	virtual int read(void *data, int size);
	
	//: Opens the input source. By default, it is open.
	virtual void open();
	
	//: Closes the input source.
	virtual void close();

	//: Make a copy of the current inputsource, standing at the same position.
	//return: The copy of the input source.
	virtual CL_InputSource *clone() const;

	//: Seeks to the specified position in the input source.
	//param pos: Position relative to 'seek_type'.
	//param seek_type: Defines what the 'pos' is relative to. Can be either seek_set, seek_cur og seek_end.
	virtual void seek(int pos, SeekEnum seek_type);

	//: Pushes the current input source position. The position can be restored again with pop_position.
	virtual void push_position();
	
	//: Pops a previous pushed input source position (returns to the position).
	virtual void pop_position();

//! Implementation:
private:
	friend class CL_NetStream;

	// Constructor used to initialize internal CL_NetStream member variable.
	CL_InputSource_NetStream() { netstream = 0; ref_count_netstream = false; }

	CL_NetStream_Generic *netstream;
	bool ref_count_netstream;
};

#endif
