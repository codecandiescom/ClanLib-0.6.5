/*
	$Id: soundbuffer_generic.h,v 1.2 2001/05/07 08:36:59 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_soundbuffer_generic
#define header_soundbuffer_generic

#include "API/Sound/soundbuffer_session.h"

class CL_SoundBuffer_Generic
{
public:
	CL_SoundBuffer_Generic();
	virtual ~CL_SoundBuffer_Generic();

	int add_reference();
	int release_reference();

	virtual class CL_StaticSoundProvider *get_static_provider() const=0;
	virtual class CL_StreamSoundProvider *get_stream_provider() const=0;
	virtual int get_length() const=0;
	virtual int get_num_samples() const=0;
	virtual int get_frequency() const=0;
	virtual bool set_frequency(int new_freq)=0;
	virtual float get_volume() const=0;
	virtual bool set_volume(float new_volume)=0;
	virtual float get_pan() const=0;
	virtual bool set_pan(float new_pan)=0;
	virtual bool is_playing(CL_SoundBuffer_Session **session=NULL, class CL_SoundCard *card=NULL) const=0;
	virtual void stop(class CL_SoundCard *card=NULL)=0;
	virtual CL_SoundBuffer_Session play(bool looping=false, class CL_SoundCard *card=NULL)=0;
	virtual CL_SoundBuffer_Session prepare(bool looping=false, class CL_SoundCard *card=NULL)=0;

private:
	int ref;
};

#endif
