/*
	$Id: inputsource_netstream.cpp,v 1.2 2002/11/02 19:57:05 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "API/Network2/NetSession/inputsource_netstream.h"
#include "API/Network2/NetSession/netstream.h"
#include "netstream_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_NetStream construction:

CL_InputSource_NetStream::CL_InputSource_NetStream(const CL_NetStream &sock)
: netstream(sock.impl), ref_count_netstream(true)
{
	netstream->add_ref();
}

CL_InputSource_NetStream::~CL_InputSource_NetStream()
{
	if (ref_count_netstream) netstream->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_NetStream attributes:

int CL_InputSource_NetStream::tell() const
{
	return 0;
}

int CL_InputSource_NetStream::size() const
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_NetStream operations:

int CL_InputSource_NetStream::read(void *data, int size)
{
	return CL_NetStream(netstream).recv(data, size);
}

void CL_InputSource_NetStream::open()
{
}

void CL_InputSource_NetStream::close()
{
}

CL_InputSource *CL_InputSource_NetStream::clone() const
{
	return new CL_InputSource_NetStream(CL_NetStream(netstream));
}

void CL_InputSource_NetStream::seek(int pos, SeekEnum seek_type)
{
}

void CL_InputSource_NetStream::push_position()
{
}

void CL_InputSource_NetStream::pop_position()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_NetStream implementation:
