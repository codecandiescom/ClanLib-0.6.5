/*
	$Id: outputsource_memory.h,v 1.12 2001/09/22 15:52:10 plasmoid Exp $

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

#ifndef header_outputsource_memory
#define header_outputsource_memory

#include "outputsource.h"

//: Outputsource writing to memory.
class CL_OutputSource_Memory : public CL_OutputSource
{
//! Construction:
public:
	//: Output Source Memory Constructor
	CL_OutputSource_Memory();

	//: Output Source Memory Destructor
	virtual ~CL_OutputSource_Memory();

//! Attributes:
public:
	//: Returns current position in output source.
	//- Returns - Current position in source.
	virtual int tell() const;

	//: Returns the size of the output source
	//- Returns - Size of the output source.
	virtual int size() const;

	//: Get the pointer to the internal data array in the memory source.
	//: Used to extract the data after usage.
	//- Returns - Pointer to the data written.
	std::string get_data() const;

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
	std::string data;
	unsigned int pos;
};

#endif
