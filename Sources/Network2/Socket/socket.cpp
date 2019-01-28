/*
	$Id: socket.cpp,v 1.1 2002/11/02 19:46:07 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifdef WIN32
#pragma warning (disable:4786)
#else
#include <unistd.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#define INVALID_SOCKET -1
#endif

#include "socket_generic.h"
#include "API/Core/System/error.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Socket construction:

CL_Socket::CL_Socket()
: impl(0)
{
}

CL_Socket::CL_Socket(int socket)
: impl(new CL_Socket_Generic)
{
	impl->add_ref();
	impl->sock = socket;

	input.socket = impl;
	output.socket = impl;
}

CL_Socket::CL_Socket(CL_Socket::Type type)
: impl(new CL_Socket_Generic)
{
	impl->add_ref();

	int in_type = SOCK_STREAM, in_proto = 0;
	switch (type)
	{
	case tcp: in_type = SOCK_STREAM; break;
	case udp: in_type = SOCK_DGRAM; break;
	}
	
	proto = type;

	impl->sock = socket(AF_INET, in_type, in_proto);
	if (impl->sock < 0) throw CL_Error("CL_Socket::CL_Socket failed");

	input.socket = impl;
	output.socket = impl;
}

CL_Socket::CL_Socket(const CL_Socket &copy)
: impl(copy.impl)
{
	if (impl) impl->add_ref();
	input.socket = impl;
	output.socket = impl;
}

CL_Socket::~CL_Socket()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Socket attributes:

int CL_Socket::get_socket() const
{
	return impl->sock;
}

CL_EventTrigger *CL_Socket::get_read_trigger() const
{
	if (impl->read == NULL) impl->read = impl->create_read_trigger();
	return impl->read;
}

CL_EventTrigger *CL_Socket::get_write_trigger() const
{
	if (impl->write == NULL) impl->write = impl->create_write_trigger();
	return impl->write;
}

CL_EventTrigger *CL_Socket::get_exception_trigger() const
{
	if (impl->exception == NULL) impl->exception = impl->create_exception_trigger();
	return impl->exception;
}

CL_IPAddress CL_Socket::get_source_address() const
{
	sockaddr_in addr_in;
	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;

	int size = sizeof(sockaddr_in);

	int result = getsockname(impl->sock, (sockaddr *) &addr_in, (socklen_t *)&size);
	if (result != 0) throw CL_Error("CL_Socket::get_source_address() failed.");

	return CL_Socket_Generic::create_ip_address(addr_in);
}

CL_IPAddress CL_Socket::get_dest_address() const
{
	sockaddr_in addr_in;
	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;

	int size = sizeof(sockaddr_in);

	int result = getpeername(impl->sock, (sockaddr *) &addr_in, (socklen_t *)&size);
	if (result != 0) throw CL_Error("CL_Socket::get_dest_address() failed.");

	return CL_Socket_Generic::create_ip_address(addr_in);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Socket operations:

CL_Socket &CL_Socket::operator =(const CL_Socket &other)
{
	if (impl) impl->release_ref();
	impl = other.impl;
	if (impl) impl->add_ref();
	input.socket = impl;
	output.socket = impl;
	return *this;
}

void CL_Socket::set_nonblocking(bool nonblocking)
{
#ifdef WIN32
	u_long argp = nonblocking ? 1 : 0;
	ioctlsocket(impl->sock, FIONBIO, &argp);
#else
	fcntl(impl->sock, F_SETFL, O_NONBLOCK, nonblocking ? 1 : 0);
#endif
}

void CL_Socket::set_nodelay(bool nodelay)
{
	static int sol_tcp = -1;
	if (sol_tcp == -1) sol_tcp = getprotobyname("tcp")->p_proto;

	int value = nodelay ? 1 : 0;
	setsockopt(impl->sock, sol_tcp, TCP_NODELAY, (const char *) &value, sizeof(int));
}

int CL_Socket::send(const void *data, int size)
{
	int result = ::send(
		impl->sock,
		(const char *) data,
		size,
		0);
	if (result < 0) throw CL_Error("CL_Socket::send failed");

	if (impl->write) impl->write->start_listen();
	return result;
}

int CL_Socket::send(const void *data, int size, const CL_IPAddress &dest)
{
	sockaddr addr;
	int len;

	dest.get_addrinfo(proto, addr, len);
	
	int result = ::sendto(
		impl->sock,
		(const char *) data,
		size,
		0,
		&addr,
		len);
	if (result < 0) throw CL_Error("CL_Socket::send failed");

	if (impl->write) impl->write->start_listen();
	return result;
}

int CL_Socket::send(const std::string &string)
{
	return send(string.c_str(), string.size());
}

int CL_Socket::recv(void *data, int size)
{
	int result = ::recv(impl->sock, (char *) data, size, 0);
	if (result < 0) throw CL_Error("CL_Socket::recv failed");

	if (impl->read) impl->read->start_listen();
	return result;
}

void CL_Socket::push(const std::string &string)
{
	impl->push_stack.push(string);
}

int CL_Socket::recv(void *data, int size, CL_IPAddress &from)
{
	char *_data = (char *) data;
	int total = 0;

	// Start filling data with pushed data (if any):
	while (!impl->push_stack.empty())
	{
		std::string &str = impl->push_stack.top();
		if (str.size() < (unsigned int)size)
		{
			memcpy(_data, str.data(), str.size());
			_data += str.size();
			total += str.size();
			size -= str.size();
			impl->push_stack.pop();
		}
		else
		{
			memcpy(_data, str.data(), size);
			str = str.substr(size);
			total += size;
			return total;
		}
	}

	// Fill with data from network socket:
	sockaddr_in addr_in;
	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;

	socklen_t addr_size = sizeof(addr_in);

	int result = ::recvfrom(
		impl->sock,
		_data,
		size,
		0,
		(sockaddr *) &addr_in,
		&addr_size);
	if (result < 0) throw CL_Error("CL_Socket::recv failed");
	total += result;

	if (impl->read) impl->read->start_listen();
	from = CL_Socket_Generic::create_ip_address(addr_in);
	return total;
}

void CL_Socket::connect(const CL_IPAddress &address)
{
	sockaddr addr;
	int len;

	address.get_addrinfo(proto, addr, len);

	int result = ::connect(impl->sock, &addr, len);
	if (result < 0) throw CL_Error("CL_Socket::connect failed");
	if (impl->read) impl->read->start_listen();
	if (impl->exception) impl->exception->start_listen();
}

void CL_Socket::shutdown(ShutdownHow how)
{
	int in_how = 0;
	switch (how)
	{
	case shutdown_receive: in_how = 0; break;
	case shutdown_send:    in_how = 1; break;
	}

	int result = ::shutdown(impl->sock, in_how);
	if (result < 0) throw CL_Error("CL_Socket::shutdown failed");
}

void CL_Socket::bind(const CL_IPAddress &address)
{
	sockaddr addr;
	int len;

	address.get_addrinfo(proto, addr, len);
	
	int result = ::bind(impl->sock, &addr, len);
	if (result < 0) throw CL_Error("CL_Socket::bind failed");
}

void CL_Socket::listen(int backlog)
{
	int result = ::listen(impl->sock, backlog);
	if (result < 0) throw CL_Error("CL_Socket::listen failed");
}

CL_Socket CL_Socket::accept()
{
	int new_sock = ::accept(impl->sock, NULL, NULL);
	if (new_sock == INVALID_SOCKET) throw CL_Error("CL_Socket::accept failed");
	if (impl->read) impl->read->start_listen();
	return CL_Socket(new_sock);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Socket signals:

CL_Signal_v0 &CL_Socket::sig_read_triggered()
{
	return impl->sig_read_triggered;
}

CL_Signal_v0 &CL_Socket::sig_write_triggered()
{
	return impl->sig_write_triggered;
}

CL_Signal_v0 &CL_Socket::sig_exception_triggered()
{
	return impl->sig_exception_triggered;
}

CL_Signal_v0 &CL_Socket::sig_disconnected()
{
	return impl->sig_disconnected;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Socket implementation:

CL_Socket::CL_Socket(CL_Socket_Generic *impl)
: impl(impl)
{
	impl->add_ref();
	input.socket = impl;
	output.socket = impl;
}
