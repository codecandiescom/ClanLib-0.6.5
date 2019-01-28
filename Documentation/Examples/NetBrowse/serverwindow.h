/*
	Network BrowseMaster/Client/Server demonstration.
	Copyright (c) 2001 by Magnus Norddahl and Kenneth Gangstoe.
*/

#ifndef _SERVERWINDOW_H_
#define _SERVERWINDOW_H_

#include <ClanLib/gui.h>

class ServerListWindow;
class CL_BrowseServer;

class ServerWindow : public CL_Window
{
public:
	ServerWindow(ServerListWindow *parent);

private:
	void on_create();
	void on_delete();

private:
	CL_Label *label_port;
	CL_Label *label_app_id;
	CL_Label *label_desc;

	CL_InputBox *input_port;
	CL_InputBox *input_app_id;
	CL_InputBox *input_desc;

	CL_Button *button_create;
	CL_Slot slot_create;
	CL_Button *button_delete;
	CL_Slot slot_delete;

	CL_BrowseServer *server;

	ServerListWindow *server_list_window;
};

#endif