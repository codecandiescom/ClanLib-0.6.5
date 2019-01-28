/*
	$Id: netobject_generic.h,v 1.5 2001/11/12 20:32:32 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_netobject_generic
#define header_netobject_generic

#include <map>
#include <string>
#include "../../API/signals.h"

class CL_NetObject_Generic
{
public:
	CL_NetObject_Generic(class CL_NetObjectChannel_Generic *obj_channel);
	CL_NetObject_Generic(int obj_id, class CL_NetObjectChannel_Generic *obj_channel);
	~CL_NetObject_Generic();

	int get_obj_id() const { return obj_id; }

	CL_Slot connect(int msg_type, CL_Slot_v1<class CL_InputSource &> *slot);
	CL_Slot connect(int talkback_type, CL_Slot_v2<const class CL_NetComputer &, class CL_InputSource &> *slot);
	void send(int msg_type, const std::string &message);
	void talkback(int talkback_type, const std::string &message);

	int add_ref();
	int release_ref();

public:
	std::map<int, CL_Signal_v1<CL_InputSource &> > msg_signals;
	std::map<int, CL_Signal_v2<const CL_NetComputer &, CL_InputSource &> > talkback_signals;
	class CL_NetObjectChannel_Generic *obj_channel;
	int obj_id;
	bool server_side;
	int ref;
};

#endif
