/*
	Loading and displaying some JPEGs using the JPEG provider.
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/jpeg.h>
#include <ClanLib/display.h>

class JPEGApp : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "clanJPEG application"; }

	virtual int main(int, char ** argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore::init();
			CL_SetupJPEG::init();
			CL_SetupDisplay::init();

			// Set mode: 640x480 16 bpp
			CL_Display::set_videomode(640, 480, 16, false, false);
		
			// Create a new surface and load a image to it
			CL_Surface *surf = CL_JPEGProvider::create("dragon-color.jpg", NULL);
			CL_Surface *surf2 = CL_JPEGProvider::create("dragon-grey.jpg", NULL);

			// Loop until the user hits escape:
			while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
			{
				// Clear display
				CL_Display::clear_display();

				// Put image to screen
				surf->put_screen(0, 0);
				surf2->put_screen(surf->get_width(), surf->get_height());
			
				// Flip front and backbuffer. This makes the changes visible:
				CL_Display::flip_display();
			
				// Update keyboard input and handle system events:
				CL_System::keep_alive();

				// Don't take 100% CPU, relax abit
				CL_System::sleep(10);
			}

			delete surf;
			delete surf2;
		
			CL_SetupDisplay::deinit();
			CL_SetupJPEG::deinit();
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
