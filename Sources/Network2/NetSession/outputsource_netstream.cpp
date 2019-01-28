/*
	$Id: outputsource_netstream.cpp,v 1.2 2002/11/02 19:57:05 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "API/Network2/NetSession/outputsource_netstream.h"
#include "API/Network2/NetSession/netstream.h"
#include "netstream_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OutputSource_NetStream Construction:

CL_OutputSource_NetStream::CL_OutputSource_NetStream(const CL_NetStream &sock)
: netstream(sock.impl), ref_count_netstream(true)
{
	netstream->add_ref();
}

CL_OutputSource_NetStream::~CL_OutputSource_NetStream()
{
	if (ref_count_netstream) netstream->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_OutputSource_NetStream Attributes:

int CL_OutputSource_NetStream::tell() const
{
	return 0;
}

int CL_OutputSource_NetStream::size() const
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OutputSource_NetStream Operations:

int CL_OutputSource_NetStream::write(const void *data, int size)
{
	return CL_NetStream(netstream).send(data, size);
}
	
void CL_OutputSource_NetStream::open()
{
}

void CL_OutputSource_NetStream::close()
{
}

CL_OutputSource *CL_OutputSource_NetStream::clone()
{
	return new CL_OutputSource_NetStream(CL_NetStream(netstream));
}
