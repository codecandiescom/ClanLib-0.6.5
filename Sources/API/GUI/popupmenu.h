/*
	$Id: popupmenu.h,v 1.30 2002/03/01 22:19:08 sphair Exp $

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

#ifndef header_popupmenu
#define header_popupmenu

#include "component.h"
#include "menunode.h"

class CL_PopupMenu_Generic;

//: PopupMenu component
class CL_PopupMenu : public CL_Component, public CL_MenuNode
{
//! Construction:
public:
	//: PopupMenu Constructor
	CL_PopupMenu(
		CL_Component *parent,
		CL_StyleManager *style = NULL);
	
	//: PopupMenu Constructor
	CL_PopupMenu(
		const CL_Rect &pos,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: PopupMenu Destructor
	virtual ~CL_PopupMenu();

//! Attributes:
public:
	//: Returns the client area of the component.
	CL_Component *get_client_area() const;

//! Signals:
public:
	//: This signal is emitted when the selection set of a tree changes.
	CL_Signal_v1<const CL_MenuNode &> &sig_selection_changed();

	//: This signal is emitted when the popupmenu is cancelled.
	CL_Signal_v0 &sig_cancelled();

	//: Draws the node (used by themes).
	CL_Signal_v2<CL_MenuNode *, CL_Point &> &sig_paint_node();

//! Implementation:
private:
	CL_PopupMenu(const CL_PopupMenu &copy) : CL_Component(NULL, NULL), CL_MenuNode(NULL) { return; } // disallow copy construction.
	CL_PopupMenu_Generic *impl;
};

#endif
