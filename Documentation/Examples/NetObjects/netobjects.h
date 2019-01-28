#ifndef header_main
#define header_main

#include <ClanLib/application.h>

class Main : public CL_ClanApplication
{
// Implementation:
private:
	virtual char *get_title() { return "ClanLib NetObjects Example"; }
	virtual int main(int argc, char **argv);
};

#endif
