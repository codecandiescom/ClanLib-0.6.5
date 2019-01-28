/*
	$Id: outputsource_netstream.h,v 1.1 2002/11/02 19:46:01 mbn Exp $

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

#ifndef header_outputsource_netstream
#define header_outputsource_netstream

#include "../../Core/IOData/outputsource.h"

class CL_NetStream;
class CL_NetStream_Generic;

//: Output Source NetStream Class.
class CL_OutputSource_NetStream : public CL_OutputSource
{
//! Construction:
public:
	//: Output Source NetStream Constructor.
	CL_OutputSource_NetStream(const CL_NetStream &netstream);

	virtual ~CL_OutputSource_NetStream();

//! Attributes:
public:
	//: Returns current position in output source.
	//return: Current position in source.
	virtual int tell() const;

	//: Returns the size of the output source.
	//return: Size of the output source.
	virtual int size() const;

//! Operations:
public:
	//: Writes larger amounts of data (no endian and 64 bit conversion).
	//param data: Points to the array from which to write.
	//param size: Number of bytes to write.
	//return: Num bytes actually written.
	virtual int write(const void *data, int size);
	
	//: Opens the output source. By default, it is open.
	virtual void open();
	
	//: Closes the output source.
	virtual void close();

	//: Make a copy of the current outputsource, standing at the same position.
	//return: The copy of the output source.
	virtual CL_OutputSource *clone();

//! Implementation:
private:
	friend class CL_NetStream;

	// Constructor used to initialize internal CL_NetStream member variable.
	CL_OutputSource_NetStream() { netstream = 0; ref_count_netstream = false; }

	CL_NetStream_Generic *netstream;
	bool ref_count_netstream;
};

#endif
