/*
	Small application demonstrating how to write a surface provider.
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

class OurSurfaceProvider : public CL_SurfaceProvider
{
public:
	OurSurfaceProvider();
	virtual ~OurSurfaceProvider();

	virtual unsigned int get_pitch() const;
	virtual unsigned int get_width() const;
	virtual unsigned int get_height() const;
	virtual unsigned int get_num_frames() const;
	virtual unsigned int get_depth() const;
	virtual unsigned int get_red_mask() const;
	virtual unsigned int get_green_mask() const;
	virtual unsigned int get_blue_mask() const;
	virtual unsigned int get_alpha_mask() const;
	virtual bool is_indexed() const;
	virtual CL_Palette *get_palette() const;
	virtual bool uses_src_colorkey() const;
	virtual unsigned int get_src_colorkey() const;
	virtual void *get_data() const;
	virtual void lock();
	virtual void unlock();

private:
	int lock_refs;
	unsigned char *buffer;
};

class SurfaceProviderApp : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "SurfaceProvider application"; }

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
			
			CL_Surface *surface = CL_Surface::create(
				new OurSurfaceProvider,
				true); // true -> delete surfaceprovider when surface is deleted
			
			// Loop until the user hits escape:
			while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
			{
				// Draw the surface:
				surface->put_screen(0, 0);
				
				// Improve response
				CL_System::sleep(10);
				
				// Flip front and backbuffer. This makes the changes visible:
				CL_Display::flip_display();
				
				// Update keyboard input and handle system events:
				CL_System::keep_alive();
			}

			delete surface;

			CL_SetupDisplay::deinit();
			CL_SetupCore::deinit();
		}
		catch (CL_Error err)
		{
			std::cout << "Error: " << err.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();

			return -1;
		}

		return 0;
	}
} app;


OurSurfaceProvider::OurSurfaceProvider()
{
	buffer = NULL;
	lock_refs = 0;
}

OurSurfaceProvider::~OurSurfaceProvider()
{
	delete[] buffer;
}

/* The next functions return the properties of our surface
*/

unsigned int OurSurfaceProvider::get_pitch() const
{
	return 4*get_width();
}

unsigned int OurSurfaceProvider::get_width() const
{
	return 320;
}

unsigned int OurSurfaceProvider::get_height() const
{
	return 240;
}

unsigned int OurSurfaceProvider::get_num_frames() const
{
	return 1;
}

unsigned int OurSurfaceProvider::get_depth() const
{
	return 32;
}

unsigned int OurSurfaceProvider::get_red_mask() const
{
	if (CL_Endian::is_system_big()) return 0x000000ff;
	return 0xff000000;
}

unsigned int OurSurfaceProvider::get_green_mask() const
{
	if (CL_Endian::is_system_big()) return 0x0000ff00;
	return 0x00ff0000;
}

unsigned int OurSurfaceProvider::get_blue_mask() const
{
	if (CL_Endian::is_system_big()) return 0x00ff0000;
	return 0x0000ff00;
}

unsigned int OurSurfaceProvider::get_alpha_mask() const
{
	if (CL_Endian::is_system_big()) return 0xff000000;
	return 0x000000ff;
}

bool OurSurfaceProvider::is_indexed() const
{
	return false;
}

CL_Palette *OurSurfaceProvider::get_palette() const
{
	return NULL; // no palette used by our surface provider.
}

bool OurSurfaceProvider::uses_src_colorkey() const
{
	return false;
}

unsigned int OurSurfaceProvider::get_src_colorkey() const
{
	return 0; // no source colorkey (single color transparency) used.
}

void *OurSurfaceProvider::get_data() const
{
	return buffer;
}

void OurSurfaceProvider::lock()
{
	// If the buffer has not been locked yet?
	if (lock_refs == 0)
	{
		// Create a new buffer
		buffer = new unsigned char[get_pitch()*get_height()];
		
		// And start filling it with data
		for (int y=0; y<240; y++)
		{
			for (int x=0; x<320; x++)
			{
				buffer[(x+y*320)*4 + 0] = 255; // alpha component of pixel
				buffer[(x+y*320)*4 + 1] =   x; // blue component
				buffer[(x+y*320)*4 + 2] =   y; // green component
				buffer[(x+y*320)*4 + 3] = x+y; // red component
			}
		}
	}
	
	// Increase the lock reference counter
	lock_refs++;
}

void OurSurfaceProvider::unlock()
{
	// Decrease the lock reference counter
	lock_refs--;

	// And if nobody uses the surface
	if (lock_refs == 0)
	{
		// Cleanup allocated memory
		delete[] buffer;
		buffer = NULL;
	}
}
