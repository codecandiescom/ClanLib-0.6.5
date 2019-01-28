/*
	$Id: outputsource.h,v 1.13 2001/09/22 15:52:10 plasmoid Exp $

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

#ifndef header_outputsource
#define header_outputsource

#include <string>

//: Interface to write data to different types of output sources.
//- <p>CL_OutputSource is used to write data to different types of input sources.
//- In most cases, this is regular files and ClanLib zlib compressed datafiles.</p>
//- 
//- <p>To access some system specific output source types, such as datafiles and
//- regular ones, use the static create functions in the CL_OutputSourceProvider
//- class.</p>
//- CL_OutputSourceProvider - Interface to open output sources with.
class CL_OutputSource
{
//! Construction:
public:
	//: Output Source Constructor
	CL_OutputSource() { set_system_mode(); }

	//: Output Source Destructor
	virtual ~CL_OutputSource() { return; }

//! Attributes:
public:
	//: Returns current position in output source.
	//- Returns - Current position in source.
	virtual int tell() const = 0;

	//: Returns the size of the output source
	//- Returns - Size of the output source.
	virtual int size() const = 0;

//! Operations:
public:
	//: Changes output data endianess to the local systems mode.
	virtual void set_system_mode();

	//: Changes output data endianess to big endian mode.
	virtual void set_big_endian_mode();

	//: Changes output data endianess to little endian mode. This is the default setting.
	virtual void set_little_endian_mode();

	//: Writes a signed 32 bit integer to output source.
	virtual void write_int32(int data);

	//: Writes an unsigned 32 bit integer to output source.
	virtual void write_uint32(unsigned int data);

	//: Writes a signed 16 bit integer (short) to output source.
	virtual void write_short16(short data);

	//: Writes an unsigned 16 bit integer (short) to output source.
	virtual void write_ushort16(unsigned short data);

	//: Writes a signed 8 bit integer (char) to output source.
	virtual void write_char8(char data);

	//: Writes an unsigned 8 bit integer (char) to output source.
	virtual void write_uchar8(unsigned char data);

	//: Writes an ieee format floating point to output source.
	virtual void write_float32(float data);

	//: Writes a bool to output source.
	virtual void write_bool8(bool data);

	//: Writes a string to the output source.
	//: <p>The binary format written to the output source is first an uint32 telling the length of the
	//: string, and then the string itself.</p>
	virtual void write_string(const std::string &str);

	//: Writes larger amounts of data (no endian and 64 bit conversion):
	//- data - Points to the array from which to write.
	//- size - Number of bytes to write.
	//- Returns - Num bytes actually written.
	virtual int write(const void *data, int size) = 0;
	
	//: Opens the output source. By default, it is open.
	virtual void open() = 0;
	
	//: Closes the output source.
	virtual void close() = 0;

	//: Make a copy of the current outputsource, standing at the same position.
	//- Returns - The copy of the output source.
	virtual CL_OutputSource *clone() = 0;

//! Implementation:
protected:
	bool little_endian_mode;
};

#endif
