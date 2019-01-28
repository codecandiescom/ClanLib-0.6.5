/*
	$Id: label.cpp,v 1.44 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/GUI/label.h"
#include "API/GUI/component_options.h"
#include "API/GUI/stylemanager.h"
#include "label_generic.h"
#include "component_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Label::CL_Label(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	init("");
	find_preferred_size();
}

CL_Label::CL_Label(
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	init(text);
	find_preferred_size();
}

CL_Label::CL_Label(
	const CL_Point &pos,
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(CL_Rect(pos.x, pos.y), parent, style), impl(NULL)
{
	init(text);
	find_preferred_size();
}

CL_Label::CL_Label(
	const CL_Rect &pos,
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(NULL)
{
	init(text);
}

void CL_Label::init(const std::string &text)
{
	impl = new CL_Label_Generic(this, text);
	get_style_manager()->connect_styles("label", this);
}

CL_Label::~CL_Label()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

const std::string &CL_Label::get_text() const
{
	return impl->text;
}

int CL_Label::get_alignment() const
{
	return impl->alignment;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_Label::set_text(const std::string &text)
{
	impl->text = text;
}

void CL_Label::set_text(int number)
{
	char buf[20];
	sprintf(buf, "%d", number);
	impl->text = buf;
}

void CL_Label::set_text(double number)
{
	char buf[20];
	sprintf(buf, "%f", number);
	impl->text = buf;
}

void CL_Label::clear()
{
	impl->text = "";
}

void CL_Label::set_alignment(int alignment)
{
	impl->alignment = alignment;
}
