#include <ClanLib/core.h>
#include <ClanLib/vorbis.h>
#include <ClanLib/sound.h>
#include <ClanLib/application.h>

class CL_DistortFilter : public CL_SoundFilter
{
public:
	CL_DistortFilter(int distortion) : m_distortion(distortion)
	{
		rand_map = new int[2048];
		for (int i=0;i<2048;i++)
		{
			rand_map[i] = (rand()%distortion) - (distortion/2);
		}
		rand_lim = 1024+rand()%1024;
		rand_pos = rand()%1024;
	}
	
	~CL_DistortFilter()
	{
		delete[] rand_map;
	}
	
	virtual void filter(int *data, int size)
	{
		for (int i=0;i<size*2;i+=rand()%4)
		{
			data[i] =  data[i]+rand_map[rand_pos++];
			if (rand_pos >= rand_lim)
			{
				rand_lim = 1024+rand()%1024;
				rand_pos = rand()%1024;
			}
		}
	}
private:
	int m_distortion;
	int *rand_map;
	int rand_lim;
	int rand_pos;
};

class MyApp : public CL_ClanApplication
{
public:
	virtual char *get_title()
	{
		return "clanVorbis test application.";
	}

	virtual int main(int argc, char** argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore::init();
			CL_SetupSound::init();

			// Load ogg file into a soundbuffer
			CL_SoundBuffer *sample = new CL_SoundBuffer(
				new CL_VorbisSoundProvider("cheer1.ogg"),
				true);

			int deg = 128;
			
			// Construct fadefilter with initial volume 0
//			CL_FadeFilter fade(0);

			// Prepare playback session
			CL_SoundBuffer_Session playback(sample->prepare());

			// Add some filters to the playback session
//			playback.add_filter(new CL_InverseEchoFilter(deg*1024), true);
//			playback.add_filter(new CL_DistortFilter(deg), true);
//			playback.add_filter(new CL_EchoFilter(8*1024), true);
//			playback.add_filter(&fade);

			std::cout << "Playing vorbis session..." << std::endl;

			// Play it!
			playback.play();
			
			// Fade in volume from 0..100 in 10 seconds
//			fade.fade_to_volume(100, 10*1000); // fade in, duration 10 seconds.

			// Loop until done playing the ogg file
			while (playback.is_playing())
			{
				// Make sure the system keeps responding
				CL_System::sleep(10);
				
				// Handle system events:
				// Exits the loop if ClanLib requests shutdown - for instance if
				// someone closes the window.
				CL_System::keep_alive();
			}

			// Cleanup
			delete sample;

			CL_SetupSound::deinit();
			CL_SetupCore::deinit();
		}
		catch (CL_Error err)
		{
			std::cout << "error: " << err.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();

			return -1;
		}

		return 0;
	}
} app;
