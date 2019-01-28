/*
	$Id: layout_manager.cpp,v 1.5 2002/01/16 19:06:54 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/GUI/layout_manager.h"
#include "layout_manager_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_LayoutManager::CL_LayoutManager()
{
	impl = new CL_LayoutManager_Generic(this);
}

CL_LayoutManager::~CL_LayoutManager()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_LayoutManager::add_resize_position(
	CL_Component *destination, ELayoutPosition destination_position,
	CL_Component *source, ELayoutPosition source_position)
{
	impl->add_resize_position(
		destination, destination_position,
		source, source_position);
}
