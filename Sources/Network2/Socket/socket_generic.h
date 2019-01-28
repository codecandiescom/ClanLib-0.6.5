/*
	$Id: socket_generic.h,v 1.2 2002/11/02 19:57:05 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef socket_generic
#define socket_generic

#include "API/Core/System/keep_alive.h"
#include "API/Network2/Socket/socket.h"
#include "event_trigger_socket.h"
#include <stack>

#ifndef WIN32
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <sys/time.h>
	#include <unistd.h> 
#else
	#include <windows.h>
	typedef int socklen_t;
#endif

class CL_EventTrigger_Socket;

class CL_Socket_Generic : public CL_KeepAlive
{
//! Static operations:
public:
//	static sockaddr_in create_sockaddr_in(const CL_IPAddress &address);

	static CL_IPAddress create_ip_address(const sockaddr_in &addr_in);

//! Construction:
public:
	CL_Socket_Generic();

	virtual ~CL_Socket_Generic();

//! Attributes:
public:
	int sock;

	std::stack<std::string> push_stack;

	CL_EventTrigger_Socket *read, *write, *exception;

	CL_Signal_v0 sig_read_triggered;

	CL_Signal_v0 sig_write_triggered;

	CL_Signal_v0 sig_exception_triggered;
	
	CL_Signal_v0 sig_disconnected;

//! Operations:
public:
	void add_ref();

	void release_ref();

//! Overridables:
public:
	virtual CL_EventTrigger_Socket *create_read_trigger();

	virtual CL_EventTrigger_Socket *create_write_trigger();

	virtual CL_EventTrigger_Socket *create_exception_trigger();

	virtual void keep_alive();

//! Implementation:
private:
	int ref_count;
	
	//: Internal function to shutdown a socket that has disconnected
	void disconnect();
};

#endif
