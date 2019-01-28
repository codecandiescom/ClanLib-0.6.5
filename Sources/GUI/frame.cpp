/*
	$Id: frame.cpp,v 1.27 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/GUI/frame.h"
#include "API/GUI/component_options.h"
#include "API/GUI/stylemanager.h"
#include "frame_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Frame::CL_Frame(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	impl = new CL_Frame_Generic(this);
	get_style_manager()->connect_styles("frame", this);
	find_preferred_size();
}

CL_Frame::CL_Frame(
	const CL_Rect &pos,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(NULL)
{
	impl = new CL_Frame_Generic(this);
	get_style_manager()->connect_styles("frame", this);
}

CL_Frame::~CL_Frame()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

bool CL_Frame::is_fill_enabled() const
{
	return impl->fill;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_Frame::enable_fill(bool enabled)
{
	impl->fill = enabled;
}

