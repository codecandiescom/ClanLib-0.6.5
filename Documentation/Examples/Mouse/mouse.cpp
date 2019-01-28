/*
	Demonstration of the CL_Mouse interface:

	This example uses the old way of checking for mouse button presses - through polling.
    Mousewheel and extra buttons are not supported with this method.
	Check out the other mouse example using signals for such features.
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

class MouseApp : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "Mouse application"; }

	virtual int main(int, char **)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore::init();
			CL_SetupDisplay::init();

			// Set mode: 320x240 16 bpp
			CL_Display::set_videomode(320, 240, 16, false);
			
			// Make sure the display is black at startup:
			CL_Display::clear_display();

			// Open the resource file containing our custom mouse cursor resource
			CL_ResourceManager *manager = new CL_ResourceManager("mouse.scr", false);

			// Set the mouse cursor to use a custom mouse provided in our resource file
			CL_MouseCursor::set_cursor(CL_MouseCursorProvider::load("sur_cursor", manager));

			// Loop until the user presses Escape
			while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
			{
				// Clear display
				CL_Display::clear_display(0.0f, 0.0f, 0.5f, 1.0f);
			
				// Flip front and backbuffer - this makes the changes visible
				CL_Display::flip_display();
			
				CL_System::sleep(10);

				// Update keyboard input and handle system events
				CL_System::keep_alive();

				if(CL_Mouse::left_pressed())
					std::cout << "Left pressed at " << CL_Mouse::get_x() << "," << CL_Mouse::get_y() << std::endl;
				if(CL_Mouse::middle_pressed())
					std::cout << "Middle pressed" << std::endl;
				if(CL_Mouse::right_pressed())
					std::cout << "Right pressed" << std::endl;
			}

			// Delete resource manager
			delete manager;
		}
		catch (CL_Error err)
		{
			std::cout << "Exception caught: " << err.message.c_str() << std::endl;
		}

		// Display console close message and wait for a key
		console.display_close_message();

		CL_SetupDisplay::deinit();
		CL_SetupCore::deinit();

		return 0;
	}
} app;
