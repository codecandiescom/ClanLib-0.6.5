/*
	$Id: checkbox.h,v 1.28 2002/03/01 13:05:57 sphair Exp $

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

#ifndef header_checkbox
#define header_checkbox

#include "button.h"

//: CheckBox component
class CL_CheckBox : public CL_Button
{
//! Construction:
public:
	//: CheckBox constructor
	CL_CheckBox(
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: CheckBox constructor
	CL_CheckBox(
		const CL_Point &pos,
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: CheckBox constructor
	CL_CheckBox(
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: CheckBox destructor
	virtual ~CL_CheckBox();

//! Attributes:
public:
	//: Returns true if the checkbox is checked, or false if it is not checked.
	bool is_checked() const;

//! Operations:
public:
	//: Checks the checkbox if check is true, or unchecks it if check is false. 
	void set_checked(bool check);

//! Implementation:
private:
	CL_CheckBox(const CL_CheckBox &copy) : CL_Button(NULL, NULL) { return; } // disallow copy construction.
};  

#endif
