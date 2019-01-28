/*
	$Id: inputbutton.h,v 1.5 2001/09/22 15:52:12 plasmoid Exp $

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

#ifndef header_input_button
#define header_input_button

//: Input Button interface.
//- <p>CL_InputButton is the interface to buttons, such as a keyboard, mouse or
//- joystick button.</p>
class CL_InputButton
{
public:
//! Construction:
	//: Input Button Destructor
	virtual ~CL_InputButton() { return; }

//! Attributes:
	//: Returns true if the button is pressed.
	//- Returns - True if button is pressed.
	virtual bool is_pressed()=0;
};

#endif
