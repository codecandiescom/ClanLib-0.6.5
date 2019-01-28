/*
	$Id: soundcard_be.h,v 1.1 2001/03/06 15:09:24 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_soundcard_be
#define header_soundcard_be

#include "Core/Sound/Generic/soundcard_generic.h"

#include <GameSoundDefs.h>
#include <SimpleGameSound.h>

class CL_SoundCard_Be : public CL_SoundCard_Generic
{
public:
	CL_SoundCard_Be();
	virtual ~CL_SoundCard_Be();

	// Member vars from CL_SoundCard:
//	CL_String name;
//	int card_no;

	// Functions inherited from API:
	// -----------------------------
	virtual CL_StreamSoundProvider *get_microphone();
	virtual CL_StreamSoundProvider *get_line_in();

	virtual void stop_all();
	virtual void set_global_volume(int volume);
	virtual void set_global_pan(int pan);

	// Functions inherited from Generic:
	// ---------------------------------
	virtual CL_CardSoundBuffer_Static *create_soundbuffer_static(
		CL_StaticSoundProvider *provider);
		
	virtual CL_CardSoundBuffer_Playback *create_cardsoundbuffer_playback_streamed(
		CL_SoundBuffer_Generic_Stream *provider, CL_SoundPlayBackDesc *desc);
};

#endif
