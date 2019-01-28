/*
	$Id: checkbox.cpp,v 1.23 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "../API/GUI/checkbox.h"
#include "../API/GUI/component.h"
#include "../API/GUI/component_options.h"
#include "../API/GUI/stylemanager.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_CheckBox::CL_CheckBox(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Button(parent, style)
{
	set_toggle_mode();
	get_style_manager()->connect_styles("checkbox", this);
	find_preferred_size();
}

CL_CheckBox::CL_CheckBox(
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Button(text, parent, style)
{
	set_toggle_mode();
	get_style_manager()->connect_styles("checkbox", this);
	find_preferred_size();
}

CL_CheckBox::CL_CheckBox(
	const CL_Point &pos,
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Button(pos, text, parent, style)
{
	set_toggle_mode();
	get_style_manager()->connect_styles("checkbox", this);
	find_preferred_size();
}

CL_CheckBox::~CL_CheckBox()
{
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

bool CL_CheckBox::is_checked() const
{
	return is_down();
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_CheckBox::set_checked(bool check)
{
	set_down(check);
}
