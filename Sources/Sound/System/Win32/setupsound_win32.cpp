/*
	$Id: setupsound_win32.cpp,v 1.8 2002/05/20 12:57:06 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include "API/Sound/setupsound.h"
#include "API/Sound/sound.h"
#include "Sound/Sound/ClanSound/soundcard_clan.h"
#include "Sound/Sound/ClanSound/cdaudio_win32.h"
#include "Sound/System/Generic/setupsound_generic.h"

#define INITGUID
#include <objbase.h>
#include <initguid.h>

DEFINE_GUID(IID_IDirectSoundNotify, 0xb0210783, 0x89cd, 0x11d0, 0xaf, 0x8, 0x0, 0xa0, 0xc9, 0x25, 0xcd, 0x16);

/////////////////////////////////////////////////////////////////////////////
// CL_SetupSound initialization

static int init_ref_count = 0;

void CL_SetupSound::init(bool register_resources_only)
{
	init_ref_count++;
	if (init_ref_count > 1) return;

	CL_SetupSound_Generic::init();

	if (register_resources_only) return;

	CL_Sound::cards.push_back(new CL_SoundCard_ClanSound());
}
	
/////////////////////////////////////////////////////////////////////////////
// CL_SetupSound deinitialization

void CL_SetupSound::deinit()
{
	init_ref_count--;
	if (init_ref_count > 0) return;

	int size = CL_Sound::cards.size();
	for (int i = 0; i < size; i++)
	{
		delete CL_Sound::cards[i];
	}

	CL_Sound::cards.clear();

	CL_SetupSound_Generic::deinit();
}

void CL_SetupCDAudio::init(bool register_resources_only)
{
	if (register_resources_only) return;

	CL_CDDrive_Win32::init_cdaudio();
}

void CL_SetupCDAudio::deinit()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SetupSound implementation
