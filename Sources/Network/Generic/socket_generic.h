/*
	$Id: socket_generic.h,v 1.10 2001/11/22 00:43:11 plasmoid Exp $

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
#include "API/Network/socket.h"
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
  #ifdef IPV6
  	#include <winsock2.h>
		#include <ws2tcpip.h>
  #endif
#endif

class CL_EventTrigger_Socket;

class CL_Socket_Generic : public CL_KeepAlive
{
// Static operations:
public:
	static sockaddr_in create_sockaddr_in(const CL_IPAddress &address);

  #ifdef IPV6
  static sockaddr_in6 create_sockaddr_in6(const CL_IPAddress &address);
  #endif 

	static CL_IPAddress create_ip_address(const sockaddr_in &addr_in);

  #ifdef IPV6
  static CL_IPAddress create_ip_address(const sockaddr_in6 &addr_in);
  #endif

// Construction:
public:
	CL_Socket_Generic();

	virtual ~CL_Socket_Generic();

// Attributes:
public:
	int sock;

	std::stack<std::string> push_stack;

	CL_EventTrigger_Socket *read, *write, *exception;

	CL_Signal_v0 sig_read_triggered;

	CL_Signal_v0 sig_write_triggered;

	CL_Signal_v0 sig_exception_triggered;

// Operations:
public:
	void add_ref();

	void release_ref();

// Overridables:
public:
	virtual CL_EventTrigger_Socket *create_read_trigger();

	virtual CL_EventTrigger_Socket *create_write_trigger();

	virtual CL_EventTrigger_Socket *create_exception_trigger();

	virtual void keep_alive();

// Implementation:
private:
	int ref_count;
};

#endif
