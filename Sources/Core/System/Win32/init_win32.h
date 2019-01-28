/*
	$Id: init_win32.h,v 1.2 2000/07/15 15:45:55 starch Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		clanCore initialization headers and win32 event routing support.
*/

#ifndef header_init_win32
#define header_init_win32

#include <list>
#include "API/Core/System/system.h"

class CL_Win32EventListener;

class CL_System_Win32 : public CL_System
{
public:
	static void add_listener(CL_Win32EventListener *listener);
	static void remove_listener(CL_Win32EventListener *listener);

	static HINSTANCE hInstance;

// private, don't access except from init_win32.cpp:
	static std::list<CL_Win32EventListener*> listeners;
};

class CL_Win32EventListener
{
public:
	virtual bool received_event(UINT uMsg, WPARAM wParam, LPARAM lParam)=0;
};

#endif
