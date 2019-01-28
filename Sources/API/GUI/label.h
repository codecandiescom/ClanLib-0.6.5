/*
	$Id: label.h,v 1.37 2002/02/04 15:21:34 sphair Exp $

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

#ifndef header_label
#define header_label

#include "component.h"

class CL_Label_Generic;

//: Label component
class CL_Label : public CL_Component
{
//! Construction:
public:
	//: Label Constructor
	CL_Label(
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Label Constructor
	CL_Label(
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Label Constructor
	CL_Label(
		const CL_Point &pos,
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Label Constructor
	CL_Label(
		const CL_Rect &pos,
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Label Destructor
	virtual ~CL_Label();

//! Attributes:
public:
	//: Returns the labeltext.
	const std::string &get_text() const;

	//: Returns the alignment setting.
	//: See set_alignment() for more info.
	int get_alignment() const;

//! Operations:
public:
	//: Sets the labeltext.
	void set_text(const std::string &text);

	//: Sets the label contents to a plain text containing the printed value of num.
	void set_text(int number);

	//: Sets the label contents to a plain text containing the printed value of num.
	void set_text(double number);

	//: Clears any label contents. Equivalent with setText("").
	void clear();

	//: Sets the alignment of the label contents
	void set_alignment(int alignment);

//! Implementation:
private:
	void init(const std::string &text);

	CL_Label(const CL_Label &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.
	CL_Label_Generic *impl;
};  

#endif
