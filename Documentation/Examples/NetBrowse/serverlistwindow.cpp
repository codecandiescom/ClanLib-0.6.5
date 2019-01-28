#include <ClanLib/display.h>

#include "serverlistwindow.h"
#include "serverwindow.h"

ServerListWindow::ServerListWindow(CL_Component *parent)
: CL_Frame(CL_Rect(0, 0, CL_Display::get_width(), CL_Display::get_height()), parent)
{
	label_address = new CL_Label(
		CL_Point(10, 12), "IP:", this);
	input_ip = new CL_InputBox(
		CL_Rect(30, 10, 120, 25), "130.225.92.246", this);
	input_port = new CL_InputBox(
		CL_Rect(120, 10, 160, 25), "6545", this);

	button_connect = new CL_Button(
		CL_Rect(10, 35, 160, 55), "Connect to browse master", this);

	label_dns = new CL_Label(
		CL_Rect(180, 12, 500, 25), "", this);

	list_servers = new CL_ListBox(
		CL_Rect(10, 75, 630, 470), this);

	button_create = new CL_Button(CL_Rect(480, 35, 630, 55), "Create new server", this);

	slot_connect = button_connect->sig_clicked().connect(
		this, &ServerListWindow::on_connect);
	slot_create = button_create->sig_clicked().connect(
		this, &ServerListWindow::on_create);

	enable_fill();
}

CL_IPAddress ServerListWindow::get_master_ip() const
{
	CL_String port = input_port->get_text();
	CL_IPAddress ip(input_ip->get_text(), port.get_as_int());

	return ip;
}

void ServerListWindow::on_create()
{
	ServerWindow *window = new ServerWindow(this);
	add_child(window);
}

void ServerListWindow::on_connect()
{
	CL_IPAddress ip = get_master_ip();

	try
	{
		label_dns->set_text(ip.dns_lookup());
	}
	catch (CL_Error error)
	{
		label_dns->set_text(error.message);
	}

	try
	{
		client = new CL_BrowseClient(ip, "NetBrowse Example");
	}
	catch (CL_Error err)
	{
		CL_MessageBox::info(err.message, this);
		return;
	}

	slot_server_added = client->sig_server_added().connect(
		this, &ServerListWindow::on_server_added);

	slot_server_updated = client->sig_server_updated().connect(
		this, &ServerListWindow::on_server_updated);

	slot_server_removed = client->sig_server_removed().connect(
		this, &ServerListWindow::on_server_removed);
}

void ServerListWindow::on_server_added(const CL_IPAddress &ip, const std::string &desc)
{
	servers[ip] = desc;
	update_list();	// temporary hack: fully refresh list
}

void ServerListWindow::on_server_updated(const CL_IPAddress &ip, const std::string &desc)
{
	servers[ip] = desc;
	update_list();	// temporary hack: fully refresh list
}

void ServerListWindow::on_server_removed(const CL_IPAddress &ip)
{
	servers.erase(servers.find(ip));
	update_list();	// temporary hack: fully refresh list
}

void ServerListWindow::update_list()
{
	list_servers->clear();

	std::map<CL_IPAddress, std::string>::iterator it;
	for (it = servers.begin(); it != servers.end(); it++)
		list_servers->insert_item((*it).second);
}
