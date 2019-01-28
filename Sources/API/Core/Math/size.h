/*
	$Id: size.h,v 1.11 2001/10/23 07:32:35 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanCore="Math"
//! header=core.h

#ifndef header_size
#define header_size

//: This defines a size
//- It contains width, height
class CL_Size
{
public:
//! Construction:
	//: Create an empty size (width = height = 0)
	CL_Size() : width(0), height(0) { }

	//: Create a size, setting the width and height
	//- w = Width
	//- h = Height
	CL_Size(int w, int h) : width(w), height(h) { }

//! Variables:
	//: The width
	int width;

	//: The height
	int height;
};

#endif
