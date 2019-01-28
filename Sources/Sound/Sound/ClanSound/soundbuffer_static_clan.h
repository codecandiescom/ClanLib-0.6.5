/*
	$Id: soundbuffer_static_clan.h,v 1.3 2001/10/14 10:28:00 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Sound card impl. that uses the ClanSound library.

*/

#ifndef header_soundbuffer_static_clan
#define header_soundbuffer_static_clan

#ifdef USE_CLANSOUND

#include "soundcard_clan.h"
#include "Sound/Sound/Generic/soundbuffer_static.h"

class CL_SoundBuffer_Generic;

class CL_CardBuffer_Static_ClanSound : public CL_CardSoundBuffer_Static
{
protected:
	CL_SoundCard_ClanSound *card;
	short *data;
	int num_samples;
	CL_SoundBuffer_Generic *owner;
	SoundFormat format;

public:
	CL_CardBuffer_Static_ClanSound(
		CL_SoundCard_ClanSound *card,
		CL_StaticSoundProvider *provider,
		CL_SoundBuffer_Generic *owner);

	virtual ~CL_CardBuffer_Static_ClanSound();

	virtual CL_CardSoundBuffer_Playback *prepare();
	
	int get_size() { return num_samples; }
	short *get_data16() { return (short *) data; }
	char *get_data8() { return (char *) data; }
	SoundFormat get_format() { return format; }
	
	CL_SoundBuffer_Generic *get_owner() { return owner; }
};

#endif /* USE_CLANSOUND */

#endif
