#include <ClanLib/core.h>
#include <ClanLib/network.h>
#include <ClanLib/gl.h>
#include <ClanLib/gui.h>
#include <ClanLib/png.h>
#include <ClanLib/jpeg.h>
#include <ClanLib/display.h>
#include <ClanLib/sound.h>

#include "GUI/stylemanager_opengl.h"
#include "netobjects.h"
#include "mainmenu.h"
#include "intro.h"

Main app; // one and only application instance.

int Main::main(int argc, char **argv)
{
	// Create a console window for text-output if not available
	CL_ConsoleWindow console("NetObjects",80,1000); // 1000 allows a y-scrollbar to be present
	console.redirect_stdio();

	try
	{
		CL_SetupCore::init();
		CL_SetupNetwork::init();
		CL_SetupGL::init();
		CL_SetupGUI::init();
		CL_SetupPNG::init();
		CL_SetupJPEG::init();
//		CL_SetupVorbis::init();
//		CL_SetupTTF::init();
		CL_SetupDisplay::init();
		CL_SetupSound::init();

		// Setup a window for our game:
		CL_Display::set_videomode(640, 480, 32, false);

		CL_OpenGL::begin_2d();

		// Setup resources:
		CL_ResourceManager *resources = new CL_ResourceManager("resources.scr", false);
		resources->load_all();

		// Setup styles for gui:
		CL_StyleManager_OpenGL styles(resources);

//		Intro intro(resources);
//		intro.run();

		// Create main menu and run it modal:
		MainMenu menu(NULL, &styles);
		menu.run();

		CL_OpenGL::end_2d();

		CL_SetupSound::deinit();
		CL_SetupDisplay::deinit();
//		CL_SetupVorbis::deinit();
//		CL_SetupTTF::deinit();
		CL_SetupPNG::deinit();
		CL_SetupJPEG::deinit();
		CL_SetupGUI::deinit();
		CL_SetupGL::deinit();
		CL_SetupNetwork::deinit();
		CL_SetupCore::deinit();
	}
	catch (CL_Error error)
	{
		std::cout << "Error: " << error.message.c_str() << std::endl;

		// Display console close message and wait for a key
		console.display_close_message();
	}

	return 0;
}
