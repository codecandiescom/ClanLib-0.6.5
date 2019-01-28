/*
	$Id: inputsource_socket.cpp,v 1.2 2002/11/02 19:57:05 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "API/Network2/Socket/inputsource_socket.h"
#include "API/Network2/Socket/socket.h"
#include "socket_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Socket construction:

CL_InputSource_Socket::CL_InputSource_Socket(const CL_Socket &sock)
: socket(sock.impl), ref_count_socket(true)
{
	socket->add_ref();
}

CL_InputSource_Socket::~CL_InputSource_Socket()
{
	if (ref_count_socket) socket->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Socket attributes:

int CL_InputSource_Socket::tell() const
{
	return 0;
}

int CL_InputSource_Socket::size() const
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Socket operations:

int CL_InputSource_Socket::read(void *data, int size)
{
	return CL_Socket(socket).recv(data, size);
}

void CL_InputSource_Socket::open()
{
}

void CL_InputSource_Socket::close()
{
}

CL_InputSource *CL_InputSource_Socket::clone() const
{
	return new CL_InputSource_Socket(CL_Socket(socket));
}

void CL_InputSource_Socket::seek(int pos, SeekEnum seek_type)
{
}

void CL_InputSource_Socket::push_position()
{
}

void CL_InputSource_Socket::pop_position()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Socket implementation:
