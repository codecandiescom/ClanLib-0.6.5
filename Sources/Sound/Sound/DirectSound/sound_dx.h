/*
	$Id: sound_dx.h,v 1.2 2001/09/08 19:24:21 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#ifndef header_sound_dx
#define header_sound_dx

#include "API/Core/Sound/sound.h"

struct SSoundCardInfo
{
	GUID guid;
	std::string name;
};

class CL_SoundCard_DX;
class CL_Sound_DX : public CL_Sound
{
public:
	CL_Sound_DX();
	virtual ~CL_Sound_DX();

	static CL_SoundCard *cur_card;
};

#endif
