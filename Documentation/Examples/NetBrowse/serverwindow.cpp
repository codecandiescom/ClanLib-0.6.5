#include "serverwindow.h"
#include "serverlistwindow.h"

ServerWindow::ServerWindow(ServerListWindow *parent)
: CL_Window(CL_Rect(100, 100, 400, 255), parent)
{
	server_list_window = parent;

	label_port = new CL_Label(
		CL_Point(10, 12), "Port: ", get_client_area());
	input_port = new CL_InputBox(
		CL_Rect(75, 10, 115, 30), "", get_client_area());

	label_app_id = new CL_Label(
		CL_Point(10, 37), "App-id: ", get_client_area());
	input_app_id = new CL_InputBox(
		CL_Rect(75, 35, 285, 55), "", get_client_area());

	label_desc = new CL_Label(
		CL_Point(10, 62), "Description: ", get_client_area());
	input_desc = new CL_InputBox(
		CL_Rect(75, 60, 285, 80), "", get_client_area());

	button_create = new CL_Button(
		CL_Rect(100, 100, 200, 120), "Create server", get_client_area());
	button_delete = new CL_Button(
		CL_Rect(100, 100, 200, 120), "Delete server", get_client_area());
	button_delete->show(false);

	slot_create = button_create->sig_clicked().connect(
		this, &ServerWindow::on_create);
	slot_delete = button_delete->sig_clicked().connect(
		this, &ServerWindow::on_delete);
}

void ServerWindow::on_create()
{
	if(input_port->get_text().empty())
	{
		CL_MessageBox::info("You need to fill in the port", this);
		return;
	}

	if(input_app_id->get_text().empty())
	{
		CL_MessageBox::info("You need to fill in the app-id", this);
		return;
	}

	CL_IPAddress master_ip = server_list_window->get_master_ip();
	CL_IPAddress server_ip("localhost", 1234);

	std::string app_id;
	std::string description;

	try
	{
		server = new CL_BrowseServer(master_ip, app_id, server_ip, description);
	}
	catch(CL_Error err)
	{
		CL_MessageBox::info(err.message, this);
		return;
	}

	button_create->show(false);
	button_delete->show(true);
}

void ServerWindow::on_delete()
{
	button_create->show(true);
	button_delete->show(false);
}