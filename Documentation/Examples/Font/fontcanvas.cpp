/*
	Font Example
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

class MinimumApp : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "Font Example"; }

	virtual int main(int, char **)
	{
		try
		{
			CL_SetupCore::init();
			CL_SetupDisplay::init();

			CL_Display::set_videomode(320, 200, 16, false, false);

			CL_ResourceManager* resources =	new CL_ResourceManager("font.scr", false);
			CL_Font* font = CL_Font::load("Fonts/fnt_clansoft", resources);

			CL_Surface *surf = CL_Surface::create(
				new CL_Canvas(320, 200, 1,
					0xff000000, 
					0x00ff0000,
					0x0000ff00,
					0x00000000), true);
					
			font->put_target(160, 80, "Hello World", surf->get_provider());
			surf->reload();
			surf->put_screen(0,0);

			font->print_left(10, 10, "Goodbye World");

			CL_Display::flip_display();
			CL_System::sleep(2500);

			CL_SetupCore::deinit();
		}
		catch (CL_Error err)
		{
			std::cout << "Exception caught: " << err.message.c_str() << std::endl;
			return -1;
		}
		return 0;
	}
} app;
