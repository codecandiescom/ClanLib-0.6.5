/*
	$Id: event_trigger_socket.h,v 1.5 2002/01/16 11:21:03 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_event_trigger_socket
#define header_event_trigger_socket

#include "API/signals.h"
#include "API/Core/System/event_trigger.h"
#include "socket_select.h"
#include <map>

class CL_Socket_Generic;

class CL_EventTrigger_Socket : public CL_EventTrigger
{
public:
	enum Type
	{
		read,
		write,
		exception
	};

// Construction:
public:
	CL_EventTrigger_Socket(CL_Socket_Generic *sock, Type type);

	~CL_EventTrigger_Socket();

// Attributes:
public:
	CL_Socket_Generic *get_socket() const { return sock; }

	Type get_type() const { return type; }

// Operations:
public:
	void start_listen();

// Implementation:
private:

	CL_Socket_Generic *sock;
	Type type;

	static CL_SocketSelect socket_select;
};

#endif
