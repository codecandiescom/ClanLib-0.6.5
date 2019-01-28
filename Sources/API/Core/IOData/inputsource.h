/*
	$Id: inputsource.h,v 1.12 2001/09/22 15:52:10 plasmoid Exp $

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

#ifndef header_inputsource
#define header_inputsource

#include <string>

//: Interface to read data.
//- CL_InputSource is used to read data from different types of input sources.
//- In most cases, this is regular files and ClanLib zlib compressed datafiles.
//- 
//- <p>To access some system specific input source types, such as datafiles and
//- regular ones, use the static create functions in the CL_InputSourceProvider
//- class.</p>
//- CL_InputSourceProvider - Interface to open input sources with.
class CL_InputSource
{
//! Enums:
public:
	//: Enum used by seek()
	enum SeekEnum
	{
		seek_set,	 // from beginning of index
		seek_cur,	 // from current position
		seek_end	 // from end of index
	};

//! Construction:
public:
	//: Input Source Constructor
	CL_InputSource() { set_system_mode(); }

	//: Input Source Destructor
	virtual ~CL_InputSource() { return; }

//! Attributes:
public:
	//: Returns current position in input source.
	//- Returns - Current position in input source.
	virtual int tell() const = 0;

	//: Returns the size of the input source
	//- Returns - Size of the input source.
	virtual int size() const = 0;

	//: Returns true if the input source is in little endian mode.
	virtual bool is_little_endian() { return little_endian_mode; }

//! Operations:
public:
	//: Changes input data endianess to the local systems mode.
	virtual void set_system_mode();

	//: Changes input data endianess to big endian mode.
	virtual void set_big_endian_mode();

	//: Changes input data endianess to little endian mode. This is the default setting.
	virtual void set_little_endian_mode();

	//: Reads a signed 32 bit integer from input source.
	//- Returns - The integer read.
	virtual int read_int32();

	//: Reads an unsigned 32 bit integer from input source.
	//- Returns - The unsigned integer read.
	virtual unsigned int read_uint32();

	//: Reads a signed 16 bit integer (short) from input source.
	//- Returns - The short read.
	virtual short read_short16();

	//: Reads an unsigned 16 bit integer (short) from input source.
	//- Returns - The unsigned short read.
	virtual unsigned short read_ushort16();

	//: Reads a signed 8 bit integer (char) from input source.
	//- Returns - The char read.
	virtual char read_char8();

	//: Reads an unsigned 8 bit integer (char) from input source.
	//- Returns - The unsigned char read.
	virtual unsigned char read_uchar8();

	//: Reads an ieee format floating point from input source.
	//- Returns - The float read.
	virtual float read_float32();

	//: Reads a bool from input source.
	//- Returns - The bool read.
	virtual bool read_bool8();

	//: Reads larger amounts of data (no endian and 64 bit conversion).
	//- data - Points to an array where the read data is stored.
	//- size - Number of bytes to read.
	//- Returns - Num bytes actually read.
	virtual int read(void *data, int size) = 0;
	
	//: Opens the input source. By default, it is open.
	virtual void open() = 0;
	
	//: Closes the input source.
	virtual void close() = 0;

	//: Make a copy of the current inputsource, standing at the same position.
	//- Returns - The copy of the input source.
	virtual CL_InputSource *clone() const = 0;

	//: Reads a string from the input source.
	//: <p>The binary format expected in the input source is first an uint32 telling the length of the
	//: string, and then the string itself.</p>
	//- Returns - The string read.
	virtual std::string read_string();

	//: Seeks to the specified position in the input source.
	//- pos - Position relative to 'seek_type'.
	//- seek_type - Defines what the 'pos' is relative to. Can be either seek_set, seek_cur og seek_end.
	virtual void seek(int pos, SeekEnum seek_type) = 0;

	//: Pushes the current input source position.
	//: The position can be restored again with pop_position.
	virtual void push_position() = 0;
	
	//: Pops a previous pushed input source position (returns to the position).
	virtual void pop_position() = 0;

//! Implementation:
protected:
	bool little_endian_mode;
};

#endif
