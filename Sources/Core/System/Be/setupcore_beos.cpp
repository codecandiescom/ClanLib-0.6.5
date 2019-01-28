/*
	$Id: setupcore_beos.cpp,v 1.2 2001/03/01 09:50:53 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		SetupCore: Initialization of the BeOS version of ClanLib.

*/

#include "API/Core/System/setupcore.h"
#include "API/Core/Display/displaycard.h"
#include "API/Core/Display/display.h"
#include "API/Core/Sound/sound.h"
#include "API/Core/Input/input.h"
#include "Core/System/Be/app_beos.h"
#include "Core/Display/Be/displaycard_be.h"
#include "Core/Display/Be/clanwindowscreen.h"
// #include "Core/Input/Be/keyboard_be.h"

CL_DisplayCard *cl_current_displaycard = NULL;

CL_DisplayCard* CL_Display::get_current_card()
{
	return cl_current_displaycard;
}

CL_SoundCard *cl_current_soundcard = NULL;

CL_SoundCard* CL_Sound::get_current_card()
{
	return cl_current_soundcard;
}

void CL_SetupCore::init_display()
{
	status_t err;
	((LibApplication*)be_app)->clanscreen = new ClanWindowScreen(&err);

	cl_current_displaycard = new CL_DisplayCard_Be(CL_Display::cards.size());
	CL_Display::cards.push_back(cl_current_displaycard);
	
//	CL_Input::keyboards.add(new CL_BeKeyboard());
}

void CL_SetupCore::init_sound()
{
//	cl_current_soundcard = new CL_SoundCard_Be();
//	CL_Sound::cards.push_back(cl_current_soundcard);
//	CL_CDDrive_Be::init_cdaudio();
}

void CL_SetupCore::init_network()
{
}

void CL_SetupCore::deinit_display()
{
}

void CL_SetupCore::deinit_sound()
{
}

void CL_SetupCore::deinit_network()
{
}
