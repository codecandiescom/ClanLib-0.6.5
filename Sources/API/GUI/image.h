/*
	$Id: image.h,v 1.11 2002/02/04 15:21:34 sphair Exp $

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

#ifndef header_image
#define header_image

#include "component.h"

class CL_Surface;
class CL_Image_Generic;

//: Image component
class CL_Image : public CL_Component
{
//! Enums:
public:
	//: Mode enums
	enum Mode
	{
		normal,
		center,
		stretch,
		tile
	};

//! Construction:
public:
	//: Image Constructor
	CL_Image(
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Image Constructor
	CL_Image(
		CL_Surface *surface,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Image Constructor
	CL_Image(
		const CL_Rect &pos,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Image Constructor
	CL_Image(
		const CL_Rect &pos,
		CL_Surface *surface,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Image Destructor
	virtual ~CL_Image();

//! Attributes:
public:
	//: Returns the surface.
	CL_Surface *get_surface() const;

	//: Returns the current display mode.
	Mode get_mode() const;

//! Operations:
public:
	//: Sets the surface.
	void set_surface(CL_Surface *surface);

	//: Sets the display mode.
	void set_mode(Mode mode);

//! Implementation:
private:
	Mode mode;

	CL_Image(const CL_Image &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.
	CL_Image_Generic *impl;
};  

#endif
