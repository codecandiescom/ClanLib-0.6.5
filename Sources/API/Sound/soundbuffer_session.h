/*
	$Id: soundbuffer_session.h,v 1.5 2001/09/22 15:52:18 plasmoid Exp $

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

#ifndef header_soundbuffer_session
#define header_soundbuffer_session

#include <stdlib.h>

class CL_SoundCard;
class CL_SoundPlayBackDesc;
class CL_SoundBuffer_Session
//: CL_SoundBuffer_Session provides control over a playing soundeffect.
//- <p>Whenever a soundbuffer is played, it returns a CL_SoundBuffer_Session
//- class, which can be used to control the sound (its volume, pitch, 
//- pan, position). It can of course also be used to retrigger the sound
//- or to stop it. The soundeffect is stored in the layer1 soundmanager
//- until all references to the sound played are removed, meaning that
//- the soundeffect (session) will be ready to play and remember its
//- state and settings (volume, frequency...) until no CL_SoundBuffer_Session
//- refers to it, via a soundbuffer session reference system.</p>
{
public:
//! Construction:
	//: Creates a soundbuffer session not attached to any object.
	CL_SoundBuffer_Session() { handle = NULL; card = NULL; }

	//: Sound Buffer Session Constructor
	CL_SoundBuffer_Session(void *handle, CL_SoundCard *_card);

	//: Copy constructor.
	CL_SoundBuffer_Session(const CL_SoundBuffer_Session &copy);

	//: Sound Buffer Session Destructor
	~CL_SoundBuffer_Session();

//! Operators:
	//: Copy operator
	CL_SoundBuffer_Session& operator=(const CL_SoundBuffer_Session &session);

//! Attributes:
	//: Get Handle
	void *get_handle() const { return handle; }

	//: Returns a pointer to the card on which the soundbuffer controlled
	//: is played/playing.
	//- Returns - Pointer to soundcard
	CL_SoundCard *get_card() const { return card; }

	//: Returns the current position of the soundeffect
	//- Returns - Current position of soundeffect.
	int get_position() const;
	
	//: <p>Returns the soundeffects relative position, compared
	//: to the total length of the soundeffect.</p>
	//: <p>The value returned will be between 0->1, where 0
	//: means the soundeffect is at the beginning, and 1
	//: means that the soundeffect has reached the end.</p>
	//- Returns - Relative position of the soundeffect.
	float get_position_relative() const;

	//: Sets the soundeffects position to 'new_pos'.
	//- The new position of the soundeffect.
	//- Returns - Returns true if operation completed succesfully.
	bool set_position(int new_pos);
	
	//: Sets the relative position of the soundeffect.
	//: <p>Value must be between 0->1, where 0 sets the 
	//: soundeffect to the beginning, and 1 sets it
	//: to the end of the soundbuffer.</p>
	//- New relative position
	//- Returns - Returns true if operation completed succesfully.
	bool set_position_relative(float new_pos);

	//: Returns the total length of the soundeffect played.
	//: <p>Value returned will be -1 if the length is unknown (in
	//: case of non-static soundeffects like streamed sound)</p>
	//- Returns - Length of the soundeffect.
	int get_length() const;

	//: Returns the frequency of the soundeffect played.
	//- Returns - Frequency of soundeffect played.
	int get_frequency() const;
	
	//: Sets the frequency of the soundeffect played.
	//- New frequency of soundeffect.
	//- Returns - Returns true, if the operation completed succesfully.
	bool set_frequency(int new_freq);

	//: Returns the linear relative volume of the soundeffect.
	//: <p>0 means the soundeffect is muted, 1 means the soundeffect
	//: is playing at "max" volume.</p>
	//- Returns - Volume of soundeffect played.
	float get_volume() const;
	
	//: <p>Returns the current pan (in a measure from -1 -> 1), where
	//: -1 means the soundeffect is only playing in the left speaker, 
	//: and 1 means the soundeffect is only playing in the right speaker.</p>
	//- Returns - Current pan of the soundeffect played.
	float get_pan() const;

//! Operations:
	//: Sets the volume of the soundeffect in a relative measure (0->1)
	//: <p>A value of 0 will effectively mute the sound (although it will
	//: still be sampled), and a value of 1 will set the volume to "max".</p>
	//- new_volume - New volume of soundeffect 
	//- Returns - Returns true if the operation completed succesfully.
	bool set_volume(float new_volume);

	//: Sets the panning of the soundeffect played in measures from -1 -> 1
	//: <p>Setting the pan with a value of -1 will pan the soundeffect to the
	//: extreme left (left speaker only), 1 will pan the soundeffect to the
	//: extreme right (right speaker only).</p>
	//- new_pan - New pan of the soundeffect played.
	//- Returns - Returns true if the operation completed sucecsfully.
	bool set_pan(float new_pan);

	//: Starts playback of the session.
	void play();

	//: Stops playback of the session.
	void stop();

	//: Returns true if the session is playing
	bool is_playing();

	//: Determines whether this session should loop
	//- loop - true if session should loop, false otherwise
	void set_looping(bool loop);
	
	//: Adds the sound filter to the session. See CL_SoundFilter for details.
	//- filter - Sound filter to pass sound through.
	//- delete_filter - If true, the filter will be deleted when the session is destroyed.
	void add_filter(class CL_SoundFilter *filter, bool delete_filter = false);

	//: Remove the sound filter from the session. See CL_SoundFilter for details.
	void remove_filter(class CL_SoundFilter *filter);

//! Implementation:
private:
	void *handle;
	CL_SoundCard *card;
};

#endif
