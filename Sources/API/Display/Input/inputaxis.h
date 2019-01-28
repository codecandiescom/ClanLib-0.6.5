/*
	$Id: inputaxis.h,v 1.5 2001/09/22 15:52:12 plasmoid Exp $

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

#ifndef header_inputaxis
#define header_inputaxis

//: Input Axis interface.
//- CL_InputAxis is the interface to an input axis, such as a joystick axis.
class CL_InputAxis
{
public:
//! Construction:
	//: Input Axis destructor
	virtual ~CL_InputAxis() { return; }

//! Attributes:
	//: <p>Returns a value between -1 and 1. Zero means centered, and -1 and 1 
	//: means all-the-way to either side (up/down, left/right, whatever).</p>
	//- Returns - Current axis position.
	virtual float get_pos()=0;
};

#endif
