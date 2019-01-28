#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

class Example2D : public CL_ClanApplication
{
public:
	// This function returns the name of your game
	virtual char *get_title()
	{
		return "ClanLib 2D API example";
	}

	virtual int main(int argc, char **argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			// Initialize ClanLib base components
			CL_SetupCore::init();

			// Initialize the ClanLib display component
			CL_SetupDisplay::init();

			// Set a videomode - 640x480x16bpp
			// false means that this program should not run full-screen
			CL_Display::set_videomode(640, 480, 16, true);

			// Load a surface from a targa-file using the targa-file
			// image loader. The targa-file image loader creates a 
			// surface from "logo.tga".
			CL_Surface *sur_logo = CL_TargaProvider::create("logo.tga", NULL);

			double sin_count = 0.0f;
			float ypos = 0.0f;
			float ydir = 3.0f;

			// Run until someone presses escape
			while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
			{
				// Clear the display in a dark blue nuance
				// The four arguments are red, green, blue and alpha
				// All color nuances in ClanLib are measured in the interval 0->1
				CL_Display::clear_display(0, 0, 0.2f, 1.0f);

				ypos += ydir;
				if (ypos+200 >= CL_Display::get_height() || ypos < 0)
				{
					ydir *= -1;
				}
				
				CL_Display::draw_line(0, (int)(ypos-1), CL_Display::get_width(), (int)(ypos-1), 1, 0, 0);
				CL_Display::draw_line(0, (int)(ypos+198), CL_Display::get_width(), (int)(ypos+198), 1, 0, 0);

				// Show the logo surface.
				// Use the get_width() and get_height() functions of both
				// CL_Display and CL_Surface, to show the surface in the bottom right corner
				sur_logo->put_screen(
					CL_Display::get_width()-sur_logo->get_width(),
					CL_Display::get_height()-sur_logo->get_height());

				CL_Display::push_clip_rect(CL_ClipRect(0, (int)(ypos), CL_Display::get_width(), (int)(ypos+198)));

				// Draw a rectangle in the center of the screen
				// going from (240, 140) -> (440, 340) _not_ including the 
				// pixels in the right-most column and bottom-most row (440, 340)
				CL_Display::fill_rect(240, 140, 440, 340, 1.0f, 1.0f, 1.0f, 1.0f);

				// Frame the rectangle with red lines
				CL_Display::draw_line(240, 140, 440, 140, 1.0f, 0, 0);
				CL_Display::draw_line(240, 340, 440, 340, 1.0f, 0, 0);
				CL_Display::draw_line(240, 140, 240, 340, 1.0f, 0, 0);
				CL_Display::draw_line(440, 140, 440, 340, 1.0f, 0, 0);

				// Show a few alpha-blending moving rectangles that moves in circles
				float x = cos(sin_count)*120;
				float y = sin(sin_count)*120;
				sin_count += 0.05;
				CL_Display::fill_rect((int)(320+x-30), (int)(240+y-30), (int)(320+x+30), (int)(240+y+30), 0, 1, 0, 0.5);
				x = cos(sin_count+3.14159)*120;
				y = sin(sin_count+3.14159)*120;
				CL_Display::fill_rect((int)(320+x-30),(int)(240+y-30),(int)(320+x+30),(int)(240+y+30), 1, 1, 0, 0.5);

				CL_Display::pop_clip_rect();

				// Flip the display, showing on the screen what we have drawed
				// since last call to flip_display()
				CL_Display::flip_display();

				// This call updates input and performs other "housekeeping"
				// call this each frame
				CL_System::keep_alive();
			}

			//Clean-up and remove the logo from memory
			delete sur_logo;
			
			// De-Initialize the ClanLib display component
			CL_SetupDisplay::deinit();

			// De-Initialize ClanLib base components
			CL_SetupCore::deinit();
		}
		catch(CL_Error error)
		{
			std::cout << "Exception caught : " << error.message.c_str() << std::endl;			

			// Display console close message and wait for a key
			console.display_close_message();

			return -1;
		}

		return 0;
	}
} my_app;

