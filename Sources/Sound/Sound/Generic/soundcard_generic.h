/*
	$Id: soundcard_generic.h,v 1.1 2001/03/06 15:09:26 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_soundcard_generic
#define header_soundcard_generic

#include "API/Sound/sound.h"

class CL_CardSessionManager;
class CL_CardSoundBuffer_Static;
class CL_CardSoundBuffer_Playback;
class CL_SoundBuffer_Generic_Stream;

class CL_SoundCard_Generic : public CL_SoundCard
{
public:
	CL_CardSessionManager *manager;

	CL_SoundCard_Generic(); // { manager = new CL_CardSessionManager(); }
	virtual ~CL_SoundCard_Generic(); // { delete manager; }
	
	virtual CL_CardSoundBuffer_Static *create_soundbuffer_static(
		class CL_SoundBuffer_Generic *owner,
		CL_StaticSoundProvider *provider)=0;
		
	virtual CL_CardSoundBuffer_Playback *create_cardsoundbuffer_playback_streamed(
		CL_SoundBuffer_Generic_Stream *soundbuffer)=0;
};

#endif
