/*
	$Id: mouse.h,v 1.9 2002/05/17 08:47:58 grumbel Exp $

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

#ifndef header_mouse
#define header_mouse

#include "inputdevice.h"
#include "key.h"
#include "../../signals.h"

//: Mouse interface in ClanLib.
//- <p>CL_Mouse extends the CL_InputDevice with functionality only useful
//- to mice.</p>
class CL_Mouse : public CL_InputDevice
{
public:
//! Construction:
	//: Mouse Destructor
	virtual ~CL_Mouse() { return; }

//! Attributes:
	//- <p>Returns - Returns the x position of the mouse in range from
	//- [ 0 - CL_Display::get_height() ]. </p>
	static int get_x(); 

	//- <p>Returns - Returns the y position of the mouse in range from
	//- [ 0 - CL_Display::get_height() ]. </p>
	static int get_y();

	//- <p>Set the mouse cursor to the position x, y</p>
	static void set_position (float x, float y);

	//- <p>Returns - Returns true if the left mouse button is pressed,
	//- else false is returned.</p>
	static bool left_pressed();

	//- <p>Returns - Returns true if the middle mouse button is pressed,
	//- else false is returned.</p>
	static bool middle_pressed();

	//- <p>Returns - Returns true if the right mouse button is pressed,
	//- else false is returned.</p>
	static bool right_pressed();

//! Signals:
	//: Signal signaled upon mouse move.
	static CL_Signal_v2<int, int> &sig_move();

	//: Signal signaled upon button press.
	//: The CL_Key contains coordinates as well as button code.
	static CL_Signal_v1<const CL_Key &> &sig_button_press();

	//: Signal signaled upon button release.
	//: The CL_Key contains coordinates as well as button code.
	static CL_Signal_v1<const CL_Key &> &sig_button_release();

private:
	static CL_Signal_v2<int, int> signal_move;
	static CL_Signal_v1<const CL_Key &> signal_button_press;
	static CL_Signal_v1<const CL_Key &> signal_button_release;
};

#endif
