/*
	Font Example
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

class FontApp : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "Font Example"; }

	virtual int main(int, char **)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore::init();
			CL_SetupDisplay::init();

			CL_Display::set_videomode(320, 200, 16, false, false);

			// Create the resource manager
			CL_ResourceManager* resources = new CL_ResourceManager("font.scr", false);

			// Load fonts
			CL_Font *font = CL_Font::load("Fonts/fnt_clansoft", resources);
			CL_Font *font_alpha = CL_Font::load("Fonts/fnt_avantgarde", resources);

			while(!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
			{
				CL_Display::fill_rect(0, 0, 160, 200, 0.0, 0.0, 0.0, 1.0);
				CL_Display::fill_rect(160, 0, 320, 200, 0.5, 0.5, 0.0, 1.0);

				// Show some text centered
				font->print_center(160, 80, "Hello World");
				font_alpha->print_center(160, 120, "Hello World");

				CL_Display::flip_display();

				CL_System::keep_alive();
				CL_System::sleep(10);
			}

			delete font_alpha;
			delete font;
			delete resources;
			
			CL_SetupDisplay::deinit();
			CL_SetupCore::deinit();
		}
		catch (CL_Error err)
		{
			std::cout << "Exception caught: " << err.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();

			return -1;
		}

		return 0;
	}
} app;
