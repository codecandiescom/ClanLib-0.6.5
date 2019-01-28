/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#ifndef _DOCUMENT_SERVER_
#define _DOCUMENT_SERVER_

#include <ClanLib/core.h>
#include <ClanLib/network.h>
#include <map>

class User
{
public:
	std::string name;
	std::string text;
};

class DocumentServer : public CL_KeepAlive
{
public:
	// Construction:
	DocumentServer(int port);

	// Attributes:
	std::map<std::string, std::string> accounts;
	std::map<CL_NetComputer, User> logins;

	// Operations:

	// implementation:
private:
	virtual void keep_alive();

	CL_NetSession netsession;
};

#endif
