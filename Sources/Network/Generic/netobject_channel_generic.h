/*
	$Id: netobject_channel_generic.h,v 1.3 2001/03/15 12:14:47 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_netobject_channel_generic
#define header_netobject_channel_generic

#include <map>
#include <string>
#include <queue>
#include "API/Core/System/keep_alive.h"
#include "API/Network/netobject.h"

class CL_NetObjectChannel_Generic : public CL_KeepAlive
{
public:
	CL_NetObjectChannel_Generic(class CL_NetSession *netgame, int channel, CL_NetObjectChannel *owner);
	~CL_NetObjectChannel_Generic();

	int add_ref();
	int release_ref();

	void begin_sync(const CL_NetGroup *group);
	void end_sync(const CL_NetGroup *group);
	bool wait_sync(int timeout = -1);

	void send(int obj_id, int msg_type, const std::string &message);
	void talkback(int obj_id, int talkback_type, const std::string &message);

public:
	virtual void keep_alive();

	int ref;
	int id_counter;
	int channel;

	CL_NetSession *netgame;
	CL_NetObjectChannel *owner;

	CL_Signal_v3<const class CL_NetObject &, int, const std::string &> sig_create_object;

	std::map<int, CL_NetObject_Generic *> objects;
	std::queue<bool> received_sync;
};

#endif
