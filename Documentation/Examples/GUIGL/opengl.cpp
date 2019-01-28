#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

#include "stylemanager_opengl.h"

class App : public CL_ClanApplication
{
public:
	char *get_title() { return "OpenGL GUI"; }

	bool quit;
	
	int main(int argc, char** argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			std::cout << std::endl;
			std::cout << "This example shows an OpenGL GUI theme." << std::endl;
			std::cout << std::endl;

			CL_SetupCore::init();
			CL_SetupGL::init();
			CL_SetupGUI::init();
			CL_SetupDisplay::init();
			CL_Display::set_videomode(1024,768,16, false);

			CL_OpenGL::disable_2d_projection();

			// Setup a camera view:
			float fov = 60.0f;
			float nearZ = 1.0f;
			float farZ = 20*100.0f;
			float ratio = CL_Display::get_width() / (float) CL_Display::get_height();

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();			
			gluPerspective(
				fov,                   // field of view
				ratio,                 // aspect ratio
				nearZ,                 // near clipping plane
				farZ);                 // far clipping plane

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			float pos[3] = {0, 0, -800};
			float center[3] = {320, 200, 0};
			float up[3] = {0, -1, 0};

			gluLookAt(
				// position
				pos[0],
				pos[1],
				pos[2],
				// center
				center[0],
				center[1],
				center[2],
				// up
				up[0],
				up[1],
				up[2]);

			// Antialias lines
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			glEnable(GL_LINE_SMOOTH);

			CL_ResourceManager *resources = new CL_ResourceManager("resources.scr", false);
			CL_StyleManager *style = new CL_StyleManager_OpenGL(resources);
			CL_GUIManager *gui = new CL_GUIManager(style);

			CL_Window *window1 = new CL_Window(CL_Rect(100, 100, 400, 300), "Window 1", gui);
			
			CL_Button *button1 = new CL_Button(CL_Rect(10,10,80,30), "Dummy 1", window1->get_client_area());
			CL_Button *button2 = new CL_Button(CL_Rect(10,40,80,60), "Dummy 2", window1->get_client_area());
			CL_Button *button3 = new CL_Button(CL_Rect(10,70,80,90), "Dummy 3", window1->get_client_area());
			CL_Button *button4 = new CL_Button(CL_Rect(10,100,80,120), "Dummy 4", window1->get_client_area());
			CL_Button *button5 = new CL_Button(CL_Rect(10,130,80,150), "Dummy 5", window1->get_client_area());

			CL_Window *window2 = new CL_Window(CL_Rect(200, 200, 500, 400), "Window 2", gui);
					
			CL_Button *buttonQuit = new CL_Button(CL_Rect(10,10,60,30), "Quit", gui);

			// Quit when user presses quit button								
			CL_Slot quitSlot = buttonQuit->sig_clicked().connect(this, &App::on_quit);

			quit = false;

			while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE) && quit == false)
			{
				CL_Display::clear_display();
				gui->show();

				CL_Display::draw_rect(0,0,639,479,1,1,0,1);

				CL_System::keep_alive();
				CL_Display::flip_display();
			}

			delete gui;
			delete style;
			delete resources;

			CL_SetupDisplay::deinit();
			CL_SetupGUI::deinit();
			CL_SetupGL::deinit();
			CL_SetupCore::deinit();
		}
		catch (CL_Error e)
		{
			std::cout << e.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}

	void on_quit()
	{
		quit = true;
	}
} app;