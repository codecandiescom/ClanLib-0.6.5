/*
	$Id: sound.h,v 1.7 2002/06/13 14:43:17 mbn Exp $

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

#ifndef header_sound
#define header_sound

#include <vector>
#include <stdlib.h>
#include "soundbuffer.h"
#include "../Core/System/cl_assert.h"

//: Soundcard interface in ClanLib.
//- <p>CL_SoundCard is the interface to a soundcard. It is used to control the
//- main mixer volume and other global settings. It also provides access to
//- the microphone and line in as streamed soundproviders.</p>
class CL_SoundCard
{
public:
//! Variables:
	//: Name of the soundcard.
	std::string name;

	//: Card number.
	int card_no;

//! Construction:
	//: Sound Card destructor
	virtual ~CL_SoundCard() { ; }

//! Attributes:
	//: Returns the streamed soundprovider representing the microphone.
	virtual CL_StreamSoundProvider *get_microphone()=0;

	//: Returns the streamed soundprovider representing the line in.
	virtual CL_StreamSoundProvider *get_line_in()=0;

//! Operations:
	//: Stops all sample playbacks on the soundcard.
	virtual void stop_all()=0;
	
	//: Sets the main/mixer volume on the soundcard.
	virtual void set_global_volume(int volume)=0;

	//: Sets the main panning position on the soundcard.
	virtual void set_global_pan(int pan)=0;
};

class CL_Sound
//: Sound interface in ClanLib.
//- <p>This class provides a list of all soundcards available on the system. It
//- is also a wrapper class for the CL_SoundCard class, providing an easier
//- access to a single selected soundcard.</p>
//- 
//- <p>All the functions that share name with those in CL_SoundCard have the
//- same functionality. The only difference is, that the ones CL_Sound
//- operate on a selected soundcard. This saves the trouble of passing around
//- a pointer to the soundcard, when only a single one is used anyway.</p>
//- See also - CL_SoundCard - Soundcard interface class.
//- See also - CL_SoundBuffer - Sample class in ClanLib.
{
public:
//! Construction:
	//: Sound Destructor
	virtual ~CL_Sound() { ; }

//! Variables:
	//: The list of soundcards available on this system.
	static std::vector<CL_SoundCard*> cards;
	
	//: List of all cards where soundbuffers are automatically preloaded.
	static std::list<CL_SoundCard*> preload_cards;

//! Attributes:
	//: Returns the streamed soundprovider representing the microphone.
	//- Returns - The microphone.
	static CL_StreamSoundProvider *get_microphone();

	//: Returns the streamed soundprovider representing the line in.
	//- Returns - The line in.
	static CL_StreamSoundProvider *get_line_in();

	//: Returns the current selected soundcard.
	//- Returns - Current soundcard.
	static CL_SoundCard *get_current_card();

//! Operations:
	//: Clears the list of preloaded soundcards.
	static void clear_card_preload();

	//: <p>Adds 'card' to the preloaded soundcards list. This means all soundbuffers 
	//: will be automatically preloaded onto the card. By default the first card 
	//: is always on the preload list.</p>
	//:
	//: <p>If a card isn't on the list, the soundbuffer's data will first be loaded when 
	//: the sample is actually played for the first time.</p>
	//- card - The card to be added to the preload list.
	static void add_card_preload(CL_SoundCard *card);

	//: Removes the specified card from the preload list.
	//- card - The card to be removed from the list.
	static void remove_card_preload(CL_SoundCard *card);

	//: Change the current selected soundcard to 'card'.
	//- card - The new current selected soundcard.
	static void select_card(CL_SoundCard *card);
	
	//: Change the current selected soundcard to soundcard number 'card_no'.
	//- card_no - Card number in the CL_Sound::cards list.
	static void select_card(int card_no);
};

#endif
