/*
	$Id: component_move_handler.h,v 1.13 2001/10/10 12:17:10 sphair Exp $

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

#ifndef header_component_move_handler
#define header_component_move_handler

class CL_Rect;
class CL_Component;
class CL_ComponentMoveHandler_Generic;

//: Component Mode Handler Class
class CL_ComponentMoveHandler
{
public:
//! Construction:
	//: Component Mode Handler Constructor
	CL_ComponentMoveHandler(const CL_Rect &move_area, CL_Component *owner, class CL_StyleManager *style);

	//: Component Mode Handler Destructor
	virtual ~CL_ComponentMoveHandler();

//! Implementation:
private:
	CL_ComponentMoveHandler_Generic *impl;
};

#endif
