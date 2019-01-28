/*
	$Id: network_delivery_socket.h,v 1.4 2001/11/01 20:27:57 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Lowlevel socket communication classes.
*/

#ifndef header_network_delivery_socket
#define header_network_delivery_socket

#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/time.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <netinet/in.h>
	#include <unistd.h>
	#include <errno.h>
	#include <fcntl.h>
#endif

#include <string.h>
#include <list>
#include <queue>

#include "Network/Generic/network_delivery_impl.h"

#ifdef WIN32
#define CL_INVALID_SOCKET INVALID_SOCKET
#define CL_SOCKET_ERROR SOCKET_ERROR
#define CL_SOCKET SOCKET
const char *get_win32_sock_error(DWORD err);
#define PRINT_SOCK_ERROR() get_win32_sock_error(WSAGetLastError())
typedef int socklen_t;
#else
#include <netdb.h>
#define CL_INVALID_SOCKET -1
#define CL_SOCKET_ERROR -1
#define closesocket(a) close(a)
#define PRINT_SOCK_ERROR() strerror(errno)
#define CL_SOCKET int
// where is this one defined??
#define TCP_NODELAY 1
#endif

class CL_Connections_Unix;
class CL_UDPConnection;

class CL_UniformSocket : public CL_Connection
{
public:
	virtual int get_socket() { return sock; }

	CL_UniformSocket(CL_ConnectionProvider *provider);
	virtual ~CL_UniformSocket();

	// CL_UniformSocket defined virtual function
	virtual bool try_connect(unsigned long remote_ip_network_format, int port);
	virtual bool init_socket(int init_socket=-1);

	// CL_Connection inherited functions
	virtual bool peek();
	virtual CL_ConnectionPacket receive();
	virtual void send(CL_ConnectionPacket message);
	virtual bool connection_lost();
	virtual unsigned long get_peer_address();
	virtual unsigned short get_peer_port();
	virtual void disconnect();

	bool send_avail();

protected:
	CL_SOCKET sock;
	bool is_connection_lost;

	char *cur_message;
	int cur_message_size;

	unsigned long read_int();
	void write_int(unsigned long value);
	inline void write_data(void *data, unsigned int size);

	// read from socket:
	enum
	{
		expect_magic,
		expect_packet_size,
		expect_packet_data,
		packet_finished
	} recv_state;

	bool read_avail();
	bool require_avail(unsigned int size);
	bool get_avail(void *buf, unsigned int size);

	CL_ConnectionProvider *provider;

private:
//	std::deque<char> recv_buffer;
	std::string recv_buffer;
	std::queue<std::string> send_buffer;
	int send_pos;
};

class CL_UniformAcceptSocket : public CL_UniformSocket
{
public:
	CL_UniformAcceptSocket(CL_ConnectionProvider *provider);
	virtual ~CL_UniformAcceptSocket();

	bool bind(int port);

	CL_UniformSocket *accept();
	int get_port() { return port; }

	// CL_UniformSocket inherited functions
	virtual bool try_connect(unsigned long remote_ip_network_format, int port);

	// CL_Connection inherited functions
	virtual bool peek();
	virtual CL_ConnectionPacket receive();
	virtual void send(CL_ConnectionPacket message);
	virtual bool connection_lost();

private:
	bool is_inited;
	int port;
};


class CL_UniformUDPConnection : public CL_UDPConnection
{
public:
	virtual int get_socket() { return sock; }

	CL_UniformUDPConnection();
	virtual ~CL_UniformUDPConnection();

	bool bind(unsigned int port);

	// CL_UDPConnection inherited functions
	virtual bool peek();
	virtual CL_UDPConnectionPacket receive();
	virtual void send(CL_UDPConnectionPacket message);
	virtual void broadcast(CL_UDPConnectionPacket message);
	virtual unsigned int get_port();

protected:
	int sock;
	int port;
};

#endif
