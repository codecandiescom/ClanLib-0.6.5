/*
	$Id: inputbutton_to_axis_digital.h,v 1.5 2001/09/22 15:52:12 plasmoid Exp $

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

#ifndef header_inputbutton_to_axis_digital
#define header_inputbutton_to_axis_digital

#include "../Input/inputbutton.h"
#include "../Input/inputaxis.h"

//: Converts two CL_InputButton's into one digital CL_InputAxis. 
class CL_InputButtonToAxis_Digital : public CL_InputAxis
{
	CL_InputButton *left;
	CL_InputButton *right;

public:
//! Construction:
	//: <p>Constructs a button to axis converter using the two buttons 'left' 
	//: and 'right'.</p>
	CL_InputButtonToAxis_Digital(
		CL_InputButton *_left,
		CL_InputButton *_right)
	{
		left = _left;
		right = _right;
	}

//! Attributes:
	//: Returns the axis position.
	//- Returns - Current axis position.
	virtual float get_pos()
	{
		if (left->is_pressed()) return -1;
		if (right->is_pressed()) return 1;
		return 0;
	}
};

#endif
