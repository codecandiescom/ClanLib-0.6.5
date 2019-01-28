/*
	$Id: netobject_channel_generic.cpp,v 1.5 2001/11/12 20:32:32 mbn Exp $

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

#include "API/Core/IOData/inputsource_memory.h"
#include "API/Core/IOData/outputsource_memory.h"
#include "API/Core/System/system.h"
#include "API/Network/netsession.h"
#include "API/Network/netmessage.h"
#include "API/Network/netcomputer.h"
#include "API/Network/netgroup.h"
#include "netobject_channel_generic.h"
#include "netobject_generic.h"
#include <iostream>

CL_NetObjectChannel_Generic::CL_NetObjectChannel_Generic(
	class CL_NetSession *_netgame,
	int _channel,
	CL_NetObjectChannel *_owner)
: ref(0), id_counter(0), channel(_channel), netgame(_netgame), owner(_owner)
{
}

CL_NetObjectChannel_Generic::~CL_NetObjectChannel_Generic()
{
}

int CL_NetObjectChannel_Generic::add_ref()
{
	return ++ref;
}

int CL_NetObjectChannel_Generic::release_ref()
{
	return --ref;
}

void CL_NetObjectChannel_Generic::send(int obj_id, int msg_type, const std::string &message)
{
	CL_OutputSource_Memory output;
	output.write_int32(obj_id);
	output.write_int32(msg_type);
	output.write(message.data(), message.size());

	// std::cout << "Sending netobject message. obj_id = " << obj_id << ", msg_type = " << msg_type << std::endl;
	netgame->send(channel, netgame->get_all(), CL_NetMessage(output.get_data()));
}

void CL_NetObjectChannel_Generic::talkback(int obj_id, int talkback_type, const std::string &message)
{
	CL_OutputSource_Memory output;
	output.write_int32(obj_id);
	output.write_int32(talkback_type);
	output.write(message.data(), message.size());

	// std::cout << "Sending netobject talkback message. obj_id = " << obj_id << ", talkback_type = " << talkback_type << std::endl;
	netgame->send(channel, netgame->get_server(), CL_NetMessage(output.get_data()));
}

void CL_NetObjectChannel_Generic::begin_sync(const CL_NetGroup *group)
{
}

void CL_NetObjectChannel_Generic::end_sync(const CL_NetGroup *group)
{
	CL_OutputSource_Memory output;
	output.write_int32(-1);

	netgame->send(channel, *group, CL_NetMessage(output.get_data()));
}

bool CL_NetObjectChannel_Generic::wait_sync(int timeout)
{
	int start_time = (int)CL_System::get_time();

	while (true)
	{
		CL_System::keep_alive();
		if (!received_sync.empty())
		{
			received_sync.pop();
			return true;
		}

		if (timeout != -1) continue;
		if ((int)CL_System::get_time() -start_time - timeout >= 0) break;
	}

	return false;
}

void CL_NetObjectChannel_Generic::keep_alive()
{
	while (netgame->peek(channel))
	{
		CL_NetMessage netmsg = netgame->receive(channel, 0);
		CL_InputSource_Memory input(netmsg.data);
		int obj_id = input.read_int32();

		if (obj_id == -1 && netmsg.data.size() == sizeof(int)) // sync message
		{
			// std::cout << "Received netobject sync message" << std::endl;
			received_sync.push(true);
			continue;
		}

		int msg_type = input.read_int32();
		// std::cout << "Received netobject message. obj_id = " << obj_id << ", msg_type = " << msg_type << std::endl;

		if (netmsg.from == netgame->get_server()) // message received from server:
		{
			std::map<int, CL_NetObject_Generic*>::iterator it = objects.find(obj_id);
			if (it == objects.end()) // object not found, create a new one.
			{
				sig_create_object(CL_NetObject(obj_id, owner), msg_type, input.get_data().substr(8));
				continue;
			}

			it->second->msg_signals[msg_type](input);
		}
		else // message received from client (talkback)
		{
			std::map<int, CL_NetObject_Generic*>::iterator it = objects.find(obj_id);
			if (it == objects.end()) // object not found, ignore message
				continue;

			it->second->talkback_signals[msg_type](netmsg.from, input);
		}
	}
}
