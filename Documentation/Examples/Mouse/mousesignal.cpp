/*
	Demonstration of the CL_Mouse interface
*/

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>
#include <strstream>

class MouseSignalApp : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "Mouse application"; }

	void on_mouse_press(const CL_Key &key)
	{
		// Use the same code as on_button_press, but only MOUSE events.
	}

	void on_button_press(CL_InputDevice *device, const CL_Key &key)
	{
		std::strstream button;

		switch(key.id) {
		case CL_MOUSE_LEFTBUTTON:
			button << "Mouse left button";
			break;
		case CL_MOUSE_MIDDLEBUTTON:
			button << "Mouse middle button";
			break;
		case CL_MOUSE_RIGHTBUTTON:
			button << "Mouse right button";
			break;
		case CL_MOUSE_THUMBBUTTON:
			button << "Mouse thumb button";
			break;
		case CL_MOUSE_WHEELBUTTON:
			button << "Mouse wheel button";
			break;
		case CL_MOUSE_WHEELUP:
			button << "Mouse wheel up";
			break;
		case CL_MOUSE_WHEELDOWN:
			button << "Mouse wheel down";
			break;
		case CL_KEY_LCTRL:
			button << "Left control";
			break;
		default:
			button << key.id << " (" << (char)key.ascii << ")";
		}

		button << " pressed at " << key.x << "," << key.y << '\0';
		std::cout << button.str() << std::endl;
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

			// Set mode: 320x240 16 bpp
			CL_Display::set_videomode(320, 240, 16, false);
			
			// Make sure the display is black at startup:
			CL_Display::clear_display();

			// Open the resource file containing our custom mouse cursor resource
			CL_ResourceManager *manager = new CL_ResourceManager("mouse.scr", false);
			
			// Set the mouse cursor to use a custom mouse provided in our resource file
			CL_MouseCursor::set_cursor(CL_MouseCursorProvider::load("sur_cursor", manager));

			// This connects ANY input (joystick, mouse, keyboard)
			CL_Slot anyinput = CL_Input::sig_button_press().connect(
				this, &MouseSignalApp::on_button_press);

			// This connects only mouse input (Not used in this example)
			// CL_Slot mouse = CL_Mouse::sig_button_press().connect(
			//     this, &MouseSignalApp::on_mouse_press);

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
			}

			// Delete resource manager
			delete manager;
		}
		catch (CL_Error err)
		{
			std::cout << "Exception caught: " << err.message.c_str() << std::endl;
		}

		CL_SetupDisplay::deinit();
		CL_SetupCore::deinit();

		// Display console close message and wait for a key
		console.display_close_message();

		return 0;
	}
} app;
