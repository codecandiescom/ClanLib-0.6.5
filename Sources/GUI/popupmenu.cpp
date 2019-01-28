/*
	$Id: popupmenu.cpp,v 1.29 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"

#ifdef WIN32
#pragma warning (disable:4355)
#endif

#include "API/GUI/stylemanager.h"
#include "popupmenu_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_PopupMenu::CL_PopupMenu(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), CL_MenuNode(this), impl(NULL)
{
	impl = new CL_PopupMenu_Generic(this);
	get_style_manager()->connect_styles("popupmenu", this);
}

CL_PopupMenu::CL_PopupMenu(
	const CL_Rect &pos,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), CL_MenuNode(this), impl(NULL)
{
	impl = new CL_PopupMenu_Generic(this);
	get_style_manager()->connect_styles("popupmenu", this);
}

CL_PopupMenu::~CL_PopupMenu()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

CL_Component *CL_PopupMenu::get_client_area() const
{
	return impl->client_area;
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v1<const CL_MenuNode &> &CL_PopupMenu::sig_selection_changed()
{
	return impl->sig_selection_changed;
}

CL_Signal_v2<CL_MenuNode *, CL_Point &> &CL_PopupMenu::sig_paint_node()
{
	return impl->sig_paint_node;
}

CL_Signal_v0 &CL_PopupMenu::sig_cancelled()
{
	return impl->sig_cancelled;
}

