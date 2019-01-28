/*
	$Id: static_provider_wave.h,v 1.7 2002/06/07 19:41:16 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

	File purpose:
		Simple sample support.

*/

//! clanSound="Sound Providers"
//! header=sound.h

#ifndef header_sampleprovider
#define header_sampleprovider

#include "../static_soundprovider.h"
#include "../soundbuffer.h"

//: ClanLib's datafile samples. This class can read samples from datafiles.
class CL_Sample : public CL_StaticSoundProvider
{
public:
//! Construction:
	//: Loads the sample 'sample_id' from the inputsource provider 'provider'. 
	//: Creates a CL_SoundBuffer using this sample and returns it.
	static CL_SoundBuffer *create(
		const std::string &sample_id,
		CL_InputSourceProvider *provider);

	//: Constructs a static sound provider that represents 'sample_id' from the
	//: inputsource provider 'provider'.
	CL_Sample(
		const std::string &sample_id,
		CL_InputSourceProvider *_provider);

	//: Sample Destructor
	virtual ~CL_Sample();

//! Operations:
	//: Locks the sample data.
	virtual void lock();

	//: Unlocks the sample data.
	virtual void unlock();

//! Attributes:
	//: Returns the sample format used by this sample.
	virtual SoundFormat get_format() const;

	//: Returns the size of the sample.
	virtual int data_size() const;

	//: Returns the sample data. Must be locked before this is done.
	virtual void *get_data() const;

	//: Returns the default frequency used when playing this sample.
	virtual int get_frequency() const;

//! Implementation:
private:
	struct WAVE_FORMAT
	{
		short formatTag;
		short nChannels;
		int nSamplesPerSec;
		int nAvgBytesPerSec;
		short nBlockAlign;
	};

	CL_InputSourceProvider *provider;
	std::string sample_id;

	signed char *sample_data;
	SoundFormat sample_format;
	int sample_size;
	int sample_freq;

	void load_data();
};

#endif
