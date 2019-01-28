/*
	$Id: inputsource_zipped.h,v 1.8 2001/10/10 11:50:27 sphair Exp $

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

#ifndef header_inputsource_zipped
#define header_inputsource_zipped

#include "inputsource.h"

class CL_InputSource_Zipped_Generic;

//: The Input Source Zipped Class
class CL_InputSource_Zipped : public CL_InputSource
{
//! Construction:
public:
	//: Starts reading zipped input from the specified input source.
	//: <p>If 'delete_input' is true, the input source will be deleted when
	//: the zipped input source is deleted.</p>
	CL_InputSource_Zipped(CL_InputSource *input, bool delete_input = false);

	// Destructor.
	virtual ~CL_InputSource_Zipped();

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

	//: Pushes the current input source position.
	//: The position can be restored again with pop_position.
	virtual void push_position();
	
	//: Pops a previous pushed input source position (returns to the position).
	virtual void pop_position();

//! Implementation:
private:
	CL_InputSource_Zipped_Generic *impl;
};

#endif
