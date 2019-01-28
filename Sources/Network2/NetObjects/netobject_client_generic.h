
#ifndef header_netobject_client_generic
#define header_netobject_client_generic

#include "API/signals.h"
#include "API/Network2/NetSession/netcomputer.h"
#include <map>

class CL_NetObject_Controller_Generic;

class CL_NetObject_Client_Generic
{
//! Construction:
public:
	CL_NetObject_Client_Generic(int obj_id, const CL_NetComputer &server, CL_NetObject_Controller_Generic *controller);

	~CL_NetObject_Client_Generic();

//! Attributes:
public:
	int obj_id;

	CL_NetComputer server;

	CL_NetObject_Controller_Generic *controller;

	std::map< int, CL_Signal_v1<CL_NetPacket &> > sig_received_message;

//! Operations:
public:
	void receive(int msg_type, CL_NetPacket &packet);

	void send(int msg_type, const CL_NetPacket &message);

	int add_ref() { return ++ref_count; }

	int release_ref()
	{
		ref_count--;
		if (ref_count == 0) { delete this; return 0; }
		return ref_count;
	}

//! Implementation:
private:
	int ref_count;
};

#endif
