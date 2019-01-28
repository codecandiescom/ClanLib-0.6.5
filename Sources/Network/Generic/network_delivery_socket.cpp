/*
	$Id: network_delivery_socket.cpp,v 1.6 2001/11/01 20:27:57 mbn Exp $

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

#include <API/Core/System/error.h>
#include "API/Core/System/cl_assert.h"
#include <Network/Generic/network_delivery_impl.h>
#include <Network/Generic/network_delivery_socket.h>

#include <string.h>
#include <errno.h>

// ClanLib network magic. This magic is the first DWORD sent across a socket.
// If the received DWORD doesn't match our magic, we can be sure it isn't a clanlib
// networking application on that port.
#define NETWORK_MAGIC 0x16042104

/******************************************************************************
                         Unix Socket implementation
******************************************************************************/

CL_UniformSocket::CL_UniformSocket(CL_ConnectionProvider *_provider)
{
	provider = _provider;
	is_connection_lost = false;
	sock = CL_INVALID_SOCKET;
	send_pos = 0;

	recv_state = expect_magic;
	cur_message = NULL;
	cur_message_size = -1;
}

CL_UniformSocket::~CL_UniformSocket()
{
	if (sock != CL_INVALID_SOCKET) closesocket(sock);
	provider->remove_connection(this);
}

bool CL_UniformSocket::read_avail()
{
	fd_set rfds;

	FD_ZERO(&rfds);
	FD_SET(sock, &rfds);

	timeval timeout;
	memset(&timeout, 0, sizeof(timeval));

	int retval = select(sock+1, &rfds, NULL, NULL, &timeout);
	if (retval == -1 || retval == 0) 
	{
		return false;
	}

	char buf[1024];

	int data_received = ::recv(
		sock,
		buf,
		1024,
		0);

	if (data_received == 0 || data_received == -1)
	{
		is_connection_lost = true;
		return false;
	}
	recv_buffer.append(buf, data_received);

	return true;
}

bool CL_UniformSocket::require_avail(unsigned int size)
{
	return recv_buffer.size() >= size;
}

bool CL_UniformSocket::get_avail(void *buf, unsigned int size)
{
	if (recv_buffer.size() < size) return false;

	memcpy(buf, recv_buffer.data(), size);
	recv_buffer.erase(0, size);

	return true;
}

bool CL_UniformSocket::init_socket(int init_socket)
{
	static int sol_tcp = -1;
	if (sol_tcp == -1) sol_tcp = getprotobyname("tcp")->p_proto;

	if (init_socket != CL_INVALID_SOCKET)
	{
		sock = init_socket;

	// set socket into non-blocking mode:
#ifdef WIN32
		u_long argp = 1;
		ioctlsocket(sock, FIONBIO, &argp);
#else
		fcntl(sock, F_SETFL, O_NONBLOCK);
#endif

		int value = 1;
		setsockopt(sock, sol_tcp, TCP_NODELAY, (const char *) &value, sizeof(int));
		write_int(NETWORK_MAGIC);
	}
	else
	{
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == CL_INVALID_SOCKET) return false;

	// set socket into non-blocking mode:
#ifdef WIN32
		u_long argp = 1;
		ioctlsocket(sock, FIONBIO, &argp);
#else
		fcntl(sock, F_SETFL, O_NONBLOCK);
#endif

		int value = 1;
		setsockopt(sock, sol_tcp, TCP_NODELAY, (const char *) &value, sizeof(int));
	}

	return true;
}

bool CL_UniformSocket::peek()
{
	if (sock == CL_INVALID_SOCKET) return false;

	read_avail();
	send_avail();

	switch (recv_state)
	{
	case expect_magic:
		{
			int found_magic;
			if (get_avail(&found_magic, sizeof(int)) == false) return false;
			found_magic = ntohl(found_magic);
			if (found_magic != NETWORK_MAGIC)
			{
				is_connection_lost=true;
			}
			else recv_state = expect_packet_size;
		}

	case expect_packet_size:
		{
			if (get_avail(&cur_message_size, sizeof(int)) == false) return false;
			cur_message_size = ntohl(cur_message_size);
			recv_state = expect_packet_data;
		}

	case expect_packet_data:
		{
			if (!require_avail(cur_message_size)) return false;
			cur_message = new char[cur_message_size];
			get_avail(cur_message, cur_message_size);
			recv_state = packet_finished;
		}

	case packet_finished:
		return true;

	default:
		throw CL_Error("Network protocol error.");
		return false;
	};

	return false;
}

CL_ConnectionPacket CL_UniformSocket::receive()
{
	if (peek() == false)
	{
		CL_ConnectionPacket ret;
		ret.size = 0;
		ret.data = NULL;
		return ret;
	}

	CL_ConnectionPacket ret;
	ret.size = cur_message_size;
	ret.data = cur_message;

	recv_state = expect_packet_size;

	return ret;
}

void CL_UniformSocket::send(CL_ConnectionPacket message)
{
	write_int(message.size);
	write_data(message.data, message.size);
}

bool CL_UniformSocket::connection_lost()
{
	return is_connection_lost;
}

unsigned long CL_UniformSocket::get_peer_address()
{
	sockaddr_in addr_in;
	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;

	int size = sizeof(sockaddr_in);
	int result = getpeername(sock, (sockaddr *) &addr_in, (socklen_t*) &size);
	if (result != 0) throw CL_Error("CL_UniformAcceptSocket::get_peer_address() failed.");

	#ifdef WIN32
	return addr_in.sin_addr.S_un.S_addr;
	#else
	return addr_in.sin_addr.s_addr;
	#endif
}

unsigned short CL_UniformSocket::get_peer_port()
{
	sockaddr_in addr_in;
	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;

	int size = sizeof(sockaddr_in);
	int result = getpeername(sock, (sockaddr *) &addr_in, (socklen_t*) &size);
	if (result != 0) throw CL_Error("CL_UniformAcceptSocket::get_peer_port() failed.");

	return ntohs(addr_in.sin_port);
}

void CL_UniformSocket::disconnect()
{
	shutdown(sock, 2);
	is_connection_lost = true;
}

unsigned long CL_UniformSocket::read_int()
{
	unsigned char data[4];
	int read = 0;
	while (read < 4)
	{
		int val = recv(sock, (char *) data+read, 4-read, 0);
		if (val == 0 || val == -1) 
		{
			is_connection_lost = true;
			return 0;
		}

		read += val;
	}

	return ntohl(*((unsigned long *) data));
}

void CL_UniformSocket::write_int(unsigned long value)
{
	unsigned long write_num = htonl(value);

	write_data(&write_num, sizeof(unsigned long));
}

void CL_UniformSocket::write_data(void *data, unsigned int size)
{
	send_buffer.push(std::string((char *) data, size));
//	send_avail();
}
#include <iostream>
bool CL_UniformSocket::send_avail()
{
	if (send_buffer.empty()) return true;

	std::string &buf = send_buffer.front();
	int size = buf.size();
	int res = ::send(sock, buf.data()+send_pos, size-send_pos, 0);
	if (res == CL_SOCKET_ERROR)
	{
#ifdef WIN32
		if (WSAGetLastError() == WSAEWOULDBLOCK) return false;
#else
		if (errno == EAGAIN) return false;
#endif
	
		cl_info_debug(info_network, PRINT_SOCK_ERROR());
		is_connection_lost = true;
		return true;
	}

	send_pos += res;
	if (send_pos  == size)
	{
		send_buffer.pop();
		send_pos = 0;
	}

	return send_buffer.empty();
}

bool CL_UniformSocket::try_connect(unsigned long remote_ip_network_format, int port)
{
	sockaddr_in sock_addr;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.s_addr = remote_ip_network_format;
	sock_addr.sin_port = htons(port);

	int res = ::connect(sock, (sockaddr *) &sock_addr, sizeof(sockaddr_in));

	fd_set fdfailure, fdwrite;
	FD_ZERO(&fdfailure);
	FD_ZERO(&fdwrite);
	FD_SET(sock, &fdfailure);
	FD_SET(sock, &fdwrite);
	res = ::select(sock+1, NULL, &fdwrite, &fdfailure, NULL);
	if (res > 0)
	{
		if (FD_ISSET(sock, &fdfailure)) // connection failed
		{
			throw CL_Error("Connection refused");
		}

		write_int(NETWORK_MAGIC);

		return true;
	}
	else 
	{
//		cl_info_debug(info_network, PRINT_SOCK_ERROR());
		throw CL_Error(PRINT_SOCK_ERROR());
		return false;
	}
}

/******************************************************************************
					CL_UniformAcceptSocket implementation
******************************************************************************/

CL_UniformAcceptSocket::CL_UniformAcceptSocket(CL_ConnectionProvider *provider)
: CL_UniformSocket(provider)
{
	port = -1;
}

CL_UniformAcceptSocket::~CL_UniformAcceptSocket()
{
}

bool CL_UniformAcceptSocket::bind(int _port)
{
	if (!init_socket()) return false;

	// We unable SO_LINGER and enable SO_REUSEADDR.
	// This allows the program to open/bind/close
	// a listening socket on a given port, and then
	// reopen and rebind another socket on the same
	// port without getting an "Address already in use"
	// error. Needed under UNIX, not a problem under Win32.
	struct linger ls;
	ls.l_onoff = 0;
	setsockopt(sock, SOL_SOCKET, SO_LINGER, (const char *)&ls, sizeof(struct linger));
	int yes = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(int));

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);

	// Bind the socket to the specified port
	int err = ::bind(sock, (sockaddr *) &addr, sizeof(sockaddr_in));	
	if (err == CL_SOCKET_ERROR) throw CL_Error(PRINT_SOCK_ERROR());

	// Call getsockname to determine allocated port-number,
	// since socket might have been created with _port==0 (any)
	socklen_t len = sizeof(sockaddr_in);
	err = getsockname(sock, (sockaddr *) &addr, &len);
	if (err == CL_SOCKET_ERROR) throw CL_Error(PRINT_SOCK_ERROR());

	port = ntohs(addr.sin_port);
	
	// Make the socket listen for incoming connection attempts
	err = listen(sock, 64);
	if (err == CL_SOCKET_ERROR) throw CL_Error(PRINT_SOCK_ERROR());

	return true;
}

bool CL_UniformAcceptSocket::peek()
{
	fd_set rfds;

	FD_ZERO(&rfds);
	FD_SET(sock, &rfds);

	timeval timeout;
	memset(&timeout, 0, sizeof(timeval));

	int retval = select(sock+1, &rfds, NULL, NULL, &timeout);
	cl_assert(retval != -1);

	if (retval == 0) return false;
	else return true;
}

CL_UniformSocket *CL_UniformAcceptSocket::accept()
{
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(sock, &rfds);

	timeval timeout;
	memset(&timeout, 0, sizeof(timeval));

	int retval = select(sock+1, &rfds, NULL, NULL, &timeout);
	if (retval == -1 || retval == 0) 
	{
		return NULL;
	}

	int res_sock = ::accept(sock, NULL, NULL);
	cl_assert(res_sock != CL_INVALID_SOCKET);

	CL_UniformSocket *ret = new CL_UniformSocket(provider);
	ret->init_socket(res_sock);

	return ret;
}

bool CL_UniformAcceptSocket::try_connect(unsigned long remote_ip_network_format, int port)
{
	cl_assert(false);

	return false;
}

CL_ConnectionPacket CL_UniformAcceptSocket::receive()
{
	cl_assert(false);
	CL_ConnectionPacket ret;
	return ret;
}

void CL_UniformAcceptSocket::send(CL_ConnectionPacket message)
{
	cl_assert(false);
}

bool CL_UniformAcceptSocket::connection_lost()
{
	cl_assert(false);
	return false;
}

/******************************************************************************
					CL_UniformUDPConnection implementation
******************************************************************************/

CL_UniformUDPConnection::CL_UniformUDPConnection()
{
	sock = CL_INVALID_SOCKET;
	port = -1;
}

CL_UniformUDPConnection::~CL_UniformUDPConnection()
{
	if (sock != CL_INVALID_SOCKET) closesocket(sock);
}

bool CL_UniformUDPConnection::bind(unsigned int _port)
{
	int res;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == CL_INVALID_SOCKET) return false;

	// We unable SO_LINGER and enable SO_REUSEADDR.
	// This allows the program to open/bind/close
	// a listening socket on a given port, and then
	// reopen and rebind another socket on the same
	// port without getting an "Address already in use"
	// error. Needed under UNIX, not a problem under Win32.
	struct linger ls;
	ls.l_onoff = 0;
	setsockopt(sock, SOL_SOCKET, SO_LINGER, (const char *)&ls, sizeof(struct linger));
	int yes = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(int));

	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);

	res = ::bind(sock, (sockaddr *) &addr, sizeof(addr));
	if (res == -1) throw CL_Error(PRINT_SOCK_ERROR());

	int enable = 1;
	res = setsockopt(
		sock,
		SOL_SOCKET,
		SO_BROADCAST,
		(const char *) &enable,
		sizeof(int));

	if (res == -1)
	{
		cl_info(info_network, PRINT_SOCK_ERROR());
		closesocket(sock);
		return false;
	}

	// Call getsockname to determine allocated port-number,
	// since socket might have been created with _port==0 (any)
	socklen_t len = sizeof(sockaddr_in);
	res = getsockname(sock, (sockaddr *) &addr, &len);
	if (res == -1) throw CL_Error(PRINT_SOCK_ERROR());

	port = ntohs(addr.sin_port);

	return true;
}

bool CL_UniformUDPConnection::peek()
{
	if (sock == CL_INVALID_SOCKET) return false;

	fd_set rfds;

	FD_ZERO(&rfds);
	FD_SET(sock, &rfds);

	timeval timeout;
	memset(&timeout, 0, sizeof(timeval));

	int retval = select(sock+1, &rfds, NULL, NULL, &timeout);
	if (retval == -1) return false;

	return (retval > 0);
}

CL_UDPConnectionPacket CL_UniformUDPConnection::receive()
{
	char *receive_buf = new char[8096];

	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	socklen_t len_addr = sizeof(addr);
	int res = ::recvfrom(sock, receive_buf, 8096, 0, (sockaddr *) &addr, &len_addr);
	cl_assert(res != CL_SOCKET_ERROR);

	CL_UDPConnectionPacket ret;
	ret.data = receive_buf;
	ret.size = res;
	ret.ip_addr = addr.sin_addr.s_addr;
	ret.port = ntohs(addr.sin_port);

	return ret;
}

void CL_UniformUDPConnection::send(CL_UDPConnectionPacket message)
{
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = message.ip_addr;
	addr.sin_port = htons(message.port);

	cl_assert(sock != CL_INVALID_SOCKET);
	cl_assert(message.size < 2000);

	int res = ::sendto(
		sock,
		(char *) message.data,
		message.size,
		0,
		(sockaddr *) &addr,
		sizeof(sockaddr_in));

	if (res == CL_SOCKET_ERROR)
	{
		cl_info(info_network, PRINT_SOCK_ERROR());
	}

	cl_assert(res != -1);
}

void CL_UniformUDPConnection::broadcast(CL_UDPConnectionPacket message)
{
	cl_assert(sock != CL_INVALID_SOCKET);
	cl_assert(message.size < 2000);

	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_BROADCAST;
	addr.sin_port = htons(message.port);

	int res = ::sendto(
		sock,
		(char *) message.data,
		message.size,
		0,
		(sockaddr *) &addr,
		sizeof(sockaddr_in));
		
	if (res == -1)
	{
		cl_info(info_network, PRINT_SOCK_ERROR());
	}

	cl_assert(res != -1);
}

unsigned int CL_UniformUDPConnection::get_port()
{
	return port;
}

// error messages under win32 are hell as usual:

#ifdef WIN32
const char *get_win32_sock_error(DWORD err)
{
	switch (err)
	{
	case WSAEACCES: return "Permission denied";
	case WSAEADDRINUSE: return "Address already in use";
	case WSAEADDRNOTAVAIL: return "Cannot assign requested address";
	case WSAEAFNOSUPPORT: return "Address family not supported by protocol family";
	case WSAEALREADY: return "Operation already in progress";
	case WSAECONNABORTED: return "Software caused connection abort";
	case WSAECONNREFUSED: return "Connection refused";
	case WSAECONNRESET: return "Connection reset by peer";
	case WSAEDESTADDRREQ: return "Destination address required";
	case WSAEFAULT: return "Bad address";
	case WSAEHOSTDOWN: return "Host is down";
	case WSAEHOSTUNREACH: return "No route to host";
	case WSAEINPROGRESS: return "Operation now in progress";
	case WSAEINTR: return "Interrupted function call";
	case WSAEINVAL: return "Invalid argument";
	case WSAEISCONN: return "Socket is already connected";
	case WSAEMFILE: return "Too many open files";
	case WSAEMSGSIZE: return "Message too long";
	case WSAENETDOWN: return "Network is down";
	case WSAENETRESET: return "Network dropped connection on reset";
	case WSAENETUNREACH: return "Network is unreachable";
	case WSAENOBUFS: return "No buffer space available";
	case WSAENOPROTOOPT: return "Bad protocol option";
	case WSAENOTCONN: return "Socket is not connected";
	case WSAENOTSOCK: return "Socket operation on non-socket";
	case WSAEOPNOTSUPP: return "Operation not supported";
	case WSAEPFNOSUPPORT: return "Protocol family not supported";
	case WSAEPROCLIM: return "Too many processes";
	case WSAEPROTONOSUPPORT: return "Protocol not supported";
	case WSAEPROTOTYPE : return "Protocol wrong type for socket";
	case WSAESHUTDOWN: return "Cannot send after socket shutdown";
	case WSAESOCKTNOSUPPORT: return "Socket type not supported";
	case WSAETIMEDOUT: return "Connection timed out";
//	case WSATYPE_NOT_FOUND: return "Class type not found";
	case WSAEWOULDBLOCK: return "Resource temporarily unavailable";
//	case WSAHOST_NOT_FOUND: return "Host not found";
	case WSANOTINITIALISED: return "Successful WSAStartup not yet performed";
	case WSANO_DATA: return "Valid name, no data record of requested type";
	case WSANO_RECOVERY: return "This is a non-recoverable error";
	case WSASYSNOTREADY: return "Network subsystem is unavailable";
	case WSATRY_AGAIN: return "Non-authoritative host not found";
	case WSAVERNOTSUPPORTED: return "WINSOCK.DLL version out of range";
	case WSAEDISCON: return "Graceful shutdown in progress";
	default: return "Unknown WSA error";
	};

/*
	static char err_buffer[16*1024];
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		err_buffer,
		16*1024,
		NULL);

	return err_buffer;
*/
}
#endif
