/*
	$Id: keyboard.h,v 1.5 2001/09/22 15:52:12 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanDisplay="Input"
//! header=display.h

#ifndef header_keyboard
#define header_keyboard

#include "inputdevice.h"

//: Keyboard interface in ClanLib.
//- <p>This is the keyboard input device interface. It contains all the extra
//- functions needed by the keyboard, which didn't fit into the general
//- CL_InputDevice abstraction.</p>
class CL_Keyboard : public CL_InputDevice
{
public:
//! Construction:
	//: Keyboard Destructor
	virtual ~CL_Keyboard() { return; }

//! Attributes:
	//: Returns true if the specified button is down on the keyboard.
	//: <p>This static function is an easier way of writing 
	//: CL_Input::keyboards[0]->get_button->is_pressed();</p>
	//- button_no - Button number checked for being pressed.
	//- Returns - True if the key is pressed.
	static bool get_keycode(int button_no);
};

#endif
