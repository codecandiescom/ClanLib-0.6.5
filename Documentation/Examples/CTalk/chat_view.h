/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#ifndef _CHAT_VIEW_H_
#define _CHAT_VIEW_H_

#include "view.h"

class MainFrame;
class ChatComponent;
class UserListComponent;

class ChatView : public View
{
public:
	// Construction:
	ChatView(MainFrame *parent);
	~ChatView();
	
	// Attributes:
	
	// Operations:
	
	// Signals:
	
	// implementation:
private:
	// events:
	void on_paint();

	CL_SlotContainer slots;

	ChatComponent *chat;
	UserListComponent *userlist;
};

#endif
