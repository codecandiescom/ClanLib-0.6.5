
#ifndef header_netchannel_demo
#define header_netchannel_demo

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/network.h>
#include <ClanLib/application.h>

class NetChannelDemo : public CL_ClanApplication
{
public:
	virtual char *get_title();
	virtual int main(int argc, char **argv);
	
	bool run_server();
	void run_client();
};

#endif
