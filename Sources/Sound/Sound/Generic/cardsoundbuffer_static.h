/*
	$Id: cardsoundbuffer_static.h,v 1.1 2001/03/06 15:09:26 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_cardsoundbuffer_static
#define header_cardsoundbuffer_static

#include "API/Sound/static_soundprovider.h"
#include "API/Sound/sound.h"

class CL_CardSoundBuffer_Playback;
class CL_CardSoundBuffer_Static
{
public:
	CL_StaticSoundProvider *provider;

	CL_CardSoundBuffer_Static(CL_StaticSoundProvider *_provider)
	{
		provider = _provider;
	}

	virtual ~CL_CardSoundBuffer_Static() { ; }
	virtual CL_CardSoundBuffer_Playback *prepare()=0;
};

#endif
