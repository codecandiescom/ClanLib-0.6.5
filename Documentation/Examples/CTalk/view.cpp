/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#include "view.h"
#include "mainframe.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

View::View(MainFrame *mainframe)
: CL_Frame(CL_Rect(0,0,0,0), mainframe), mainframe(mainframe), floating(false),
	title("unnamed view")
{
	// views are not visible per default. Only when set floating or active.
	show(false);
	mainframe->add_view(this);
}

View::~View()
{
	mainframe->remove_view(this);
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:


/////////////////////////////////////////////////////////////////////////////
// Operations:

void View::set_floating(bool new_floating)
{
	floating = new_floating;
	sig_floating(new_floating);
}

void View::set_title(const std::string &new_title)
{
	title = new_title;
	sig_new_title(title);
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:
