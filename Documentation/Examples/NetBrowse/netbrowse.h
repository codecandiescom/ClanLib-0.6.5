/*
	Network BrowseMaster/Client/Server demonstration.
	Copyright (c) 2001 by Magnus Norddahl and Kenneth Gangstoe.
*/

#ifndef _NETBROWSE_H_
#define _NETBROWSE_H_

#include <ClanLib/application.h>

// The main application
class App : public CL_ClanApplication
{
public:
	virtual char *get_title();
	virtual int main(int argc, char** argv);
} app;

#endif