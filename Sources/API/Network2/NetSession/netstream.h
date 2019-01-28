/*
	$Id: netstream.h,v 1.1 2002/11/02 19:46:01 mbn Exp $

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

#ifndef header_netstream
#define header_netstream

#ifdef WIN32
#pragma warning(disable : 4786)
#endif

#include "inputsource_netstream.h"
#include "outputsource_netstream.h"
#include <string>

class CL_NetSession;
class CL_NetComputer;
class CL_EventTrigger;
class CL_NetStream_Generic;

//: Net stream class.
class CL_NetStream
{
//! Construction:
public:
	//: Connects to a remote netstream.
	//param stream_channel: Channel identifier establishing connection to.
	//param dest: Remote computer on which to connect.
	CL_NetStream(
		const std::string &stream_channel,
		CL_NetComputer &dest);

	CL_NetStream(const CL_NetStream &copy);

	virtual ~CL_NetStream();

//! Attributes:
public:
	//: Input source interface for netstream.
	CL_InputSource_NetStream input;

	//: Output source interface for netstream.
	CL_OutputSource_NetStream output;

	//: Event trigger for reading.
	CL_EventTrigger *get_read_trigger();

	//: Event trigger for writing.
	CL_EventTrigger *get_write_trigger();

//! Operations:
public:
	//: Copy constructor.
	CL_NetStream &operator =(const CL_NetStream &copy);

	//: Send data onto stream.
	int send(const void *data, int size);

	//: Receive data from stream.
	int recv(void *data, int size);

//! Implementation:
public:
	CL_NetStream(CL_NetStream_Generic *impl);

	CL_NetStream_Generic *impl;
};

#endif
