#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

#include "component_type_window.h"
#include "mainframe.h"

// The main application
class GUIEditor : public CL_ClanApplication
{
public:
	char *get_title() { return "GUIEditor"; }

	CL_ResourceManager *resources;
	CL_GUIManager *gui;
	CL_StyleManager_Default *style;
		
	int main(int argc, char** argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore::init();
			CL_SetupDisplay::init();
			CL_SetupGUI::init();

			CL_Display::set_videomode(800, 600, 32, false);
			CL_Display::clear_display(235.0f/255.0f, 235.0f/255.0f, 227.0f/255.0f);

			// Create the GUI using the default theme
			resources = new CL_ResourceManager("../GUIThemeDefault/gui.scr", false);
			style = new CL_StyleManager_Default(resources);
			gui = new CL_GUIManager(style);

			CL_Slot slot_quit = CL_System::sig_quit().connect(this, &GUIEditor::on_quit);

			MainFrame mainframe(gui);
			mainframe.set_position(CL_Rect(0,0,800,600));
			gui->run();

			delete gui;
			delete style;
			delete resources;

			CL_SetupGUI::deinit();
			CL_SetupDisplay::deinit();
			CL_SetupCore::deinit();
		}
		catch (CL_Error e)
		{
 			std::cout << e.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}

	void on_paint()
	{
		CL_Display::clear_display(235.0f/255.0f, 235.0f/255.0f, 227.0f/255.0f);
	}

	void on_quit()
	{
		gui->quit();
	}

} app;
