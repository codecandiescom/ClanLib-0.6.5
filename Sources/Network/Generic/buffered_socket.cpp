/*
	$Id: buffered_socket.cpp,v 1.3 2001/09/08 19:24:18 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include "API/Network/buffered_socket.h"
#include "buffered_socket_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_BufferedSocket construction:

CL_BufferedSocket::CL_BufferedSocket(int socket)
: CL_Socket(socket), impl(NULL)
{
	impl = new CL_BufferedSocket_Generic(CL_Socket::impl);
	CL_Socket::impl = impl;
}

CL_BufferedSocket::CL_BufferedSocket(CL_Socket::Type type)
: CL_Socket(type), impl(NULL)
{
	impl = new CL_BufferedSocket_Generic(CL_Socket::impl);
	CL_Socket::impl = impl;
}

CL_BufferedSocket::CL_BufferedSocket(const CL_Socket &copy)
: CL_Socket(new CL_BufferedSocket_Generic(copy.impl)), impl(NULL)
{
	impl = static_cast<CL_BufferedSocket_Generic *>(CL_Socket::impl);
}

CL_BufferedSocket::CL_BufferedSocket(const CL_BufferedSocket &copy)
: CL_Socket(copy.impl), impl(copy.impl)
{
}

CL_BufferedSocket::~CL_BufferedSocket()
{
	// no need to delete impl; that is handled by CL_Socket.
}

/////////////////////////////////////////////////////////////////////////////
// CL_BufferedSocket attributes:

int CL_BufferedSocket::get_input_buffer_size() const
{
	return impl->get_input_buffer_size();
}

/////////////////////////////////////////////////////////////////////////////
// CL_BufferedSocket operations:

void CL_BufferedSocket::set_input_buffer_size(int size)
{
	impl->set_input_buffer_size(size);
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:
