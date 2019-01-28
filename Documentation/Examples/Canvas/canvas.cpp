/*
        CL_Canvas test app.
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

class DrawingApp : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "Drawing application"; }

	virtual int main(int, char **)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore::init();
			CL_SetupDisplay::init();

			CL_Display::set_videomode(320, 200, 16, false);

			CL_Canvas *src_canvas = new CL_Canvas(
				320, 200, 1, 
				CL_Color::get_red_mask(RGB565),
				CL_Color::get_green_mask(RGB565),
				CL_Color::get_blue_mask(RGB565),
				CL_Color::get_alpha_mask(RGB565),
				true,
				0); // transcol == black
		
			src_canvas->fill_rect(0, 0, 319, 199, 1.0, 1.0, 1.0, 1.0);
			src_canvas->fill_rect(160, 0, 319, 199, 0.0, 0.0, 0.0, 1.0);

			CL_Canvas *dst_canvas = new CL_Canvas(
				320, 200, 1, 
				CL_Color::get_red_mask(RGB565),
				CL_Color::get_green_mask(RGB565),
				CL_Color::get_blue_mask(RGB565),
				CL_Color::get_alpha_mask(RGB565));
	
			dst_canvas->fill_rect(0, 0, 319, 199, 0.0, 0.0, 1.0, 1.0);

			CL_Surface *src_surface = CL_Surface::create(src_canvas);
			src_surface->put_target(0,0,0, dst_canvas);
	
			CL_Surface *dst_surface = CL_Surface::create(dst_canvas);
			
			// Loop until the user hits escape:
			while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
			{
				// put the destination surface on the screen please
				dst_surface->put_screen(0,0);
			
				// Flip front and backbuffer. This makes any changes visible:
				CL_Display::flip_display();
			
				// Update keyboard input and handle system events:
				// Exits the loop if ClanLib requests shutdown - for instance if
				// someone closes the window.
				CL_System::keep_alive();

				// Sleep alittle to free up some CPU cycles
				CL_System::sleep(10);
			}

			delete src_canvas;
			delete dst_canvas;
			delete src_surface;
			delete dst_surface;

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
