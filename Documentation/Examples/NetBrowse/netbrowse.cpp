#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/network.h>
#include <ClanLib/display.h>

#include "netbrowse.h"
#include "serverlistwindow.h"

char *App::get_title()
{
	return "NetBrowse Example";
}

int App::main(int argc, char** argv)
{
	// Create a console window for text-output if not available
	CL_ConsoleWindow console("Console");
	console.redirect_stdio();

	try
	{
		CL_SetupCore::init();
		CL_SetupNetwork::init();
		CL_SetupDisplay::init();
		CL_SetupGUI::init();

		CL_Display::set_videomode(640, 480, 32, false);

		// Create the GUI using the default theme
		CL_ResourceManager resources("resources.scr", false);
		CL_StyleManager_Default style(&resources);
		CL_GUIManager gui(&style);

		ServerListWindow server_list_window(&gui);

		// Main loop
		while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
		{
			gui.show();

			CL_Display::flip_display();

			CL_System::keep_alive();
			CL_System::sleep(10);
		}

		CL_SetupGUI::deinit();
		CL_SetupDisplay::deinit();
		CL_SetupNetwork::deinit();
		CL_SetupCore::deinit();
	}
	catch (CL_Error e)
	{
 		std::cout << "Error:" << e.message.c_str() << std::endl;

		// Display console close message and wait for a key
		console.display_close_message();
	}

	return 0;
}