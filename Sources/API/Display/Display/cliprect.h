/*
	$Id: cliprect.h,v 1.5 2001/09/22 15:52:11 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanDisplay="Display 2D"
//! header=display.h

#ifndef header_cliprect
#define header_cliprect

//: Clipping rectangle class in ClanLib.
//- <p>This class is used to define clipping regions when drawing graphics to the
//- backbufffer. This is useful if it is too expensive to update the entire
//- screen each time - instead you can define a limited area where everything
//- outside the rectangle should be discarded.</p>
//- 
//- <p>The clipping rectangle is used together with the clipping rect functions
//- in CL_Display and CL_DisplayCard.</p>
//- See also: CL_Display - The DisplayCard wrapper class.
//- See also: CL_DisplayCard - The DisplayCard class.
class CL_ClipRect
{
public:
//! Variables:
	//: Min x-coordinate of the rectangle.
	int m_x1;

	//: Min y-coordinate of the rectangle.
	int m_y1;

	//: Max x-coordinate of the rectangle - NOT included in clipping area
	int m_x2;

	//: Max y-coordinate of the rectangle - NOT included in clipping area
	int m_y2;

//! Construction:
	//: Constructs an uninitialized clip rectangle.
	//: <p>(x1, y1) and (x2, y2) contain random values, and should be manually
	//: initialized before usage of the clip rect.</p>
	CL_ClipRect();

	//: Copy constructor.
	CL_ClipRect(const CL_ClipRect &rect);

	//: Constructs a clipping rectangle from (x1,y1) to (x2,y2).
	//: (x1,y1) - Upper left corner of the rectangle.
	//: (x2,y2) - Lower right corner of the rectangle (not included)
	CL_ClipRect(int x1, int y1, int x2, int y2);

//! Attributes:
	//: Tests if the specified rectangle needs to be clipped with this clip rect.
	//- rect - The rectangle to be tested.
	//- Returns - True if the passed rectangle needs to be clipped.
	bool test_clipped(const CL_ClipRect &rect) const;

	//: Tests whether the specified rectangle is entirely contained within this clip rect.
	//- rect - The rectangle to be tested.
	//- Returns - True if the passed rectangle is contained within this rectangle.
	bool test_unclipped(const CL_ClipRect &rect) const;

	//: Tests whether all of the specified rectangle is outside this rectangle.
	//- rect - The rectangle to be tested.
	//- Returns - True if the entire specified rect is outside this rectangle.
	bool test_all_clipped(const CL_ClipRect &rect) const;

//! Operations:
	//: Clips the given rectangle and returns the result.
	//- rect - The rectangle to be clipped.
	//- Returns - The clipped rectangle.
	CL_ClipRect clip(const CL_ClipRect &rect) const;
	
//! Operators:
	//: Standard C++ == operator.
	//- Returns - True if specified rectangle equals this rectangle.
	bool operator == (const CL_ClipRect &rect) const;

};

#endif
