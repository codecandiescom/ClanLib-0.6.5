
#ifndef header_outputsource_netpacket_generic
#define header_outputsource_netpacket_generic

class CL_NetPacket_Generic;

class CL_OutputSource_NetPacket_Generic
{
//! Construction:
public:
	CL_OutputSource_NetPacket_Generic(CL_NetPacket_Generic *netpacket, bool ref_count);

	~CL_OutputSource_NetPacket_Generic();

//! Attributes:
public:
	CL_NetPacket_Generic *netpacket;

	int position;

//! Implementation:
private:
	bool ref_count;
};

#endif
