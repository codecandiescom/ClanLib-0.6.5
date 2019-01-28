
#ifndef header_netpacket_generic
#define header_netpacket_generic

class CL_NetPacket_Generic
{
//! Construction:
public:
	CL_NetPacket_Generic();

	~CL_NetPacket_Generic();

//! Attributes:
public:
	//: Packet data.
	unsigned char *data;

	//: Packet size.
	int size;

//! Operations:
public:
	int add_ref();

	int release_ref();

	void resize(int new_size);

//! Implementation:
private:
	int allocated_size;

	int ref_count;
};

#endif
