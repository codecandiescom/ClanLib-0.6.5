/*
	$Id: radiobutton.cpp,v 1.7 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "../API/GUI/radiobutton.h"
#include "../API/GUI/component.h"
#include "../API/GUI/component_options.h"
#include "../API/GUI/stylemanager.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_RadioButton::CL_RadioButton(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Button(parent, style)
{
	set_toggle_mode();
	get_style_manager()->connect_styles("radiobutton", this);
	find_preferred_size();
}

CL_RadioButton::CL_RadioButton(
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Button(text, parent, style)
{
	set_toggle_mode();
	get_style_manager()->connect_styles("radiobutton", this);
	find_preferred_size();
}

CL_RadioButton::CL_RadioButton(
	const CL_Point &pos,
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Button(pos, text, parent, style)
{
	set_toggle_mode();
	get_style_manager()->connect_styles("radiobutton", this);
	find_preferred_size();
}

CL_RadioButton::~CL_RadioButton()
{
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

bool CL_RadioButton::is_checked() const
{
	return is_down();
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_RadioButton::set_checked(bool check)
{
	set_down(check);
}
