#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

class InputboxTest : public CL_ClanApplication
{
public:
	char *get_title() { return "InputboxTest"; }

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
			CL_Display::set_videomode(640,480,16, false, false, false);

			CL_ResourceManager resources("../GUIThemeDefault/gui.scr", false);
			CL_StyleManager_Default style(&resources);
			CL_GUIManager gui(&style);

			CL_ComponentManager manager("inputboxtest.gui", false, &style, &gui);

			CL_Slot s = ((CL_InputBox *) manager.get_component("my_inputbox"))->sig_changed().connect(
				this, &InputboxTest::on_input_changed);

			CL_Slot s2 = ((CL_ScrollBar *) manager.get_component("my_ver_scrollbar"))->sig_value_changed().connect(
				this, &InputboxTest::on_ver_scrolled);

			int fps = 0;
			unsigned int stime = CL_System::get_time();

			while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
			{
				CL_Display::clear_display(0.60f, 0.60f, 0.60f);
				gui.show();

				CL_System::keep_alive();
				CL_Display::flip_display();

				fps++;
				if (CL_System::get_time() >= stime+1000)
				{
					char buf[50];

					stime = CL_System::get_time();
					sprintf(buf, "fps: %d", fps);
					((CL_Label *) manager.get_component("fps"))->set_text(buf);

					fps = 0;
				}
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

	void on_input_changed(const std::string &s)
	{
		std::cout << s.c_str() << std::endl;
	}

	void on_ver_scrolled(int pos)
	{
		std::cout << pos << std::endl;
	}
} app;
