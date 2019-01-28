/*
	$Id: point.h,v 1.9 2001/09/22 15:52:11 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

	File purpose:
		Point class.

*/

//! clanCore="Math"
//! header=core.h

#ifndef header_point
#define header_point

//: This defines a point.
//- It contains x,y
class CL_Point
{
public:
//! Construction:
	//: Create an empty point (x = y = 0)
	CL_Point() : x(0), y(0) { }

	//: Create a point
	CL_Point(int x1, int y1) : x(x1), y(y1) { }

//! Variables:
	//: The point
	int x, y;
};

#endif
