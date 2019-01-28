/*
	The example shows how the physical buttons can be abstracted into logical
	buttons used by the game.
	
	For instance, the fire_button is the game's logical fire button. But
	physically it is serveral buttons on the keyboards and the joystick. 
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

class InputDevApp : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "Input device application"; }

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
			
			// Abstract game input from physical input:
			CL_InputButton_Group *fire_button = new CL_InputButton_Group;
			CL_InputAxis_Group *hori_axis = new CL_InputAxis_Group;
			CL_InputAxis_Group *vert_axis = new CL_InputAxis_Group;

			// Add keyboard input:
			fire_button->add(
				CL_Input::keyboards[0]->get_button(CL_KEY_ESCAPE));

			fire_button->add(
				CL_Input::keyboards[0]->get_button(CL_KEY_SPACE));

			hori_axis->add(
				new CL_InputButtonToAxis_Analog(
					CL_Input::keyboards[0]->get_button(CL_KEY_LEFT),
					CL_Input::keyboards[0]->get_button(CL_KEY_RIGHT)
					));

			vert_axis->add(
				new CL_InputButtonToAxis_Analog(
					CL_Input::keyboards[0]->get_button(CL_KEY_UP),
					CL_Input::keyboards[0]->get_button(CL_KEY_DOWN)
					));

			// Add joystick input (if available):
			if (CL_Input::joysticks.size())
			{
				fire_button->add(CL_Input::joysticks[0]->get_button(0));
				fire_button->add(CL_Input::joysticks[0]->get_button(1));
				
				hori_axis->add(CL_Input::joysticks[0]->get_axis(0));
				vert_axis->add(CL_Input::joysticks[0]->get_axis(1));
			}

			// Loop until one of the fire buttons are down:
			while (fire_button->is_pressed() == false)
			{
				// Clear display with alpha background:
				CL_Display::clear_display(0.0f, 0.0f, 0.5f, 0.2f);

				// Calculate box position:			
				int x = CL_Display::get_width()/2 +
					(int) (hori_axis->get_pos()*CL_Display::get_width()/2);

				int y = CL_Display::get_height()/2 +
					(int) (vert_axis->get_pos()*CL_Display::get_height()/2);

				// Draw a box where the axes are:
				CL_Display::fill_rect(
					x-5, y-5, x+5, y+5,
					1.0f, 0.0f, 0.0f, 1.0f);

				// Flip front and backbuffer. This makes the changes visible:
				CL_Display::flip_display();
			
				// Update keyboard input and handle system events:
				CL_System::keep_alive();
			}

			delete fire_button;
			delete hori_axis;
			delete vert_axis;

			CL_SetupDisplay::deinit();
			CL_SetupCore::deinit();
		}
		catch (CL_Error err)
		{
			std::cout << "Error: " << err.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();

			return 1;
		}

		return 0;
	}
} app;
