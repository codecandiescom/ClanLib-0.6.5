/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#ifndef _CTALK_H_
#define _CTALK_H_

#include <ClanLib/application.h>

class MainFrame;
class LoginView;
class ChatView;

class CTalk : public CL_ClanApplication
{
public:
	virtual int main(int argc, char **argv);
	virtual char *get_title() { return "CTalk"; }
	
private:
	MainFrame *mainframe;
	LoginView *login_view;
	ChatView *chat_view;

	void server_loop();
	// Server application loop.

	void main_loop();
	// Main application loop when gui is running.

	void on_login(const std::string &username, const std::string &password);
	// Called when login button is clicked in login view.

	void on_resize(int width, int height);
	// Called when application window is resized.
};

#endif
