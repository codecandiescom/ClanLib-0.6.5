/*
	$Id: outputsource_zipped.h,v 1.9 2001/10/10 11:50:27 sphair Exp $

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

#ifndef header_outputsource_zipped
#define header_outputsource_zipped

#include "outputsource.h"

class CL_OutputSource_Zipped_Generic;

//: The Output Source Zipped Class
class CL_OutputSource_Zipped : public CL_OutputSource
{
//! Construction:
public:
	//: <p>Constructs an output source that zips the data and passes it on to
	//: the passed output source.</p>
	//: <p>If delete_output is true, the zipped output source will delete the
	//: output source at destruction.</p>
	CL_OutputSource_Zipped(CL_OutputSource *output, bool delete_output = false);

	//: Copy constructor. Does the same as the clone() function.
	CL_OutputSource_Zipped(const CL_OutputSource_Zipped &copy);

	//: Destructor.
	virtual ~CL_OutputSource_Zipped();

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
	CL_OutputSource_Zipped_Generic *impl;
};

#endif
