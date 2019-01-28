/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#ifndef _USERLIST_VIEW_H_
#define _USERLIST_VIEW_H_

#include "mainframe.h"

class UserListComponent : public CL_Frame
{
public:
	// Construction:
	UserListComponent(CL_Component *parent);
	~UserListComponent();
	
	// Attributes:
	
	// Operations:
	
	// Signals:
	
	// implementation:
private:
	CL_Component *parent;

	// The list of users
	CL_ListBox *list;

	CL_SlotContainer slots;

	// events:
	void on_paint();

	void on_resize(int old_width, int old_height);

	void on_user_added(const std::string &user);

	void on_user_removed(const std::string &user);
};

#endif
