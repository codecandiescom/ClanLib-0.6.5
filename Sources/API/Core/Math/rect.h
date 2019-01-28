/*
	$Id: rect.h,v 1.23 2002/01/05 22:21:38 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

	File purpose:
		Rect class.

*/

//! clanCore="Math"
//! header=core.h

#ifndef header_rect
#define header_rect

//: Rect Class
class CL_Rect
{
//! Variables:
public:
	//: Left x coord
	int x1;
	//: Bottom y coord
	int y1;
	//: Right x coord
	int x2;
	//: Top y coord
	int y2;
	
public:
//! Construction:
	//: Rect Constructor
	//- nx1	- right constraint
	//- ny1 - bottom constraint
	//- nx2 - left constraint
	//- ny2 - top constraint
	CL_Rect(int nx1 = 0, int ny1 = 0, int nx2 = 0, int ny2 = 0) : x1(nx1), y1(ny1), x2(nx2), y2(ny2) { }
	
	//: Rect Copy Constructor
	//- rect - The rectangle to copy
	CL_Rect(const CL_Rect &rect)
	{
		this->x1 = rect.x1;
		this->x2 = rect.x2;
		this->y1 = rect.y1;
		this->y2 = rect.y2;
	};

//! Operations:
	//: Creates a rectangle around the given centre
	//- center_x - The x coord of the centre
	//- center_y - the y coord of the centre
	//- width - The width of the rectangle
	//- height - The height of the rectangle
	//- Returns - The centred rectangle
	static CL_Rect center(int center_x, int center_y, int width, int height)
	{
		return CL_Rect(
			center_x - width/2,
			center_y - height/2,
			center_x + width/2,
			center_y + height/2);
	}

	//: Align the rectangle left
	//- x - the left edge of the rectangle
	//- y - the bottom of the rectangle
	//- width - the width of the rectangle
	//- height - the height of the rectangle
	//- centre_vert - centre the rectangle around the y coord
	//- Returns - Left aligned rectangle 
	static CL_Rect left(int x, int y, int width, int height, bool center_vert = true)
	{
		return CL_Rect(
			x - width,
			center_vert ? (y - height/2) : y,
			x,
			center_vert ? (y + height/2) : y + height);
	}

	//: Align the rectangle right
	//- x - the right edge of the rectangle
	//- y - the bottom of the rectangle
	//- width - the width of the rectangle
	//- height - the height of the rectangle
	//- centre_vert - centre the rectangle around the y coord
	//- Returns - Right aligned rectangle 
	static CL_Rect right(int x, int y, int width, int height, bool center_vert = true)
	{
		return CL_Rect(
			x,
			center_vert ? (y - height/2) : y,
			x + width,
			center_vert ? (y + height/2) : y + height);
	}

	//: Move the rectangle
	//- delta_x - Distance to move rectangle on x-axis
	//- delta_y - Distance to move rectangle on y-axis
	void move(int delta_x, int delta_y)
	{
		x1 += delta_x;
		x2 += delta_x;
		y1 += delta_y;
		y2 += delta_y;
	}

	//: Normalize rectangle. Ensures that x1<x2 and y1<y2.
	void normalize()
	{
		if (x1 > x2)
		{
			int temp = x2;
			x2 = x1;
			x1 = temp;
		}

		if (y1 > y2)
		{
			int temp = y2;
			y2 = y1;
			y1 = temp;
		}
	}

//! Attributes:
	//: Determine if a point is inside the rectangle
	//- x - the x coord of the point
	//- y - the y coord of the point
	//- Returns - True if the point is in the rectangle or on it's border.
	bool inside(int x, int y) const
	{
		return x >= x1 && y >= y1 && x < x2 && y < y2;
	}

	//: Get Width
	//- Returns - Width of rectangle
	int get_width() const { return x2 - x1; };

	//: Get Height
	//- Returns - Height of rectangle
	int get_height() const { return y2 - y1; };

	//: Sets all rectangle dimensions/points to 0.
	void clear() { x1 = y1 = x2 = y2 = 0; };

//! Operators:
	//: Equivalancy operator
	bool operator == (const CL_Rect &rect) const;
};

#endif
