/*
	$Id: treeview.cpp,v 1.16 2002/03/01 22:19:09 sphair Exp $
	
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
#include "treeview_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_TreeView::CL_TreeView(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), CL_TreeNode(this), impl(NULL)
{
	impl = new CL_TreeView_Generic(this);
	get_style_manager()->connect_styles("treeview", this);
	find_preferred_size();
}

CL_TreeView::CL_TreeView(
	const CL_Rect &pos,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), CL_TreeNode(this), impl(NULL)
{
	impl = new CL_TreeView_Generic(this);
	get_style_manager()->connect_styles("treeview", this);
}

CL_TreeView::~CL_TreeView()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

CL_Component *CL_TreeView::get_client_area() const
{
	return impl->client_area;
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v1<const CL_TreeNode &> &CL_TreeView::sig_selection_changed()
{
	return impl->sig_selection_changed;
}

CL_Signal_v2<CL_TreeNode *, CL_Point &> &CL_TreeView::sig_paint_node()
{
	return impl->sig_paint_node;
}

