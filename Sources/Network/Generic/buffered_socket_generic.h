/*
	$Id: buffered_socket_generic.h,v 1.2 2001/09/08 19:24:18 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef buffered_socket_generic
#define buffered_socket_generic

#include "socket_generic.h"

class CL_BufferedSocket_Generic : public CL_Socket_Generic
{
// Construction:
public:
	CL_BufferedSocket_Generic(CL_Socket_Generic *parent_impl);

	virtual ~CL_BufferedSocket_Generic();

// Attributes:
public:
	int get_input_buffer_size() const;

	CL_Socket_Generic *parent_impl;

// Operations:
public:
	void set_input_buffer_size(int size);

// Implementation:
private:
};

#endif
