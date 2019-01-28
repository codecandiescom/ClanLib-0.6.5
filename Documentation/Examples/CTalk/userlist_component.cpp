/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#include "userlist_component.h"
#include "document.h"

#include <ClanLib/display.h>
#include <ClanLib/gui.h>

/////////////////////////////////////////////////////////////////////////////
// Construction:

UserListComponent::UserListComponent(CL_Component *_parent)
: CL_Frame(CL_Rect(0,0,0,0), _parent), parent(_parent)
{
	list = new CL_ListBox(CL_Rect(0,0,100,100), this);

	// Connect events to signals:
	slots.connect(sig_paint(), this, &UserListComponent::on_paint);
	slots.connect(sig_resize(), this, &UserListComponent::on_resize);
//	slots.connect(parent->get_document()->sig_user_added, this, &UserListComponent::on_user_added);
//	slots.connect(parent->get_document()->sig_user_removed, this, &UserListComponent::on_user_removed);

//	Document is not done yet, so add some dummy data instead:

	on_user_added("Sphair");
	on_user_added("Mbn");
}

UserListComponent::~UserListComponent()
{
}
	
/////////////////////////////////////////////////////////////////////////////
// Attributes:
	
/////////////////////////////////////////////////////////////////////////////
// Operations:

/////////////////////////////////////////////////////////////////////////////
// Events:

void UserListComponent::on_paint()
{
	CL_Display::fill_rect(0, 0, get_width(), get_height(), 0.46f, 0.82f, 0.65f);
}

void UserListComponent::on_resize(int old_width, int old_height)
{
	list->set_size(get_width(), get_height());
}

void UserListComponent::on_user_added(const std::string &user)
{
	list->insert_item(user);
}

void UserListComponent::on_user_removed(const std::string &user)
{
}