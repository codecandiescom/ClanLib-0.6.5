/*
	$Id: inputsource_memory_generic.h,v 1.7 2001/12/11 20:44:21 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_inputsource_memory_generic
#define header_inputsource_memory_generic

#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"

class CL_OutputSource_MemoryGeneric;

class CL_InputSource_MemoryGeneric : public CL_InputSource
//: Interface to read data from a memory_generic source.
// CL_InputSource_MemoryGeneric is used to read data from a memory_generic source.
//!also: CL_InputSourceProvider - Interface to open input sources with.
{
public:
	CL_InputSource_MemoryGeneric(void *data, int size, bool delete_data = false);

	CL_InputSource_MemoryGeneric(const CL_InputSource_MemoryGeneric *MG);
	//: MemoryGeneric copy constructor.
	// Makes a seperate copy of the memory in MG.
	//!also: MemoryGeneric::clone.
	//!param: MG - Pointer to the MemoryGeneric object from which to copy.

	virtual ~CL_InputSource_MemoryGeneric();

	virtual int read(void *data, int size);
	//: Reads larger amounts of data (no endian and 64 bit conversion).
	//!param: data - Points to an array where the read data is stored.
	//!param: size - Number of bytes to read.
	//!retval: Num bytes actually read.
	
	virtual void open();
	//: Opens the input source. By default, it is open.
	
	virtual void close();
	//: Closes the input source.

	virtual CL_InputSource *clone() const;
	//: Make a copy of the current InputSource, standing at the same position.
	//!retval: The clone of the input source.

	virtual int tell() const;
	//: Returns current position in input source.
	//!retval: Current position in input source.

	virtual void seek(int pos, SeekEnum seek_type);
	//: Seeks to the specified position in the input source.
	//!param: pos - Position relative to 'seek_type'.
	//!param: seek_type - Defines what the 'pos' is relative to. Can be either seek_set, seek_cur og seek_end.

	virtual int size() const;
	//: Returns the size of the input source
	//!retval: Size of the input source.

	virtual void push_position();
	//: Pushes the current input source position. The position can be restored again with pop_position.
	
	virtual void pop_position();
	//: Pops a previous pushed input source position (returns to the position).

	virtual void purge();
	//: Purges the input buffer of data without deleting the buffer

private:
	unsigned char *data;
	unsigned int pos, length;
	bool delete_data;
};

class CL_InputSourceProvider_Memory : public CL_InputSourceProvider
{
public:
	CL_InputSourceProvider_Memory(unsigned char *data, unsigned int size, bool delete_data);
	CL_InputSourceProvider_Memory(CL_InputSource_MemoryGeneric *MG);
	CL_InputSourceProvider_Memory(CL_OutputSource_MemoryGeneric *MG);

	virtual CL_InputSource *open_source(const std::string &handle);
	virtual CL_InputSourceProvider *clone();

private:
	unsigned char *data;
	unsigned int size;
	bool delete_data;
};

#endif

