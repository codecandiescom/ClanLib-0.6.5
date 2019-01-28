/*
	$Id: inputhat.h,v 1.5 2001/09/22 15:52:12 plasmoid Exp $

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

#ifndef header_inputhat
#define header_inputhat

//: Joystick hat interface.
//- CL_InputHat is the interface to the "hat" on a joystick.
class CL_InputHat
{
public:
//! Construction:
	//: Input Hat Destructor
	virtual ~CL_InputHat() { return; }

//! Attributes:
	//: <p>Returns the current direction of the hat (in degrees from 0-360). If the 
	//: value returned is -1, the hat is centered.</p>
	//- Returns - Current hat direction (degrees 0-360) or -1 if centered.
	virtual float get_direction()=0;
};

#endif
