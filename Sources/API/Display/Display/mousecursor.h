/*
	$Id: mousecursor.h,v 1.5 2001/09/22 15:52:11 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanDisplay="Display 2D"
//! header=display.h

#ifndef header_mousecursor
#define header_mousecursor

class CL_MouseCursorProvider;

//: Mouse cursor management class.
//- <p>Use this class to set the mouse cursor icon, or to toggle its
//- visibility.</p>
class CL_MouseCursor
{
public:
//! Operations:
	//: <p>Set the system mouse cursor to 'provider'. If 'frame' is also
	//: specified, it will use that subframe in the cursor provider.</p>
	//- provider - Mouse cursor to use.
	//- frame - Subframe in the mouse cursor to use.
	static void set_cursor(
		CL_MouseCursorProvider *provider,
		int frame=0);

	//: <p>Makes the currently selected mouse cursor visible. If 'show_async' is
	//: true, it will use a secondary thread to draw the mouse. This ensures
	//: that the mouse cursor will keep running smooth despite of the
	//: framerate in the game.</p>
	//- show_async - True if a secondary thread should be used for
	//-              mouse painting.
	static void show(bool show_async=true);

	//: Hides the mouse cursor.
	static void hide();
};

#endif
