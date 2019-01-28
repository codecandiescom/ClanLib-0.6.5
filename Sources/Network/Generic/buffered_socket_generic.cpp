/*
	$Id: buffered_socket_generic.cpp,v 1.2 2001/09/08 19:24:18 japj Exp $

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

#include "buffered_socket_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_BufferedSocket_Generic construction:

CL_BufferedSocket_Generic::CL_BufferedSocket_Generic(CL_Socket_Generic *parent_impl)
: CL_Socket_Generic(), parent_impl(parent_impl)
{
}

CL_BufferedSocket_Generic::~CL_BufferedSocket_Generic()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_BufferedSocket_Generic attributes:

int CL_BufferedSocket_Generic::get_input_buffer_size() const
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_BufferedSocket_Generic operations:


void CL_BufferedSocket_Generic::set_input_buffer_size(int size)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_BufferedSocket_Generic implementation:
