/*
	$Id: socket_select.h,v 1.6 2002/01/16 11:21:03 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_socket_select
#define header_socket_select

#include <list>
#include "API/Core/System/thread.h"

#ifdef WIN32
#include <winsock2.h>
#endif

class CL_Thread;
class CL_Mutex;
class CL_EventTrigger_Socket;

class CL_SocketSelect : CL_Runnable
{
// Construction:
public:
	CL_SocketSelect();
	// Creates the socket select listener thread.

	virtual ~CL_SocketSelect();
	// Shut down socket select listener thread.

// Attributes:
public:

// Operations:
public:
	void listen_read(CL_EventTrigger_Socket *socket);
	// Start a read listen for the socket.

	void listen_write(CL_EventTrigger_Socket *socket);
	// Start a write listen for the socket.

	void listen_exception(CL_EventTrigger_Socket *socket);
	// Start a exception listen for the socket.

	void remove_read(CL_EventTrigger_Socket *socket);
	// Remove socket from the read listening.

	void remove_write(CL_EventTrigger_Socket *socket);
	// Remove socket from the write listening.

	void remove_exception(CL_EventTrigger_Socket *socket);
	// Remove socket from the exception listening.

// Implementation:
private:
	void signal_listen_thread();
	// Make the socket select thread break its listen and check for updates.
	// This is used to make it start listening on new sockets, and stop listening
	// on old sockets.

	virtual void run();
	// Socket select thread function.

	CL_Thread *listen_thread;
	CL_Mutex *mutex;
	volatile bool stop_thread;

	std::list<CL_EventTrigger_Socket*> read_sockets;
	std::list<CL_EventTrigger_Socket*> write_sockets;
	std::list<CL_EventTrigger_Socket*> exception_sockets;

#ifdef WIN32
	WSAEVENT signal_event;
#else
	int signal_pipes[2];
#endif
};

#endif
