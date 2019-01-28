/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#ifdef WIN32
#pragma warning(disable : 4355)
#endif

#include "login_view.h"
#include <ClanLib/display.h>
#include <ClanLib/gui.h>

/////////////////////////////////////////////////////////////////////////////
// LoginView construction:

LoginView::LoginView(MainFrame *parent)
: View(parent), components("login_view", this)
{
	// Load and connect frame controls:
	components.get_component("cancel", &cancel);
	components.get_component("username", &username);
	components.get_component("password", &password);
	components.get_component("ok", &ok);

	// Connect events to signals:
	slots.connect(sig_paint(), this, &LoginView::on_paint);
	slots.connect(cancel->sig_clicked(), this, &LoginView::on_cancel);
	slots.connect(password->sig_return_pressed(), this, &LoginView::on_enter);
	slots.connect(ok->sig_clicked(), this, &LoginView::on_enter);

	set_title("Login");
}

LoginView::~LoginView()
{
}

/////////////////////////////////////////////////////////////////////////////
// LoginView attributes:

/////////////////////////////////////////////////////////////////////////////
// LoginView operations:

void LoginView::set_username(const std::string &new_username)
{
	username->set_text(new_username);
}

/////////////////////////////////////////////////////////////////////////////
// LoginView events:

void LoginView::on_paint()
{
	CL_Display::fill_rect(0, 0, get_width(), get_height(), 1.0f, 1.0f, 1.0f);
}

void LoginView::on_enter()
{
	sig_login(username->get_text(), password->get_text());
}

void LoginView::on_cancel()
{
	sig_cancel();
}
