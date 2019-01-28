/*
	$Id: implementation_clansound.cpp,v 1.3 2001/04/28 12:04:36 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_CLANSOUND

#include <iostream>
#include "implementation_clansound.h"
#include "Sound/Sound/ClanSound/soundcard_clan.h"
#include "Sound/Sound/ClanSound/cdaudio_linux.h"
#include "Core/System/Unix/implementation.h"
#include "API/Core/System/error.h"

static CL_Implementation_ClanSound impl_clansound;

extern "C"
{
	DL_PREFIX char *clan_module_identify()
	{
		return "Default Clansound implementation";
	}

	DL_PREFIX char* clan_module_abbreviation()
	{
		return "cls";
	}

	DL_PREFIX void clan_module_init()
	{
		impl_clansound.add_sound();
	}
}

char *clansound_identify() { return clan_module_identify(); }
char *clansound_abbreviation() { return clan_module_abbreviation(); }
void clansound_init() { clan_module_init(); }

void CL_Implementation_ClanSound::add_sound()
{
	try
	{
		CL_Sound::cards.push_back(new CL_SoundCard_ClanSound);
		CL_CDDrive_Linux::init_cdaudio();
	}
	catch (CL_Error error)
	{
		std::cout << "ClanSound: " << error.message << std::endl;
	}
}

#endif
