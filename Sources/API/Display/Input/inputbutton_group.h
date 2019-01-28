/*
	$Id: inputbutton_group.h,v 1.6 2002/06/13 14:43:17 mbn Exp $

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

#ifndef header_inputbutton_group
#define header_inputbutton_group

#include <list>
#include "../Input/inputbutton.h"

//: CL_InputButton container class.
//- Groups a set of buttons - if either of them are down, CL_InputButton_Groups is down.
class CL_InputButton_Group : public CL_InputButton
{
	std::list<CL_InputButton*> buttons;
public:
//! Operations:
	//: Adds a input button to the group.
	//- add_button - Button to be added.
	void add(CL_InputButton *add_button)
	{
		buttons.push_back(add_button);
	}

	//: Removes a (previously added) button from the group.
	//- remove_button - Button to be removed.
	void remove(CL_InputButton *remove_button)
	{
		buttons.remove(remove_button);
	}

//! Attributes:
	//: <p>Returns true if just one of the buttons in the button group is down; 
	//: false otherwise.</p>
	//- Returns - True if pressed, false otherwise.
	virtual bool is_pressed()
	{
		for (
			std::list<CL_InputButton*>::iterator it = buttons.begin();
			it != buttons.end();
			it++)
		{
			if ((*it)->is_pressed() == true) return true;
		}

		return false;
	}
};

#endif
