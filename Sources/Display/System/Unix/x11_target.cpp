/*
	$Id: x11_target.cpp,v 1.10 2002/06/12 14:35:23 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "x11_target.h"
#include "API/Core/System/error.h"
#include "API/Display/Display/display.h"
#include "API/Display/Input/input.h"
#include "Display/Display/X11/display_xwindow.h"
#include "Display/Input/X11/keyboard_x11.h"
#include "Display/Input/X11/mouse_x11.h"
#include "Display/Input/X11/joystick_linux.h"

#include <iostream>

Display *CL_X11Target::dpy = NULL;
Window CL_X11Target::root;
int CL_X11Target::default_scr;
int CL_X11Target::display_counter=0;

const char *CL_X11Target::get_long_name()
{
	return "X Window";
}

const char *CL_X11Target::get_short_name()
{
	return "x11";
}

void CL_X11Target::init()
{
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) throw CL_Error("Failed to open display. No XWindow display available.");

	default_scr = DefaultScreen(dpy);
	root = RootWindow(dpy, default_scr);

	CL_XWindow_CompatibleCard *card = create_displaycard(dpy, root, display_counter);

	CL_Display::cards.push_back(card);

	CL_Input::keyboards.push_back(new CL_XWindowKeyboard(card));
	CL_Input::pointers.push_back(new CL_Mouse_XWin(card));

#ifdef USE_JOY
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,1,0)
//#ifdef USE_JOY
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

void CL_X11Target::deinit()
{
//	XCloseDisplay(dpy);
}

CL_XWindow_CompatibleCard *CL_X11Target::create_displaycard(Display *dpy, Window root, int display_counter)
{
	return new CL_XWindow_DisplayCard(
		dpy,
		root,
		display_counter);
}
