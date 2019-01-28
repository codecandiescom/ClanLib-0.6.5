/*
	$Id: component_resize_handler.h,v 1.8 2001/10/10 12:17:10 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanGUI="Framework"
//! header=gui.h

#ifndef header_component_resize_handler
#define header_component_resize_handler

class CL_Rect;
class CL_Component;
class CL_ComponentResizeHandler_Generic;

//: Component Resize Handler
class CL_ComponentResizeHandler
{
public:
//! Construction:
	//: Component Resize Handler Constructor
	CL_ComponentResizeHandler(const CL_Rect &resize_area, CL_Component *owner, class CL_StyleManager *style);

	//: Component Resize Handler Destructor
	virtual ~CL_ComponentResizeHandler();

//! Implementation:
private:
	CL_ComponentResizeHandler_Generic *impl;
};

#endif
