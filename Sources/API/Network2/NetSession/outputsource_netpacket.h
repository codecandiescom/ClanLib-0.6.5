/*
	$Id: outputsource_netpacket.h,v 1.1 2002/11/02 19:46:01 mbn Exp $

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

#ifndef header_outputsource_netpacket
#define header_outputsource_netpacket

#include "../../Core/IOData/outputsource.h"

class CL_NetPacket;
class CL_OutputSource_NetPacket_Generic;

//: Output Source NetPacket Class.
class CL_OutputSource_NetPacket : public CL_OutputSource
{
//! Construction:
public:
	//: Output Source NetPacket Constructor.
	CL_OutputSource_NetPacket(const CL_NetPacket &netpacket);

	virtual ~CL_OutputSource_NetPacket();

//! Attributes:
public:
	//: Returns current position in output source.
	//return: Current position in source.
	virtual int tell() const;

	//: Returns the size of the output source
	//return: Size of the output source.
	virtual int size() const;

//! Operations:
public:
	//: Writes larger amounts of data (no endian and 64 bit conversion):
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
	friend class CL_NetPacket;

	// Constructor used to initialize internal CL_NetPacket member variable.
	CL_OutputSource_NetPacket();

	CL_OutputSource_NetPacket_Generic *impl;
};

#endif
