/*
	Example of playing a soundbuffer from a wave file.
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/sound.h>
#include <iostream>

class SoundbufferApp : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "Soundbuffer application"; }

	virtual int main(int, char **)
	{
		CL_ConsoleWindow console("SoundbufferApp");
		console.redirect_stdio();

		try
		{
			CL_SetupCore::init();	
			CL_SetupSound::init();

			// Test recording:
			rec_sound();

			// Play some sounds
			play_sound1();
			play_sound3();
			
			// Play some sounds from resource script
			play_sound3_ex(false);
			
			// Play some sounds from datafile
			play_sound3_ex(true);

			// Play streamed sound:
			play_sound4();

			// Play from resource script, reference count test:
			play_sound5();

			CL_SetupSound::deinit();
			CL_SetupCore::deinit();
		}
		catch (CL_Error err)
		{
			std::cout << "error: " << err.message.c_str() << std::endl;
		}

		console.display_close_message();

		return 0;
	}

	void play_sound1()
	{
		std::cout << "play_sound1..." << std::endl;

		// Load a sample from a wave file:
		CL_SoundBuffer soundbuffer(new CL_Sample("beep.wav", NULL), true);

		// Setup a session handle for the playback:
		CL_SoundBuffer_Session session = soundbuffer.prepare();

		// Start the playback:
		session.play();

		// Wait until the sample finishes:
		while(session.is_playing())
		{
			CL_System::keep_alive();
			CL_System::sleep(100);
		}
	}

	void play_sound2()
	{
		std::cout << "play_sound2..." << std::endl;

		// Load sample, play it once and unload it when done:
		CL_SoundBuffer soundbuffer(new CL_Sample("beep.wav", NULL), true);
		soundbuffer.play();

		// Note: if you call this function instead of play_sound() in this
		//       example, you wont actually hear the sample because program
		//       exits right away.
	}

	void play_sound3()
	{
		std::cout << "play_sound3..." << std::endl;

		// This time load from a resource file.
		CL_ResourceManager resources("sound.scr", false);
		CL_SoundBuffer soundbuffer("beep", &resources);

		// Setup a session handle for the playback:
		CL_SoundBuffer_Session session = soundbuffer.prepare();

		// Start the playback:
		session.play();

		// Wait until the sample finishes:
		while(session.is_playing())
		{
			CL_System::keep_alive();
			CL_System::sleep(100);
		}
	}

	void play_sound3_ex(bool from_datafile)
	{
		std::cout << "play_sound3_ex..." << std::endl;

		// Compile datafile if to be used:
		std::string file = "sound.scr";
		if (from_datafile)
		{
			CL_DatafileCompiler::write(file, "sound.dat");
			file = "sound.dat";
		}

		// This time load from a resource file.
		CL_ResourceManager resources(file, from_datafile);

		// Do a multiple resource load test:
		for (int i=0; i<3; i++)
		{
			CL_SoundBuffer soundbuffer("beep", &resources);

			// Setup a session handle for the playback:
			CL_SoundBuffer_Session session = soundbuffer.prepare();

			// Start the playback:
			session.play();

			// Wait until the sample finishes:
			while(session.is_playing())
			{
				CL_System::keep_alive();
				CL_System::sleep(100);
			}
		}
	}

	void play_sound4()
	{
		std::cout << "play_sound4..." << std::endl;

		// Load sample as streaming:
		CL_SoundBuffer soundbuffer(new CL_Streamed_WaveSample("beep.wav"), true);

		// Setup a session handle for the playback:
		CL_SoundBuffer_Session session = soundbuffer.prepare();

		// Start the playback:
		session.play();

		// Wait until the sample finishes:
		while(session.is_playing())
		{
			CL_System::keep_alive();
			CL_System::sleep(100);
		}
	}

	void play_sound5()
	{
		std::cout << "play_sound5..." << std::endl;

		// Reference counting test:
		CL_ResourceManager resources("sound.scr", false);

		for (int i=0; i<5; i++)
		{
			CL_SoundBuffer soundbuffer("beep", &resources);

			// Setup a session handle for the playback:
			CL_SoundBuffer_Session session = soundbuffer.prepare();

			// Start the playback:
			session.play();

			// Wait until the sample finishes:
			while(session.is_playing())
			{
				CL_System::keep_alive();
				CL_System::sleep(100);
			}
		}
	}

	void rec_sound()
	{
		std::cout << "rec_sound..." << std::endl;
		CL_StreamSoundProvider *mic = CL_Sound::get_microphone();
		if (mic == 0) return; // no mic support.

		CL_StreamSoundProvider_Session *mic_session = mic->begin_session();

		std::cout << "recording..." << std::endl;
		mic_session->play();
		CL_System::sleep(10000);
		mic_session->stop();

		// Get for 10 secs of audio data:
		unsigned char buffer[22050*4*10];
		int received = mic_session->get_data(buffer, 22050*4*10);

		mic->end_session(mic_session);

		// Put it into a raw sample and play it:
		CL_SoundBuffer sample(
			new CL_Sample_RawData(buffer, received, 2, true),
			true);

		std::cout << "playing back recording..." << std::endl;
		CL_SoundBuffer_Session session = sample.play();

		// Wait until the sample finishes:
		while(session.is_playing())
		{
			CL_System::keep_alive(100);
		}
	}
} app;
