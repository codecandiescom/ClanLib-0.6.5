#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/ttf.h>

class TTF_App : public CL_ClanApplication
{
public:
	char *get_title() { return "TTF Application"; }

	int main(int, char **)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore::init();
			CL_SetupDisplay::init();
			CL_SetupTTF::init();

			// Setup the display
			CL_Display::set_videomode(641, 400, 16, false, false);

			CL_Display::clear_display(1.0,1.0,1.0,1);
			CL_Display::flip_display();
			CL_Display::clear_display(1.0,1.0,1.0,1);

			char theta[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
			char beta[] = {"abcdefghijklmnopqsrtuvwyxz"};

			// Open a resource manager for the font resource
			CL_ResourceManager* resources = new CL_ResourceManager("ttf.scr", false);
			
			// Load in the font
			CL_Font *font = CL_Font::load("Fonts/fnt_clansoft", resources);

			// Loop until the user hits escape:
			while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
			{
				// Change some font settings
				font->change_size(20);
				font->change_colour(80,210,110,255);

				// Clear the display to white
				CL_Display::clear_display(1,1,1,1);

				// Draw some text with the current font settings
				font->print_right(160,170, "Hello Sailor");
				font->print_center(40, 80, "Hello World");
				font->print_left(30,50,"Hello All");
				font->print_center(150,160,beta);

				// Change some font settings
				font->change_size(40);
				font->change_colour(110,110,110,255);

				// Draw some text with the new font settings
				font->print_center(200,200,beta);

				// Display the current screen
				CL_Display::flip_display();
				
				// Make sure the system keeps responding
				CL_System::sleep(50);

				// Update keyboard input and handle system events:
				// Exits the loop if ClanLib requests shutdown - for instance if
				// someone closes the window.
				CL_System::keep_alive();
			}

			// Make sure we cleanup
			delete font;
			delete resources;

			CL_SetupTTF::deinit();
			CL_SetupDisplay::deinit();
			CL_SetupCore::deinit();
		}
		catch(CL_Error err)
		{
			std::cout << "Exception caught: " << err.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();

			return -1;
		}

		return 0;
	}
} app;
