/*
	$Id: inputaxis_basic.h,v 1.6 2002/06/13 14:43:17 mbn Exp $

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

#ifndef header_inputaxis_basic
#define header_inputaxis_basic

#include "inputaxis.h"

//: CL_InputAxis interface used to ease manual implementation of an input axis.
class CL_InputAxis_Basic : public CL_InputAxis
{
protected:
	float pos;
	
public:
//! Construction:
	//: Constructs an axis standing in center position.
	CL_InputAxis_Basic()
	{
		pos = 0;
	}
	
	//: Constructs an axis standing in position 'pos'.
	//- pos - Initial axis position. Range -1 to 1.
	CL_InputAxis_Basic(float pos)
	{
		this->pos = pos;
	}

	//: Input Axis Basic Destructor
	virtual ~CL_InputAxis_Basic() {;}

//! Attributes:
	//: Returns the current axis position.
	//- Returns - The current axis position.
	virtual float get_pos()
	{
		return pos;
	}
	
//! Operations:
	//: Set the current axis position.
	//- pos - New axis position. Range -1 to 1.
	void set_pos(float pos)
	{
		this->pos = pos;
	}
};

#endif
