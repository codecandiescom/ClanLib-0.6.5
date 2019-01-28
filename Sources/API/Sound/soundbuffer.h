/*
	$Id: soundbuffer.h,v 1.11 2001/12/11 20:44:20 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanSound="Audio Mixing"
//! header=sound.h

#ifndef header_soundbuffer
#define header_soundbuffer

#include "soundbuffer_session.h"
#include "../Core/Resources/resource_manager.h"
#include "../Core/Resources/resource.h"

class CL_StaticSoundProvider;
class CL_StreamSoundProvider;
class CL_SoundCard;
class CL_SoundBuffer_Generic;

class CL_SoundBuffer
//: Sample interface in ClanLib.
//- <p>The CL_SoundBuffer class represents a sample in ClanLib. It can either be
//- static or streamed. The soundbuffer gets its sample data from a
//- soundprovider, that is passed during construction.</p>
//-
//- <p>A static sample are normally sound effects and other sounds, that do not
//- change. ClanLib will always load the entire sample into memory, and
//- possibly upload it to the soundcard.</p>
//-
//- <p>Streamed samples are either large sample that should be loaded a bit at a
//- time (music for instance), or sounds that change from playback to
//- playback (a microphone, speech over the net, etc).</p>
//- See also - CL_StaticSoundProvider - Static sample data source.
//- See also -CL_StreamSoundProvider - Streamed sample data source.
//- See also -CL_SoundBuffer_Session - Sample playback control.
{
public:
//! Construction:
	//: Sound Buffer Destructor
	virtual ~CL_SoundBuffer();

	//: Load
	static CL_SoundBuffer *load(
		const std::string &res_id,
		CL_ResourceManager *manager);

	//: <p>Creates a static soundbuffer. If 'delete_provider' is true, the provider
	//: will be deleted when the soundbuffer is deleted.</p>
	//- Returns - The created soundbuffer.
	static CL_SoundBuffer *create(
		CL_StaticSoundProvider *provider,
		bool delete_provider=false);
	
	//: <p>Creates a streamed soundbuffer. If 'delete_provider' is true, the provider
	//: will be delete when the soundbuffer is deleted.</p>
	//- Returns - The created soundbuffer.
	static CL_SoundBuffer *create(
		CL_StreamSoundProvider *provider,
		bool delete_provider=false);
	
	//: Construct unattached sound buffer.
	CL_SoundBuffer();
	
	//: Loads a soundbuffer (sample) from a resource.
	CL_SoundBuffer(
		const std::string &res_id,
		CL_ResourceManager *manager);

	//: <p>Creates a static soundbuffer. If 'delete_provider' is true, the provider
	//: will be deleted when the soundbuffer is deleted.</p>
	CL_SoundBuffer(
		CL_StaticSoundProvider *provider,
		bool delete_provider=false);
	
	//: <p>Creates a streamed soundbuffer. If 'delete_provider' is true, the provider
	//: will be delete when the soundbuffer is deleted.</p>
	CL_SoundBuffer(
		CL_StreamSoundProvider *provider,
		bool delete_provider=false);

	//: <p>Create a copy of this soundbuffer. The copy will share the buffer with the
	//: original.</p>
	CL_SoundBuffer(const CL_SoundBuffer &copy);

//! Operators:
	//: Copy assignment.
	void operator =(const CL_SoundBuffer &copy);

//! Attributes:
	//: <p>Returns the static soundprovider attached or NULL if it is a streamed
	//: sound buffer.</p>
	//- Returns - The static soundprovider or NULL if none.
	CL_StaticSoundProvider *get_static_provider() const;

	//: <p>Return the streamed soundprovider attached or NULL if it is a static
	//: sound buffer.</p>
	//- Returns - The streamed soundprovider of NULL if none.
	CL_StreamSoundProvider *get_stream_provider() const;

	//: Returns the length of the soundbuffer. 
	//: That is num_samples * bytes_per_sample.
	//- Returns - Length of soundbuffer.
	int get_length() const;
	
	//: Returns the number of samples in the soundbuffer.
	//- Returns - Number of samples in soundbuffer.
	int get_num_samples() const;

	//: Returns the start frequency used when the buffer is played.
	//- Returns - Default/start frequency.
	int get_frequency() const;
	
//! Operations:
	//: Sets the start frequency used when the buffer is played.
	//- new_freq - The new default frequency.
	//- Returns - True if successful, false otherwise.
	bool set_frequency(int new_freq);

	//: Returns the start/default volume used when the buffer is played.
	//- Returns - The volume.
	float get_volume() const;
	
	//: Sets the default volume used when the buffer is played.
	//- new_volume - The new default volume.
	//- Returns - True if successful, false otherwise.
	bool set_volume(float new_volume);

	//: Returns the default panning position when the buffer is played.
	//- Returns - The panning position.
	float get_pan() const;

	//: Sets the default panning position when the buffer is played.
	//- new_pan - The new default panning position.
	//- Returns - True if successful, false otherwise.
	bool set_pan(float new_pan);

	//: Returns true if an instance of this soundbuffer is playing
	//- session - Pointer to session-pointer, set to point to the first session 
	//-           playing this soundbuffer. Return value is only valid if function returns true
	bool is_playing(CL_SoundBuffer_Session **session=NULL, CL_SoundCard *card=NULL) const;

	//: Stops any sessions playing this soundbuffer
	void stop(CL_SoundCard *card=NULL);

	//: <p>Plays the soundbuffer on the specified soundcard 
	//: and using the specified playback description.</p>
	//- card - Soundcard to be used - NULL means use the current selected
	//-        sound card (CL_Sound::get_selected_card().
	//- Returns - The playback session.
	CL_SoundBuffer_Session play(bool looping=false, CL_SoundCard *card=NULL);

	//: Prepares the soundbuffer for playback on the specified soundcard.
	//- card - Soundcard to be used or NULL to use
	//-        CL_Sound::get_selected_card().
	//- Returns - The playback session.
	CL_SoundBuffer_Session prepare(bool looping=false, CL_SoundCard *card=NULL);

//! Implementation:
public:
	CL_SoundBuffer_Generic *impl;
	CL_Resource resource;
};

#endif
