#include <stdio.h>
#include <ClanLib/gl.h>
#include <ClanLib/png.h>

#include "app.h"

Application app;

char *Application::get_title()
{
	static char str[80];
	sprintf(str, "%s v%d.%d", APP_NAME, APP_VER_MAJOR, APP_VER_MINOR);
	return str;
}

int Application::main(int argc, char** argv)
{
	// Create a console window for text-output if not available
	CL_ConsoleWindow console("Console");
	console.redirect_stdio();

	try
	{
		CL_SetupCore::init();
		CL_SetupGL::init();
		CL_SetupPNG::init();
		CL_SetupDisplay::init();

		CL_Display::set_videomode(640, 480, 32, true);

		// Show black screen while initializing
		CL_Display::fill_rect(0, 0, 640, 480, 0.0f, 0.0f, 0.0f, 1.0f);
		CL_Display::flip_display();

		logo = CL_PNGProvider::create("Logo.png", NULL);

		// Antialias lines
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_LINE_SMOOTH);

		run();

		CL_SetupDisplay::deinit();
		CL_SetupPNG::deinit();
		CL_SetupGL::deinit();
		CL_SetupCore::deinit();
	}
	catch (CL_Error e)
	{
		std::cout << e.message.c_str() << std::endl;
	}

	// Display console close message and wait for a key
	console.display_close_message();

	return 0;
}

void Application::run()
{
	bool quit = false;

	try {
		do {
			if(CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
				quit = true;

			if(CL_Keyboard::get_keycode(CL_KEY_SPACE) == false)
			{
				update();
				draw();
			}
						
			CL_Display::flip_display();
			CL_System::keep_alive();
		} while(quit == false);
	}
	catch (CL_Error err)
	{
		std::cout << "Error: " << err.message.c_str() << std::endl;
	}			
}

void Application::draw()
{
	static int x = 0;
	static int y = 0;
	static int count = 0;

	// Fade background
	CL_Display::fill_rect(0,0,640,480, 0.0f, 0.0f, 0.0f, 0.07f);

	// Draw background "smoke"
	for(int p = 0; p < 20; p++) {
		int x = rand() % 640;
		int y = rand() % 480;
		CL_Display::fill_rect(x - 150, y - 150, x + 150, y + 150, 1.0f, 1.0f, 1.0f, 0.005f);
	}

	// Draw lines
	for(int l = 0; l < LINE_COUNT; l++)
		lines[l].draw();

	// Move logo around ?
	if(rand() % 100 == 0 || count)
	{
		if(count == 0)
			count = 30;
		else
			count--;
		x = rand() % 400 - 200;
		y = rand() % 400 - 200;
	}

	// Draw logo
	logo->put_screen((320 - 105) + x, (240 - 22) + y);
}

void Application::update()
{
	for(int i=0; i<LINE_COUNT; i++)
		lines[i].update();
}
