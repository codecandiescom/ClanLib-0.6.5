/*
	$Id: radiobutton.h,v 1.5 2002/03/01 13:05:57 sphair Exp $

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

#ifndef header_radiobutton
#define header_radiobutton

#include "button.h"

//: CheckBox component
class CL_RadioButton : public CL_Button
{
//! Construction:
public:
	//: RadioButton constructor
	CL_RadioButton(
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: RadioButton constructor
	CL_RadioButton(
		const CL_Point &pos,
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: RadioButton constructor
	CL_RadioButton(
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: RadioButton destructor
	virtual ~CL_RadioButton();

//! Attributes:
public:
	//: Returns true if the radiobutton is checked, or false if it is not checked.
	bool is_checked() const;

//! Operations:
public:
	//: Checks the radiobutton if check is true, or unchecks it if check is false. 
	void set_checked(bool check);

//! Implementation:
private:
	CL_RadioButton(const CL_RadioButton &copy) : CL_Button(NULL, NULL) { return; } // disallow copy construction.
};  

#endif
