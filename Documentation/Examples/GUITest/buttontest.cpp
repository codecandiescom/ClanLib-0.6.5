#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

class ButtonTest : public CL_ClanApplication
{
public:
	char *get_title() { return "ButtonTest"; }

	CL_ComponentManager *manager;
	
	int main(int argc, char** argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();
		
		try
		{
			CL_SetupCore::init();
			CL_SetupGUI::init();
			CL_SetupDisplay::init();
			CL_Display::set_videomode(640,480,16, false);

			CL_ResourceManager resources("../GUIThemeDefault/gui.scr", false);
			CL_StyleManager_Default style(&resources);
			CL_GUIManager gui(&style);

			manager = CL_ComponentManager::create("buttontest.gui", false, &style, &gui);
			
			while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
			{
				CL_Display::clear_display(0.60f, 0.60f, 0.60f);
				gui.show();
				CL_System::keep_alive();
				CL_Display::flip_display();
			}
			
			delete manager;

			CL_SetupDisplay::deinit();
			CL_SetupGUI::deinit();
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
} app;
