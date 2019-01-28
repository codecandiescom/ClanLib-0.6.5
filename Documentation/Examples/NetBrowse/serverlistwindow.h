/*
	Network BrowseMaster/Client/Server demonstration.
	Copyright (c) 2001 by Magnus Norddahl and Kenneth Gangstoe.
*/

#ifndef _SERVERLISTWINDOW_H_
#define _SERVERLISTWINDOW_H_

#include <ClanLib/gui.h>
#include <ClanLib/network.h>

class ServerListWindow : public CL_Frame
{
public:
	ServerListWindow(CL_Component *parent);

	CL_IPAddress get_master_ip() const;

// Callbacks
private:
	void on_create();
	void on_connect();

	void on_server_added(const CL_IPAddress &ip, const std::string &desc);
	void on_server_updated(const CL_IPAddress &ip, const std::string &desc);
	void on_server_removed(const CL_IPAddress &ip);

	void update_list();

private:
	CL_ResourceManager *resources;

	CL_GUIManager *gui;
	CL_StyleManager_Default *style;

	CL_Label *label_address;
	CL_InputBox *input_port;
	CL_InputBox *input_ip;
	CL_Label *label_dns;
	CL_ListBox *list_servers;
	CL_Button *button_connect;
	CL_Button *button_create;

	CL_BrowseClient *client;

	CL_Slot slot_connect;
	CL_Slot slot_create;

	CL_Slot slot_server_added;
	CL_Slot slot_server_updated;
	CL_Slot slot_server_removed;

	std::map<CL_IPAddress, std::string> servers;
};

#endif