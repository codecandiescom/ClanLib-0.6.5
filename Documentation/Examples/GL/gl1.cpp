#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

class GLApp : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "OpenGL Example"; }

	virtual int main(int, char **)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore::init();
			// Make sure the display will use the OpenGL initializer
			CL_SetupGL::init();
			CL_SetupDisplay::init();

			GLfloat	rtri;			// Angle For The Triangle
			GLfloat	rquad;			// Angle For The Quad	

			int width = 800;
			int height = 600;

			CL_Display::set_videomode(width, height, 32, false);
			
			float f = 0;
			while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
			{
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// This Will Clear The Background Color To Black
				glClearDepth(1.0);						// Enables Clearing Of The Depth Buffer
				glDepthFunc(GL_LESS);					// The Type Of Depth Test To Do
				glEnable(GL_DEPTH_TEST);				// Enables Depth Testing
				glShadeModel(GL_SMOOTH);				// Enables Smooth Color Shading

				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluPerspective(45.0f+f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
				f -= 0.1f;
				glMatrixMode(GL_MODELVIEW);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
				glLoadIdentity();						// Reset The View
				glTranslatef(-1.5f,0.0f,-6.0f);			// Move Into The Screen And Left

				glRotatef(rtri,0.0f,1.0f,0.0f);			// Rotate The Triangle On The Y Axis
			
				glBegin(GL_POLYGON);					// Start Drawing A Polygon
				glColor3f(1.0f,0.0f,0.0f);				// Red
				glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Front)
				glColor3f(0.0f,1.0f,0.0f);				// Green
				glVertex3f(-1.0f,-1.0f, 1.0f);			// Left Of Triangle (Front)
				glColor3f(0.0f,0.0f,1.0f);				// Blue
				glVertex3f( 1.0f,-1.0f, 1.0f);			// Right Of Triangle (Front)

				glColor3f(1.0f,0.0f,0.0f);				// Red
				glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Right)
				glColor3f(0.0f,0.0f,1.0f);				// Blue
				glVertex3f( 1.0f,-1.0f, 1.0f);			// Left Of Triangle (Right)
				glColor3f(0.0f,1.0f,0.0f);				// Green
				glVertex3f( 1.0f,-1.0f, -1.0f);			// Right Of Triangle (Right)

				glColor3f(1.0f,0.0f,0.0f);				// Red
				glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Back)
				glColor3f(0.0f,1.0f,0.0f);				// Green
				glVertex3f( 1.0f,-1.0f, -1.0f);			// Left Of Triangle (Back)
				glColor3f(0.0f,0.0f,1.0f);				// Blue
				glVertex3f(-1.0f,-1.0f, -1.0f);			// Right Of Triangle (Back)

				glColor3f(1.0f,0.0f,0.0f);				// Red
				glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Left)
				glColor3f(0.0f,0.0f,1.0f);				// Blue
				glVertex3f(-1.0f,-1.0f,-1.0f);			// Left Of Triangle (Left)
				glColor3f(0.0f,1.0f,0.0f);				// Green
				glVertex3f(-1.0f,-1.0f, 1.0f);			// Right Of Triangle (Left)
				glEnd();								// Done Drawing The Polygon

				glLoadIdentity();
				glTranslatef(1.5f,0.0f,-7.0f);			// Move Right 1.5 Units
				glRotatef(rquad,1.0f,1.0f,1.0f);		// Rotate The Quad On X,Y & Z

				glBegin(GL_QUADS);						// Start Drawing A Quad
				glColor3f(0.0f,1.0f,0.0f);				// Blue
				glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Top)
				glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Top)
				glVertex3f(-1.0f, 1.0f, 1.0f);			// Bottom Left Of The Quad (Top)
				glVertex3f( 1.0f, 1.0f, 1.0f);			// Bottom Right Of The Quad (Top)

				glColor3f(1.0f,0.5f,0.0f);				// Orange
				glVertex3f( 1.0f,-1.0f, 1.0f);			// Top Right Of The Quad (Bottom)
				glVertex3f(-1.0f,-1.0f, 1.0f);			// Top Left Of The Quad (Bottom)
				glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Bottom)
				glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Bottom)

				glColor3f(1.0f,0.0f,0.0f);				// Red
				glVertex3f( 1.0f, 1.0f, 1.0f);			// Top Right Of The Quad (Front)
				glVertex3f(-1.0f, 1.0f, 1.0f);			// Top Left Of The Quad (Front)
				glVertex3f(-1.0f,-1.0f, 1.0f);			// Bottom Left Of The Quad (Front)
				glVertex3f( 1.0f,-1.0f, 1.0f);			// Bottom Right Of The Quad (Front)

				glColor3f(1.0f,1.0f,0.0f);				// Yellow
				glVertex3f( 1.0f,-1.0f,-1.0f);			// Top Right Of The Quad (Back)
				glVertex3f(-1.0f,-1.0f,-1.0f);			// Top Left Of The Quad (Back)
				glVertex3f(-1.0f, 1.0f,-1.0f);			// Bottom Left Of The Quad (Back)
				glVertex3f( 1.0f, 1.0f,-1.0f);			// Bottom Right Of The Quad (Back)

				glColor3f(0.0f,0.0f,1.0f);				// Blue
				glVertex3f(-1.0f, 1.0f, 1.0f);			// Top Right Of The Quad (Left)
				glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Left)
				glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Left)
				glVertex3f(-1.0f,-1.0f, 1.0f);			// Bottom Right Of The Quad (Left)

				glColor3f(1.0f,0.0f,1.0f);				// Violet
				glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Right)
				glVertex3f( 1.0f, 1.0f, 1.0f);			// Top Left Of The Quad (Right)
				glVertex3f( 1.0f,-1.0f, 1.0f);			// Bottom Left Of The Quad (Right)
				glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Right)
				glEnd();								// Done Drawing The Quad
				
				rtri+=5.0f;								// Increase Rotation Variable For The Triangle
				rquad-=5.0f;							// Decrease Rotation Variable For The Quad

				CL_Display::flip_display();
				CL_System::keep_alive();
			}

			CL_SetupDisplay::deinit();
			CL_SetupGL::deinit();
			CL_SetupCore::deinit();
		}
		catch (CL_Error err)
		{
			std::cout << "Exception caught: " << err.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}
} app;
