/*
	$Id: frame.h,v 1.26 2001/12/27 23:23:34 sphair Exp $

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

#ifndef header_frame
#define header_frame

#include "component.h"

class CL_Frame_Generic;

//: Frame component
class CL_Frame : public CL_Component
{
//! Construction:
public:
	//: Frame Constructor
	CL_Frame(
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Frame Constructor
	CL_Frame(
		const CL_Rect &pos,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Frame Destructor
	virtual ~CL_Frame();

//! Attributes:
public:
	//: Is fill enabled
	bool is_fill_enabled() const;

//! Operations:
public:
	//: Enable Fill
	void enable_fill(bool enabled = true);

//! Implementation:
private:
	CL_Frame(const CL_Frame &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.
	CL_Frame_Generic *impl;
};  

#endif
