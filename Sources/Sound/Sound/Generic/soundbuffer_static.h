/*
	$Id: soundbuffer_static.h,v 1.2 2001/05/07 08:36:59 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_soundbuffer_static
#define header_soundbuffer_static

#include "API/Sound/soundbuffer.h"
#include "API/Sound/static_soundprovider.h"
#include "cardsoundbuffer_static.h"
#include "soundbuffer_generic.h"

class CL_SoundBuffer_Generic_Static : public CL_SoundBuffer_Generic
{
protected:
	CL_StaticSoundProvider *provider;
	CL_CardSoundBuffer_Static **card_buffers;
	bool delete_provider;

	float volume, pan;
	int frequency;

public:
	CL_SoundBuffer_Generic_Static(
		CL_StaticSoundProvider *provider,
		bool delete_provider);

	virtual ~CL_SoundBuffer_Generic_Static();
	
	virtual CL_StaticSoundProvider *get_static_provider() const;
	virtual CL_StreamSoundProvider *get_stream_provider() const { return NULL; }

	virtual int get_length() const;
	virtual int get_num_samples() const;

	virtual int get_frequency() const;
	virtual bool set_frequency(int new_freq);

	virtual float get_volume() const;
	virtual bool set_volume(float new_volume);

	virtual float get_pan() const;
	virtual bool set_pan(float new_pan);

	virtual bool is_playing(CL_SoundBuffer_Session **session=NULL, CL_SoundCard *card=NULL) const;
	virtual void stop(CL_SoundCard *card=NULL);

	virtual CL_SoundBuffer_Session play(bool looping=false, CL_SoundCard *card=NULL);
	virtual CL_SoundBuffer_Session prepare(bool looping=false, CL_SoundCard *card=NULL);
};

#endif
