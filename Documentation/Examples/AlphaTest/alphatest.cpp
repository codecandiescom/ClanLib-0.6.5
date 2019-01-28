/*
    alpha test example
*/

#include <ClanLib/core.h>
#include <ClanLib/jpeg.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

class SurfaceApp : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "Alpha test application"; }

	virtual int main(int, char **)
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
			CL_Display::set_videomode(640, 480, 16, false);
		
			// Create a gradient surface
			CL_Canvas *canvas = new CL_Canvas(100, 640);
			canvas->lock();
			unsigned int* data = (unsigned int*) canvas->get_data();
			for (int y=0; y<480; y++)
			{
				for (int x=0; x<100; x++)
				{
					data[x + 100 * y] = CL_Color::get_color(RGBA8888, 
					1.0f,
					0.0f,
					0.0f,
					float (y)/ 480.0f); 
				}
			}			
			canvas->unlock();
			CL_Surface *gradient = CL_Surface::create(canvas, true);
		
			// Load surface from jpg file:
			CL_Surface *surface = CL_JPEGProvider::create("back.jpg", NULL, RGB565);
		
			float alpha = 0.0f;

			// Loop until the user hits escape:
			while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
			{
				// Draw surface
				surface->put_screen(0, 0);
				
				// Change the alpha
				alpha += 0.01f;
				if (alpha > 1.0)
					alpha -= 1.0f;

				// Draw some green alpha lines
				for (int y = 10; y < 80; y += 10)
					CL_Display::draw_line(0, y, 640, y, 0.0, 1.0, 0.0, 1.0f - alpha);

				// Draw a blue alpha box				
				CL_Display::fill_rect(30, 30, 290, 170, 0.0, 0.0, 1.0, alpha);

				// Draw the red gradient
				gradient->put_screen(0,0);
			
				// Flip front and backbuffer. This makes the changes visible:
				CL_Display::flip_display();
			
				// Update keyboard input and handle system events:
				// Exits the loop if ClanLib requests shutdown - for instance if
				// someone closes the window.
				CL_System::keep_alive();
			}

			delete surface;
			delete gradient;

			// De-Initialize the ClanLib components
			CL_SetupDisplay::deinit();
			CL_SetupJPEG::deinit();
			CL_SetupCore::deinit();
		}
		catch(CL_Error error)
		{
			std::cout << "Exception caught : " << error.message.c_str() << std::endl;			

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}
} app;
