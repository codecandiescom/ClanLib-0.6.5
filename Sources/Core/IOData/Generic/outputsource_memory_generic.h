/*
	$Id: outputsource_memory_generic.h,v 1.4 2001/04/29 17:04:28 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_outputsource_memory_generic
#define header_outputsource_memory_generic

#include "API/Core/IOData/outputsource.h"

class CL_OutputSource_MemoryGeneric : public CL_OutputSource
//: Outputsource writing to memory_generic.
{
public:
	CL_OutputSource_MemoryGeneric(unsigned int blocksize = 4*1024);
	virtual ~CL_OutputSource_MemoryGeneric();
	
	void *get_data() const;
	//: Get the pointer to the internal data array in the memory_generic source.
	//: Used to extract the data after usage.
	//!retval: Pointer to the data written.

	virtual int write(const void *data, int size);
	//: Writes larger amounts of data (no endian and 64 bit conversion):
	//: <br>
	//!param: data - Points to the array written.
	//!param: size - Number of bytes to write.
	//!retval: Num bytes actually written.
	
	virtual void open();
	//: Opens the output source. By default, it is open.
	
	virtual void close();
	//: Closes the output source.

	virtual CL_OutputSource *clone();
	//: Make a copy of the current OutputSource, standing at the same position.
	//!retval: The copy of the output source.

	virtual int tell() const;
	//: Returns current position in output source.
	//!retval: Current position in source.

	virtual int size() const;
	//: Returns the size of the output source
	//!retval: Current size of output source.

	virtual void purge();
	//: Cleans out the databuffer without deleting it.

private:
	unsigned int m_blocksize;
	unsigned char *m_data;
	unsigned int m_size, m_pos;
};

#endif
