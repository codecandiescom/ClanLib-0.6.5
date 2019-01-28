/*
	$Id: input.h,v 1.11 2002/01/23 11:47:54 sphair Exp $

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

#ifndef header_input
#define header_input

#include <vector>
#include "../../signals.h"
#include "key.h"

class CL_InputDevice;
class CL_Keyboard;

//: Input component class.
//- CL_Input is the access point to keyboards, joysticks and (mouse) pointers.
class CL_Input
{
public:
//! Construction:
	//: Input Destructor
	virtual ~CL_Input() { return; }

//! Attributes:
	//: List of keyboards available.
	static std::vector<CL_Keyboard *> keyboards;
	
	//: List of joysticks and gamepads available.
	static std::vector<CL_InputDevice *> joysticks;
	
	//: List of mice and digitizers available.
	static std::vector<CL_InputDevice *> pointers;

//! Signals:
	//: Signal signaled upon button press.
	//: The CL_Key contains coordinates as well as button code.
	static CL_Signal_v2<CL_InputDevice *, const CL_Key &> &sig_button_press();

	//: Signal signaled upon button release.
	//: The CL_Key contains coordinates as well as button code.
	static CL_Signal_v2<CL_InputDevice *, const CL_Key &> &sig_button_release();

	//: Signal signaled upon mouse move.
	//: Obsolete, please use CL_Mouse::sig_move instead of this signal!
	static CL_Signal_v3<CL_InputDevice *, int, int> &sig_mouse_move();

private:
	static CL_Signal_v2<CL_InputDevice *, const CL_Key&> signal_button_press;
	static CL_Signal_v2<CL_InputDevice *, const CL_Key&> signal_button_release;
	static CL_Signal_v3<CL_InputDevice *, int, int> signal_mouse_move;
};

#endif
