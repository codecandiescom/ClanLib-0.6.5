/*
	Init display example.
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

class InitDisplayApp : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "InitDisplay application"; }

	virtual int main(int, char **)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore::init();
			CL_SetupDisplay::init();

			// Set mode: 640x480 16 bpp
			CL_Display::set_videomode(640, 480, 16, false);

			// Loop until the user hits escape:
			while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
			{
				// Clear display with random colors:
				CL_Display::clear_display(
					(rand() % 100) / 100.0f,  // red component
					(rand() % 100) / 100.0f,  // green component
					(rand() % 100) / 100.0f); // blue component

				CL_Display::draw_line(
					rand() % 2000 - 1000,
					rand() % 2000 - 1000,
					rand() % 2000 - 1000,
					rand() % 2000 - 1000, 0, 0, 0, 0);

				// Flip front and backbuffer. This makes the changes visible:
				CL_Display::flip_display();
			
				// Update keyboard input and handle system events:
				CL_System::keep_alive();
			}

			CL_SetupDisplay::deinit();
			CL_SetupCore::deinit();
		}
		catch (CL_Error err)
		{
			std::cout << "Error occoured: " << err.message.c_str() << std::endl;
		}

		// Display console close message and wait for a key
		console.display_close_message();

		return 0;
	}
} app;
