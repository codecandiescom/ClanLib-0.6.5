/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#ifndef _LOGIN_VIEW_H_
#define _LOGIN_VIEW_H_

#include "mainframe.h"
#include "view.h"

class LoginView : public View
{
public:
	// Construction:
	LoginView(MainFrame *parent);
	~LoginView();
	
	// Attributes:
	
	// Operations:
	void set_username(const std::string &username);
	
	// Signals:
	CL_Signal_v2<const std::string &, const std::string &> sig_login; // sig_login(name, password)
	CL_Signal_v0 sig_cancel;
	
	// implementation:
private:
	CL_ComponentManager components;
	CL_SlotContainer slots;
 	CL_Button *ok;
	CL_Button *cancel;
	CL_InputBox *username;
	CL_InputBox *password;

	// events:
	void on_paint();
	void on_enter();
	void on_cancel();
};

#endif
