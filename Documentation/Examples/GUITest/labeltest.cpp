#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>

class LabelTest : public CL_ClanApplication
{
public:
	char *get_title() { return "LabelTest"; }

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
			CL_ComponentManager manager("labeltest.gui", false, &style, &gui);

			CL_Slot slot_mouse_down = ((CL_Label *)manager.get_component("my_label"))->sig_mouse_down().connect(
				this, &LabelTest::on_mouse_down);

			while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
			{
				CL_Display::clear_display(1,1,1);
				gui.show();
				
				CL_System::keep_alive();
				CL_Display::flip_display();
			}
			
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
	
	void on_mouse_down(const CL_Key &key)
	{
		std::cout << "You clicked the label!" << std::endl;
	}
} app;
