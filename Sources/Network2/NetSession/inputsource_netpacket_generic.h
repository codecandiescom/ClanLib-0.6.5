
#ifndef header_inputsource_netpacket_generic
#define header_inputsource_netpacket_generic

class CL_NetPacket_Generic;

class CL_InputSource_NetPacket_Generic
{
//! Construction:
public:
	CL_InputSource_NetPacket_Generic(CL_NetPacket_Generic *netpacket, bool ref_count);

	~CL_InputSource_NetPacket_Generic();

//! Attributes:
public:
	CL_NetPacket_Generic *netpacket;

	int position;

//! Implementation:
private:
	bool ref_count;
};

#endif
