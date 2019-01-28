/*
	$Id: vidmode.h,v 1.6 2001/11/07 02:59:18 plasmoid Exp $

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

#ifndef header_vidmode
#define header_vidmode

#include <string>
#include "../../Core/System/clanstring.h"

//: Video mode class. 
//- This class is used to describe a videomode in ClanLib.
//-
//- <p>CL_VidMode is returned from a display card when a list of video modes
//- are requested by a call to CL_DisplayCard::get_videomodes().</p>
//-
//- <p>Note that not all display implementations can give you a list of
//- supported modes. If none are requested, you can select whatever mode you
//- want. This usually happens when the implementation runs in a window and
//- that there's no such thing as a video mode.</p>
class CL_VidMode
{
public:
//! Variables:
	//: Videomode width.
	int width;

	//: Videomode height.
	int height;

	//: Videomode depth.
	int bpp;

	//: True if modeX.
	bool mode_x;

//! Construction:
	//: Class Constructor.
	//- width - Width of the videomode.
	//- height - Height of the videomode.
	//- bpp - Depth (8, 16, 24, 32 bpp) of the videomode.
	//- mode_x - Use mode x if available.
	CL_VidMode(int width, int height, int bpp, bool mode_x=false)
	{
		this->width = width;
		this->height = height;
		this->bpp = bpp;
		this->mode_x = mode_x;
	}

//! Operations:
	//: Converts the video mode description to a string.
	//- Returns - A string describing the video mode in human language!
	std::string asString()
	{
		CL_String ret;
		ret << width << " x " << height << " - " << bpp << " bits pr. pixel";
		if (mode_x == true) ret << " (modeX)";
		return ret.get_string();
	}
};

#endif
