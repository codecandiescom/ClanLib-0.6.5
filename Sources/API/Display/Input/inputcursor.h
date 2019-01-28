/*
	$Id: inputcursor.h,v 1.6 2002/05/17 08:47:58 grumbel Exp $

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

#ifndef header_inputcursor
#define header_inputcursor

//: Input cursor interface.
//- CL_InputCursor is the interface to cursors, such as a mouse cursor.
class CL_InputCursor
{
public:
//! Construction:
	//: Input Cursor Destructor
	virtual ~CL_InputCursor() { return; }

//! Attributes:
	//: Returns current x-coordinate.
	//- Returns - Current x position.
	virtual float get_x()=0;

	//: Returns current y-coordinate.
	//- Returns - Current y position.
	virtual float get_y()=0;

	//: Returns the range of the x-coordinate.
	//- Returns - Max x coordinate value.
	virtual float get_max_x()=0;
	
	//: Returns the range of the y-coordinate.
	//- Returns - Max y coordinate value.
	virtual float get_max_y()=0;

	//: Set the cursor to the position given by x and y
	virtual void set_position (float x, float y)=0;

};

#endif
