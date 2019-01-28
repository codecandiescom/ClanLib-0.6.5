/*
	$Id: outputsource_socket.cpp,v 1.2 2002/11/02 19:57:05 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "API/Network2/Socket/outputsource_socket.h"
#include "API/Network2/Socket/socket.h"
#include "socket_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OutputSource_Socket Construction:

CL_OutputSource_Socket::CL_OutputSource_Socket(const CL_Socket &sock)
: socket(sock.impl), ref_count_socket(true)
{
	socket->add_ref();
}

CL_OutputSource_Socket::~CL_OutputSource_Socket()
{
	if (ref_count_socket) socket->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_OutputSource_Socket Attributes:

int CL_OutputSource_Socket::tell() const
{
	return 0;
}

int CL_OutputSource_Socket::size() const
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OutputSource_Socket Operations:

int CL_OutputSource_Socket::write(const void *data, int size)
{
	return CL_Socket(socket).send(data, size);
}
	
void CL_OutputSource_Socket::open()
{
}

void CL_OutputSource_Socket::close()
{
}

CL_OutputSource *CL_OutputSource_Socket::clone()
{
	return new CL_OutputSource_Socket(CL_Socket(socket));
}
