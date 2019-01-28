/*
	$Id: radiogroup.h,v 1.4 2002/03/01 19:36:54 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanGUI="Controls"
//! header=gui.h

#ifndef header_radiogroup
#define header_radiogroup

#include "../signals.h"
#include <vector>

class CL_RadioGroup_Generic;
class CL_RadioButton;

//: RadioGroup
class CL_RadioGroup
{
//! Construction:
public:
	//: RadioGroup Constructor
	CL_RadioGroup();

	//: RadioGroup Destructor
	~CL_RadioGroup();

//! Attributes:
public:
	//: Returns the amount of buttons added to the group.
	int get_count() const;
	
	//: Returns the selected button if any, NULL otherwise.
	CL_RadioButton *get_toggled() const;

	//: Returns all buttons in the group.
	const std::vector<CL_RadioButton *> &get_buttons() const;
	
//! Operations:
public:
	//: Adds a component into the group.
	void add(CL_RadioButton *button, bool delete_component = false);
	
	//: Removes a component from the group.
	void remove(CL_RadioButton *button);
	
//! Signals:
public:
	//: This signal is emitted when the radio selection is changed.
	CL_Signal_v1<CL_RadioButton *> &sig_selection_changed();

//! Implementation:
private:
	CL_RadioGroup(const CL_RadioGroup &copy) { return; } // disallow copy construction.
	CL_RadioGroup_Generic *impl;
};

#endif
