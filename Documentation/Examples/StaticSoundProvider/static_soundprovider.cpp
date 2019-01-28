/*
	Example of writing your own static soundprovider.
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/sound.h>
#include <math.h>

class OurStaticSoundProvider : public CL_StaticSoundProvider
{
public:
	OurStaticSoundProvider();
	virtual ~OurStaticSoundProvider();

	virtual void lock();
	virtual void unlock();
	virtual SoundFormat get_format() const;
	virtual int data_size() const;
	virtual void *get_data() const;
	virtual int get_frequency() const;

private:
	int lock_refs;
	short *buffer;
};

class StaticSoundProviderApp : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "StaticSoundProvider application"; }

	virtual int main(int, char **)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore::init();
			CL_SetupSound::init();

			CL_SoundBuffer *soundbuffer = CL_SoundBuffer::create(
				new OurStaticSoundProvider,
				true); // true -> delete provider when soundbuffer is deleted

			soundbuffer->play(true);

			int i=0;
			while (i != 40)
			{
				// Update keyboard input and handle system events:
				// Exits the loop if ClanLib requests shutdown - for instance if
				// someone closes the window.
				CL_System::keep_alive();

				CL_System::sleep(50);

				i++;
			}
						
			delete soundbuffer;

			CL_SetupSound::deinit();
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

#define buffer_size 22050*10

OurStaticSoundProvider::OurStaticSoundProvider()
{
	lock_refs = 0;
	buffer = NULL;
}

OurStaticSoundProvider::~OurStaticSoundProvider()
{
	delete[] buffer;
}

void OurStaticSoundProvider::lock()
{
	// Check reference counting
	if (lock_refs == 0)
	{
		// First time creation of buffer
		buffer = new short[buffer_size];
		
		// Calculate static sound
		for (int i=0; i<buffer_size; i++)
		{
			buffer[i] = (short) (sin(i*6.28/100)*32000);
		}
	}
	
	lock_refs++;
}

void OurStaticSoundProvider::unlock()
{
	lock_refs--;
	
	// Check reference counting
	if (lock_refs == 0)
	{
		// Cleanup when no references
		delete[] buffer;
		buffer = NULL;
	}
}

SoundFormat OurStaticSoundProvider::get_format() const
{
	return sf_8bit_signed_stereo;
}

int OurStaticSoundProvider::data_size() const
{
	return sizeof(short)*buffer_size;
}

void *OurStaticSoundProvider::get_data() const
{
	return buffer;
}

int OurStaticSoundProvider::get_frequency() const
{
	return 22050;
}
