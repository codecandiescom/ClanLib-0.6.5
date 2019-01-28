/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gui.h>

#include "ctalk.h"
#include "mainframe.h"
#include "login_view.h"
#include "chat_view.h"
#include "document.h"
#include "document_server.h"

CTalk app; // single global instance of the application.

int CTalk::main(int argc, char **argv)
{
#ifdef _DEBUG
	CL_ConsoleWindow console("CTalk debug window");
	console.redirect_stdio();
#endif

	try
	{
		if (argc == 2 && strcmp(argv[1], "-server") == 0) // server mode
		{
			CL_SetupCore::init();
			CL_SetupNetwork::init();
			server_loop();
			CL_SetupNetwork::deinit();
			CL_SetupCore::deinit();
			return 0;
		}

		CL_SetupCore::init();
		CL_SetupGUI::init();
		CL_SetupDisplay::init();
		CL_SetupNetwork::init();
		CL_Display::set_videomode(640, 480, 16, false, false /*should be true, but broken*/);
		
		main_loop();
		
		CL_SetupNetwork::deinit();
		CL_SetupDisplay::deinit();
		CL_SetupGUI::deinit();
		CL_SetupCore::deinit();
	}
	catch (CL_Error error)
	{
		std::cout << "Error: " << error.message.c_str() << std::endl;
#ifdef _DEBUG
		console.display_close_message();
#endif
		return 255;
	}

	return 0;
}

void CTalk::server_loop()
{
	DocumentServer server(6432);
	while (true)
	{
		CL_System::keep_alive(100);
	}
}

void CTalk::main_loop()
{
	CL_ResourceManager resources("resources.scr", false);
	resources.add_resources(CL_ResourceManager("../GUITest/resources.scr", false));

	CL_StyleManager_Default styles(&resources);

	CL_Slot slot_resize = CL_Display::sig_resized().connect(this, &CTalk::on_resize);

	mainframe = new MainFrame(&resources, NULL, &styles);
	login_view = new LoginView(mainframe);

	CL_Slot slot_cancel = login_view->sig_cancel.connect((CL_Component*) login_view, &CL_Component::quit);
	CL_Slot slot_login = login_view->sig_login.connect(this, &CTalk::on_login);

	login_view->set_username("nobody");

	mainframe->set_active_view(login_view);
	mainframe->run();

	delete login_view;
	delete mainframe;
}

void CTalk::on_login(const std::string &username, const std::string &password)
{
	if (mainframe->get_document() == NULL)
	{
		mainframe->set_document(new Document("localhost", 6432, username, password));
		chat_view = new ChatView(mainframe);
	}
	mainframe->set_active_view(chat_view);
}

void CTalk::on_resize(int width, int height)
{
	mainframe->set_position(CL_Rect(0, 0, width, height));
}
