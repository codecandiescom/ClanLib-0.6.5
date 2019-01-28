/*
	$Id: soundcard_dx.h,v 1.2 2001/09/08 19:24:21 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#ifndef header_soundcard_dx
#define header_soundcard_dx

#include "Core/Sound/Generic/soundcard_generic.h"

class CL_SoundCard_DX : public CL_SoundCard_Generic
{
	bool is_initialized;
	std::string name;
	LPGUID guid;
	bool soundcard_enabled;

	void init_directsound();
public:
	LPDIRECTSOUND soundcard;

	CL_SoundCard_DX(int card_no, std::string _name, LPGUID guid);
	virtual ~CL_SoundCard_DX();

	virtual CL_CardSoundBuffer_Static *create_soundbuffer_static(
		CL_SoundBuffer_Generic *owner,
		CL_StaticSoundProvider *provider);

	CL_CardSoundBuffer_Playback *create_cardsoundbuffer_playback_streamed(
		CL_SoundBuffer_Generic_Stream *soundbuffer);

	virtual CL_StreamSoundProvider *get_microphone() { return NULL; }
	virtual CL_StreamSoundProvider *get_line_in() { return NULL; }

	virtual void stop_all() { ; }
	virtual void set_global_volume(int volume) { ; }
	virtual void set_global_pan(int pan) { ; }

};

#endif
