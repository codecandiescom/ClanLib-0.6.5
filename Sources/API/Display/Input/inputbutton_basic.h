/*
	$Id: inputbutton_basic.h,v 1.5 2001/09/22 15:52:12 plasmoid Exp $

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

#ifndef header_inputbutton_basic
#define header_inputbutton_basic

#include "../Input/inputbutton.h"

//: <p>CL_InputButton interface used to ease manual implementation of a input 
//: button.</p>
class CL_InputButton_Basic : public CL_InputButton
{
protected:
	bool state;

public:
//! Construction:
	//: Constructs an input button defaulting to not being pressed (false).
	CL_InputButton_Basic()
	{
		state = false;
	}
	
	//: Constructs an input button defaulting to 'state'.
	CL_InputButton_Basic(bool state)
	{
		this->state = state;
	}
	
	//: Input Button Basic Destructor
	virtual ~CL_InputButton_Basic() {;}

//! Attributes:
	//: Returns true if the button is pressed.
	//- Returns - True if pressed, false otherwise.
	virtual bool is_pressed()
	{
		return state;
	}
	
//! Operations:
	//: <p>Changes the current button state. If 'state' is true, the button will be
	//: up, otherwise down.</p>
	//- state - New button state. True = up, False = down.
	void set_state(bool state)
	{
		this->state = state;
	}
};

#endif
