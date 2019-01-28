/*
	$Id: soundbuffer_static_dx.h,v 1.2 2001/09/08 19:24:21 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#ifndef header_soundbuffer_dx
#define header_soundbuffer_dx

#include "Core/Sound/Generic/soundcard_generic.h"
#include "Core/Sound/Generic/soundbuffer_static.h"
#include "soundcard_dx.h"

class CL_CardSoundBufferStatic_DX : public CL_CardSoundBuffer_Static
{
public:
	CL_SoundCard_DX *parent_card;
	CL_SoundBuffer_Generic *buffer;
	LPDIRECTSOUNDBUFFER soundbuffer;
	void reload_data(bool lock_provider);

	CL_CardSoundBufferStatic_DX(
		CL_SoundCard_DX *card,
		CL_SoundBuffer_Generic *buffer,
		CL_StaticSoundProvider *provider);

	virtual ~CL_CardSoundBufferStatic_DX();

	virtual CL_CardSoundBuffer_Playback *prepare();
};

#endif