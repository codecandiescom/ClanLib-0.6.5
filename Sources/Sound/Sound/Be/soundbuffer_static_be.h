/*
	$Id: soundbuffer_static_be.h,v 1.1 2001/03/06 15:09:24 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Sound card impl. that uses the ClanSound library.

*/

#ifndef header_soundbuffer_static_be
#define header_soundbuffer_static_be

#include "soundcard_be.h"
#include "Core/Sound/Generic/soundbuffer_static.h"

class CL_CardBuffer_Static_Be : public CL_CardSoundBuffer_Static
{
protected:
	BSimpleGameSound* soundbuffer;

public:
	CL_CardBuffer_Static_Be(CL_StaticSoundProvider *provider);
	virtual ~CL_CardBuffer_Static_Be();

	virtual CL_CardSoundBuffer_Playback *prepare(CL_SoundPlayBackDesc *desc);
};

#endif
