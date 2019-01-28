/*
	$Id: displaycard_win32compatible.h,v 1.3 2002/05/20 13:06:49 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#ifndef header_displaycard_win32compatible
#define header_displaycard_win32compatible

#include "API/core/Math/point.h"
#include "../Generic/displaycard_generic.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Core/System/Win32/init_win32.h"

class CL_DisplayCard_Win32Compatible : public CL_DisplayCard_Generic, CL_Win32EventListener
{
public:
	CL_DisplayCard_Win32Compatible(int card_no);
	virtual ~CL_DisplayCard_Win32Compatible();

	HWND get_hwnd() { return hwnd; }

	void create_window(
		int width,
		int height,
		int bpp,
		bool full_screen,
		bool allow_resize);

	virtual void show_system_cursor();
	virtual void hide_system_cursor();

protected:
	void destroy_window();

	HWND hwnd; //This needs to be inherited

private:
	virtual bool received_event(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//: Calculates the size of a window (including the borders and the title bar), given its client area dimensions.
	//- dimensions - Pointer to a CL_Point containing the width and height of the client area. When the method returns contains the dimensions of the window.
	//- dwStyle - Style of the window.
	//- bMenu - Specifies whether the window has a menu (default = false).
	void get_window_size( CL_Point *dimensions, DWORD dwStyle, BOOL bMenu = false );

	//: Calculates the top-left coordinates of a centralized window, given its dimensions.
	//- dimensions - Pointer to a CL_Point containing the width and height of a window. When the method returns contains the coordinates.
	void centralize_window( CL_Point *dimensions );

};

#endif
