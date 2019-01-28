/*
    $Id: plasma.cpp,v 1.6 2001/10/16 15:44:17 japj Exp $

    demonstrates the use of CL_Canvas and assembler - vogel
*/


#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <math.h>

#ifndef PI
#define PI 3.141592654
#endif

#define width 640
#define height 480

extern "C"
{
	void calculate_plasma(
		char* dest, 
		char* s1, 
		char* s2, 
		char* s3,
		int w,
		int h
	);
}

class TPlasma : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "Plasma Demo"; }

	virtual int main(int, char**)
	{
		std::cout << "Plasma Demo using CL_Canvas and Assembler" << std::endl;

		CL_SetupCore::init();
		CL_SetupDisplay::init();

		// Setting resolution to width x height x 16 (fullscreen for better performance)
		CL_Display::set_videomode(width, height, 16);

		char* data = new char[width*height*4*4];
		int* dummy = (int*) data;

		// Initialize the big 'surface'
		for (int y=0;y<height*2;y++)
			for (int x=0;x<width*2;x++)
			{
				float x1 = 2*PI*x/(2*width);
				float y1 = 2*PI*y/(2*height);
				*(dummy++) = 
					((int) (85 * fabs(sin(2*x1 * cos(2*y1))))) << 24 |
					((int) (85 * fabs(cos(x1 * sin(y1)))))     << 16 |
					((int) (85 * fabs(cos(3*x1) * sin(y1))))   << 8;
			}
		
		CL_Canvas canvas(width, height); //defaults to RGBA
		CL_Surface *plasma = CL_Surface::create_dynamic(&canvas);
		
		float c1 = 0.0f;
		float c2 = float(PI / 3.0f);
		float c3 = float(PI * 3.0f / 4.0f);
		
		int last_time = CL_System::get_time();
		
		try
		{
			while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
			{
				// Lock the canvas
				canvas.lock();

				// Calculate the plasma
				calculate_plasma(
					(char*) canvas.get_data(), 
					
					data + 4*((int) (width/2+width/2*cos(c1))) + 
					width*2*4*((int) (height/2+height/2*sin(c1))),
					
					data + 4*((int) (width/2+width/2*cos(c2))) + 
					width*2*4*((int) (height/2+height/2*sin(c2))),
					
					data + 4*((int) (width/2+width/2*cos(c3))) + 
					width*2*4*((int) (height/2+height/2*sin(c3))),
					
					width,
					height
					);

				canvas.unlock();

				// Reload the dynamic canvas (because we changed the canvas data)
				plasma->reload();
				
				// And put it to the screen
				plasma->put_screen(0,0);

				// Calculate elapsed time since last update			
				float time_elapsed = (CL_System::get_time() - last_time) / 1000.0f;
				last_time = CL_System::get_time();
		
				// Make update framerate independend
				float d = (2 * PI / 10) * time_elapsed;
				c1 += d; c2 -= d; c3 -= d;
			
				CL_Display::flip_display();
				CL_System::keep_alive();
			}
		}
		catch (CL_Error err)
		{
			std::cout << "Error: " << err.message.c_str() << std::endl;
		}
		
		delete plasma;
		delete data;

		CL_SetupDisplay::deinit();
		CL_SetupCore::deinit();

		return 0;
	}
} app;
