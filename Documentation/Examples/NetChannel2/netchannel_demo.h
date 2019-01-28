
#ifndef header_netchannel_demo
#define header_netchannel_demo

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/network2.h>
#include <ClanLib/application.h>

#include <stdlib.h>

class NetChannelDemo : public CL_ClanApplication
{
public:
	virtual char *get_title();
	virtual int main(int argc, char **argv);
	
	bool run_server();
	void run_client();
protected:
	void on_client_disconnect(CL_NetComputer &computer);
	void on_server_disconnect(CL_NetComputer &computer);
	void on_server_connect(CL_NetComputer &computer);
	void on_message_receive(CL_NetPacket &packet, CL_NetComputer &computer);
	void on_ping_receive(CL_NetPacket &packet, CL_NetComputer &computer);
	void send_ping();
	
	CL_Slot slot_connect;
	CL_Slot slot_disconnect;
	CL_Slot slot_receive_ping;
	CL_Slot slot_receive_message;

	CL_NetSession *netgame;
};

#endif
