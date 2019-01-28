#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

#include "cl_writepng.h"

class ScreenShotApp : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "Press space to take screenshot"; }

	CL_Surface *take_screen_shot()
	{
		unsigned short width = CL_Display::get_width();
		unsigned short height = CL_Display::get_height();
	
		CL_Canvas *canvas = new CL_Canvas(width, height);
		
		float r=0, g=0, b=0, a=0;
	
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				CL_Display::get_target()->get_pixel( x, y, &r, &g, &b, &a );
				canvas->draw_pixel( x, y, r, g, b, 1.0f );
			}
		}

		return CL_Surface::create(canvas);
	}
	
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
			CL_Display::set_videomode(320, 200, 16, false);

			std::cout << "Press space to get a screenshot" << std::endl;

			// Loop until the user hits escape:
			while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
			{
				// Clear the display
				CL_Display::clear_display();
				
				// Let's draw something
				CL_Display::fill_rect(1,1,200,200,0.2f,0.2f,0.2f,1.0f);
				CL_Display::fill_rect(1,1,50,50,1.0f,0.0f,0.0f,1.0f);//red
				CL_Display::fill_rect(50,50,100,100,0.0f,1.0f,0.0f,1.0f);//green
				CL_Display::fill_rect(150,150,200,200,0.0f,0.0f,1.0f,1.0f);//blue
				
				// Make sure our updates are displayed
				CL_Display::flip_display();
				
				// Update keyboard input and handle system events:
				// Exits the loop if ClanLib requests shutdown - for instance if
				// someone closes the window.
				
				CL_System::keep_alive();

				// If someone pressed SPACE
				if(CL_Keyboard::get_keycode(CL_KEY_SPACE))
				{
					// Save a screenshot
					std::cout << "Saving png" << std::endl;
					CL_Surface *screenshot = take_screen_shot();
					WritePNG::write_png( screenshot, "screenshot.png" );
				}
			}

			CL_SetupDisplay::deinit();
			CL_SetupCore::deinit();
		}
		catch (CL_Error err)
		{
			std::cout << "Error: " << err.message.c_str() << std::endl;
		}

		// Display console close message and wait for a key
		console.display_close_message();

		return 0;
	}
} app;
