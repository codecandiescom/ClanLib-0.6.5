/*
	$Id: outputsource_file.h,v 1.9 2001/12/11 20:44:18 mbn Exp $

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

#ifndef header_outputsource_file
#define header_outputsource_file

#include "outputsource.h"

#include <string>
#include <stdio.h>

//: Outputsource writing to file.
class CL_OutputSource_File : public CL_OutputSource
{
//! Construction:
public:
	//: Output Source File Constructor
	CL_OutputSource_File(const std::string &file);

	//: Output Source File Constructor
	CL_OutputSource_File();

	//: Output Source File Destructor
	virtual ~CL_OutputSource_File();

//! Attributes:
public:
	//: Returns current position in output source.
	//- Returns - Current position in source.
	virtual int tell() const;

	//: Returns the size of the output source
	//- Returns - Current size of output source.
	virtual int size() const;

	//: Get the pointer to the internal data array in the memory source.
	//: Used to extract the data after usage.
	//- Returns - Pointer to the data written.
	std::string get_data() const;

//! Operations:
public:
	//: Writes larger amounts of data (no endian and 64 bit conversion):
	//- data - Points to the array written.
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
	unsigned int pos;
	FILE *file;
	std::string filename;
};

#endif
