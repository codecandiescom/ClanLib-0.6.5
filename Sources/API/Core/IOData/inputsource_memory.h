/*
	$Id: inputsource_memory.h,v 1.10 2001/09/22 15:52:10 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanCore="I/O Data"
//! header=core.h

#ifndef header_inputsource_memory
#define header_inputsource_memory

#include "inputsource.h"

//: Interface to read data from a memory source.
//- CL_InputSource_Memory is used to read data from a memory source.
//- CL_InputSourceProvider - Interface to open input sources with.
class CL_InputSource_Memory : public CL_InputSource
{
//! Construction:
public:
	//: Input Source Memory Constructor
	CL_InputSource_Memory(std::string data);

	//: Input Source Memory Destructor
	virtual ~CL_InputSource_Memory();

//! Attributes:
public:
	//: Returns current position in input source.
	//- Returns - Current position in input source.
	virtual int tell() const;

	//: Returns the size of the input source
	//- Returns - Size of the input source.
	virtual int size() const;

	//: Get the pointer to the internal data array in the memory source.
	//: Used to extract the data after usage.
	//- Returns - Pointer to the data written.
	std::string get_data() const;

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

	//: Pushes the current input source position.
	//: The position can be restored again with pop_position.
	virtual void push_position();
	
	//: Pops a previous pushed input source position (returns to the position).
	virtual void pop_position();

//! Implementation:
private:
	std::string data;
	unsigned int pos;
	bool delete_data;
};

#endif
