
#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/sound.h>

class SinusStream : public CL_StreamSoundProvider_Session
{
public:
	SinusStream()
	{
		pos = 0;
		playing = false;
		stream_length = 60*44100; // play sound for 60 seconds.
	}
	
	virtual bool eof() const
	{
		return (pos >= stream_length); // only stream for four seconds.
	}
	
	virtual void stop()
	{
		playing = false;
	}

	virtual bool play()
	{
		playing = true;
		return true;
	}

	virtual bool set_position(int pos)
	{
		this->pos = pos;
		return true;
	}
	
	virtual int get_data(void *data_ptr, int data_requested)
	{
		int samples_req = data_requested / 2;
		if (samples_req + pos > stream_length)
		// playing beyond the end of stream data
		{
			samples_req = stream_length - pos;
			if (samples_req < 0)
			{
				stop();
				return 0;
			}
		}
		
		short *samples = (short *) data_ptr;

		// Generate required audio samples
		for (int i=0; i<samples_req; i++)
		{
			samples[i] = (short) (sin(((pos+i)*440*3.14)/44100)*32000);
		}
	
		pos += samples_req;

		return data_requested; // return the amount samples given.
	}

	virtual int get_frequency() const
	{
		return 44100;
	}

	virtual SoundFormat get_format() const
	{
		return sf_16bit_signed;
	}

private:
	int pos;
	bool playing;
	
	int stream_length;
};

class SinusStreamProvider : public CL_StreamSoundProvider
{
public:
	virtual CL_StreamSoundProvider_Session *begin_session()
	{
		return new SinusStream();
	}

	virtual void end_session(CL_StreamSoundProvider_Session *session)
	{
		delete session;
	}
};

class App : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "Streamed sinus test"; }

	virtual int main(int argc, char **argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore::init();
			CL_SetupSound::init();

			// Create the soundbuffer with the SinusStreamProvider

			CL_SoundBuffer *buffer = CL_SoundBuffer::create(
				new SinusStreamProvider,
				true);

			// Prepare a soundbuffer session
			CL_SoundBuffer_Session session(buffer->prepare());

			// Start playing
			session.play();
			
			// Wait some time
			CL_System::keep_alive(5000);

			// Stop playback
			session.stop();
			
			// Cleanup
			delete buffer;

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
