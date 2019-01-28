/*
	$Id: layout_manager.h,v 1.9 2001/10/10 12:17:10 sphair Exp $

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

#ifndef header_layout_manager
#define header_layout_manager

class CL_Component;
class CL_LayoutManager_Generic;

//: Layout components.
class CL_LayoutManager
{
//! Enums:
public:
	//: Layout Position Enums
	enum ELayoutPosition
	{
		x1 = 0,
		y1 = 1,
		x2 = 2,
		y2 = 3
	};

//! Construction:
public:
	//: Layout Manager Constructor
	CL_LayoutManager();

	//: Layout Manager Destructor
	virtual ~CL_LayoutManager();

//! Attributes:
public:

//! Operations:
public:
	void add_resize_position(
		CL_Component *destination, ELayoutPosition destination_position,
		CL_Component *source, ELayoutPosition source_position);

//! Implementation:
private:
	CL_LayoutManager_Generic *impl;
};

#endif
