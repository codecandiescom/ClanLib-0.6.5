/*
	$Id: fbdev_target.cpp,v 1.3 2001/09/08 19:12:49 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "fbdev_target.h"
#include "API/Display/Display/display.h"
#include "API/Display/Input/input.h"
#include "Display/Display/FBDev/display_fbdev.h"
#include "Display/Input/TTY/keyboard_tty.h"
#include "Display/Input/X11/joystick_linux.h"

const char *CL_FBDevTarget::get_long_name()
{
	return "Linux fbdev kernel framebuffer";
}

const char *CL_FBDevTarget::get_short_name()
{
	return "fbdev";
}

void CL_FBDevTarget::init()
{
	CL_FBDev_DisplayCard *card = new CL_FBDev_DisplayCard(CL_Display::cards.size());

	CL_Display::cards.push_back(card);

	CL_Input::keyboards.push_back(new CL_TTYKeyboard());
//	CL_Input::pointers.push_back(new CL_Mouse_FBDev());

#ifdef USE_JOY
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,1,0)
	for (int i=0; i<8; i++)
	{
		CL_LinuxJoystick *joy = new CL_LinuxJoystick();
		if (joy->init(i))
			CL_Input::joysticks.push_back(joy);
		else
			delete joy;
	}

#endif
#endif
}

void CL_FBDevTarget::deinit()
{
}
