/*
	$Id: unix_pipe_connection.cpp,v 1.1 2001/02/15 13:18:52 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_NETWORK

#include <API/Core/System/error.h>
#include "API/Core/System/cl_assert.h"
#include <Core/Network/Generic/network_delivery_impl.h>
#include <Core/Network/Unix/network_delivery_unix.h>
#include <Core/Network/Unix/network_delivery_pipe.h>

#include <string.h>
#include <errno.h>

/******************************************************************************
                      Unix PipeConnection implementation
******************************************************************************/

CL_UnixPipeConnection::CL_UnixPipeConnection(int _read, int _write)
{
	read = _read;
	write = _write;
	is_connection_lost = false;
}

CL_UnixPipeConnection::~CL_UnixPipeConnection()
{
	close(read);
	close(write);
}

bool CL_UnixPipeConnection::peek()
{
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(read, &rfds);

	timeval timeout;
	memset(&timeout, 0, sizeof(timeval));

	int retval = select(read+1, &rfds, NULL, NULL, &timeout);
	return retval > 0;
}

CL_ConnectionPacket CL_UnixPipeConnection::receive()
{
	unsigned long size_data = read_int();

	unsigned char *data = new unsigned char[size_data];
	read_data(data, size_data);

	CL_ConnectionPacket ret;
	ret.size = size_data;
	ret.data = data;

	return ret;
}

void CL_UnixPipeConnection::send(CL_ConnectionPacket message)
{
	write_int(message.size);
	write_data(message.data, message.size);
}

bool CL_UnixPipeConnection::connection_lost()
{
	return is_connection_lost;
}

unsigned long CL_UnixPipeConnection::read_int()
{
	unsigned long ret;
	read_data(&ret, sizeof(unsigned long));

	return ret;
}

void CL_UnixPipeConnection::read_data(void *dest, int size_data)
{
	int total_read = 0;
	while (total_read < size_data)
	{
		int res = ::read(
			read, 
			((unsigned char *) dest)+total_read,
			size_data-total_read);

		if (res == 0)
		{
			std::cout << "OH MY GOD - TIME TO QUIT" << std::endl;
			is_connection_lost = true;
			return;
		}
			
		total_read += res;
	}
}

void CL_UnixPipeConnection::write_int(unsigned long value)
{
	write_data(&value, sizeof(unsigned long));
}

inline void CL_UnixPipeConnection::write_data(void *data, unsigned int size)
{
	::write(write, data, size);
}

#endif // USE_NETWORK


