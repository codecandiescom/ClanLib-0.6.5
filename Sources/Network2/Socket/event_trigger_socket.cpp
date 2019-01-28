/*
	$Id: event_trigger_socket.cpp,v 1.1 2002/11/02 19:46:07 mbn Exp $

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

#include "event_trigger_socket.h"
#include "socket_generic.h"

#ifdef WIN32
#include "Core/System/Win32/event_trigger_win32.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_EventTrigger_Socket::CL_EventTrigger_Socket(CL_Socket_Generic *sock, Type type)
: sock(sock), type(type)
{
	start_listen();
}

CL_EventTrigger_Socket::~CL_EventTrigger_Socket()
{
	switch (type)
	{
	case read:      socket_select.remove_read(this); break;
	case write:     socket_select.remove_write(this); break;
	case exception: socket_select.remove_exception(this); break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

/////////////////////////////////////////////////////////////////////////////
// Operations:

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void CL_EventTrigger_Socket::start_listen()
{
	reset();
	switch (type)
	{
	case read:      socket_select.listen_read(this); break;
	case write:     socket_select.listen_write(this); break;
	case exception: socket_select.listen_exception(this); break;
	}
}

CL_SocketSelect CL_EventTrigger_Socket::socket_select;
