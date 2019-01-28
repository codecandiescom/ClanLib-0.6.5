/*
	$Id: unix_pipe_connection.h,v 1.3 2001/09/08 19:24:20 japj Exp $
	
	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Network delivery implementation - Unix sockets version.
*/

#ifdef USE_NETWORK

#ifndef header_network_delivery_pipe
#define header_network_delivery_pipe

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <list>

#include "Network/Generic/network_delivery_impl.h"

class CL_UnixPipeConnection : public CL_Connection
{
	int read, write;
	bool is_connection_lost;

	unsigned long read_int();
	void read_data(void *dest, int size_data);
	void write_int(unsigned long value);
	inline void write_data(void *data, unsigned int size);

public:
	CL_UnixPipeConnection(int read, int write);
	virtual ~CL_UnixPipeConnection();

	int get_socket() { return read; }

	// CL_Connection inherited functions
	virtual bool peek();
	virtual CL_ConnectionPacket receive();
	virtual void send(CL_ConnectionPacket message);
	virtual bool connection_lost();
};


#endif

#endif // USE_NETWORK
