/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#include "chat_view.h"
#include "chat_component.h"
#include "userlist_component.h"

#include <ClanLib/display.h>
#include <ClanLib/gui.h>

/////////////////////////////////////////////////////////////////////////////
// Construction:

ChatView::ChatView(MainFrame *parent)
: View(parent)
{
	// Connect events to signals:
	slots.connect(sig_paint(), this, &ChatView::on_paint);

	chat = new ChatComponent(this);
	userlist = new UserListComponent(this);

	chat->set_size(get_width() - 100, get_height());
	userlist->set_size(100, get_height());
	userlist->set_position(get_width() - 100, 0);

	set_title("Chat");
}

ChatView::~ChatView()
{
}
	
/////////////////////////////////////////////////////////////////////////////
// Attributes:
	
/////////////////////////////////////////////////////////////////////////////
// Operations:

/////////////////////////////////////////////////////////////////////////////
// Events:

void ChatView::on_paint()
{
//	CL_Display::fill_rect(0, 0, get_width(), get_height(), 0.0f, 1.0f, 0.0f);
}
