/*
	Loads a surface from a pcx file.
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

class SurfaceApp : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "Surface application"; }

	virtual int main(int, char **)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore::init();
			CL_SetupDisplay::init();

			// Set mode: 320x200 16 bpp
			CL_Display::set_videomode(320, 400, 16, false);

			CL_ResourceManager *resources = 0;

			CL_Surface *surface1 = load_image_from_file();
			CL_Surface *surface2 = load_image_from_resource();

			// Loop until the user hits escape:
			while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
			{
				// Draw surfaces:
				surface1->put_screen(0, 0);
				surface1->put_screen(0, 200);
				
				// Flip front and backbuffer. This makes the changes visible:
				CL_Display::flip_display();

				// Improve responsiveness:
				CL_System::sleep(10);
				
				// Update keyboard input and handle system events:
				CL_System::keep_alive();
			}

			delete surface1;
			delete surface2;
			delete resources;

			CL_SetupDisplay::deinit();
			CL_SetupCore::deinit();
		}
		catch (CL_Error err)
		{
			std::cout << "Error: " << err.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}

	// Load image from file
	CL_Surface *load_image_from_file()
	{
		// Load surface from pcx file:
		return CL_PCXProvider::create("survivor.pcx", NULL);
	}

	// Load image from resource file
	CL_Surface *load_image_from_resource()
	{
		// Open resources:
		CL_ResourceManager *resources = new CL_ResourceManager("resources.scr", false);

		// Load surface from resource:
		return new CL_Surface("survivor", resources);
	}
} app;
