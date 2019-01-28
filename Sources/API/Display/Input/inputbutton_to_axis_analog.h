/*
	$Id: inputbutton_to_axis_analog.h,v 1.5 2001/09/22 15:52:12 plasmoid Exp $

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

#ifndef header_inputbutton_to_axis_analog
#define header_inputbutton_to_axis_analog

#include "../Input/inputbutton.h"
#include "../Input/inputaxis.h"

//: Converts two CL_InputButton's into one analog CL_InputAxis. 
class CL_InputButtonToAxis_Analog : public CL_InputAxis
{
	CL_InputButton *left;
	CL_InputButton *right;
	float cur_pos;

public:
//! Construction:
	//: <p>Constructs a button to axis converter using the two buttons 'left' 
	//: and 'right'.</p>
	CL_InputButtonToAxis_Analog(CL_InputButton *left, CL_InputButton *right)
	{
		this->left = left;
		this->right = right;
		cur_pos = 0;
	}

//! Attributes:
	//: Returns the axis position.
	//- Returns - Current axis position.
	virtual float get_pos()
	{
		if (left->is_pressed() && cur_pos>-0.1) cur_pos=(float)-0.1;
		else if (right->is_pressed() && cur_pos<0.1) cur_pos=(float)0.1;

		if (left->is_pressed() || right->is_pressed()) cur_pos*=(float)1.2;
		else cur_pos*=(float)0.8;

		if (cur_pos>1) cur_pos=1;
		else if (cur_pos<-1) cur_pos=-1;

		return cur_pos;
	}
};

#endif
