/*
	$Id: stream_provider_raw.h,v 1.6 2001/12/11 20:44:21 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

	File purpose:
		Streamed sample header file
*/

//! clanSound="Sound Providers"
//! header=sound.h

#ifndef header_streamed_raw_sample
#define header_streamed_raw_sample

#include <stdlib.h>
#include "../stream_soundprovider.h"
#include "../soundbuffer.h"

//: Streamed sample in raw PCM format (no header).
//- <p>This class providers playback support for raw PCM samples. That is just
//- the PCM sample data, not header.</p>
//- 
//- <p>Unlike CL_Sample_Raw, this class doesn't load the entire PCM stream into
//- memory. Instead it slowly loads it from disk as the data is needed. With
//- very large samples, this is a very good idea.</p>
//- 
//- <p>If the sample is small and used a lot, you should consider using
//- CL_Sample_Raw instead.</p>
//- See Also - CL_Sample_Raw - The static raw sample provider.
//- See Also - CL_Sample - The static wave sample provider.
//- See Also - CL_Streamed_WaveSample - The streamed wave sample provider.
//- See Also - CL_SoundBuffer - Sound playback class in ClanLib.
//- See Also - CL_StreamSoundProvider - Streamed sound provider interface.
//- See Also - CL_StaticSoundProvider - Static sound provider interface.
class CL_Streamed_RawSample : public CL_StreamSoundProvider
{
public:
//! Construction:
	//: Creates a soundbuffer using this provider.
	//- filename - Filename of the raw PCM stream.
	//- frequency - Default playback frequency.
	//- inputprovider - Input source (datafile, regular file). If NULL
	//- it defaults to a regular file.
	//- looped - True if the streamed should be looped (stream never ends).
	//- Returns - The soundbuffer created.
	static CL_SoundBuffer *create(
		const std::string &filename,
		SoundFormat format,
		int frequency,
		CL_InputSourceProvider *inputprovider = NULL,
		bool looped = false);

	//: Constructs an instance of this class.
	//- filename - Filename of the raw PCM stream.
	//- frequency - Default playback frequency.
	//- inputprovider - Input source (datafile, regular file). If NULL
	//- it defaults to a regular file.
	//- looped - True if the streamed should be looped (stream never ends).
	CL_Streamed_RawSample(
		const std::string &filename,
		SoundFormat format,
		int frequency,
		CL_InputSourceProvider *inputprovider = NULL,
		bool looped = false);

	//: Streamed Raw Sample Destructor
	virtual ~CL_Streamed_RawSample();

//! Operations:
	//: Called by the sound implementation when a session is begun.
	virtual CL_StreamSoundProvider_Session *begin_session();

	//: Called by the sound implementation just before a playback session has
	//: ended.
	virtual void end_session(CL_StreamSoundProvider_Session *session);

//! Implementation:
protected:
	std::string filename;
	CL_InputSourceProvider *inputprovider;
	bool looped;
	SoundFormat format;
	int frequency;
};

//: Streamed RawSample Session Class
class CL_Streamed_RawSample_Session : public CL_StreamSoundProvider_Session
{
protected:
	CL_InputSource *input;

	SoundFormat sample_format;
	int sample_size;
	int sample_freq;
	int sample_left;
	bool looped;

public:
//! Construction:
	//: Streamed RawSample Session Constructor
	CL_Streamed_RawSample_Session(
		CL_InputSource *input,
		SoundFormat format,
		int frequency,
		bool looped);

	//: Streamed RawSample Session Destructor
	virtual ~CL_Streamed_RawSample_Session();

//! Operations:
	//: Stop (Function to be fixed)
	virtual void stop() {}; //FIXME

	//: Play (Function to be fixed)
	virtual bool play() { return false; }; //FIXME

	//: Set position (Function to be fixed)
	virtual bool set_position(int pos) { pos=pos; return false; }; //FIXME
	
	//: Eof
	virtual bool eof() const;

//! Attributes:
	//: Get data
	virtual int get_data(void *data_ptr, int data_requested);

	//: Get frequency
	virtual int get_frequency() const;

	//: Get format
	virtual SoundFormat get_format() const;

};

#endif
