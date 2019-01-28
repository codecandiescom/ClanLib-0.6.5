/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

#include <ClanLib/core.h>
#include <ClanLib/network.h>

class Document : public CL_KeepAlive
{
public:
	// Construction:
	Document(
		const std::string &hostname, int port,
		const std::string &username, const std::string &password);
	~Document();

	// Attributes:
	std::list<std::string> users;

	// Operations:
	void update_text(const std::string &text);

	// Signals:
	CL_Signal_v2<const std::string &/*user*/, const std::string &/*text*/> sig_text_update;

	CL_Signal_v1<const std::string &/*user*/> sig_user_added;

	CL_Signal_v1<const std::string &/*user*/> sig_user_removed;

	CL_Signal_v0 sig_login_ok;

	CL_Signal_v0 sig_login_fail;

	// implementation:
private:
	virtual void keep_alive();

	CL_NetSession netsession;
};

#endif
