/*
	$Id: label_default.cpp,v 1.25 2002/01/16 19:02:12 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/Display/Font/font.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/GUI/component_style.h"
#include "label_default.h"

CL_Label_Default::CL_Label_Default(
	CL_Label *_label,
	CL_StyleManager_Default *style)
: CL_ComponentStyle(_label), label(_label)
{
	resources = style->get_resources();
	font = CL_Font::load("Label/font", resources);
	
	slot_paint = label->sig_paint().connect(
		this, &CL_Label_Default::on_paint);
	slot_get_preferred_size = label->sig_get_preferred_size().connect(
		this, &CL_Label_Default::on_get_preferred_size);
}

CL_Label_Default::~CL_Label_Default()
{
	delete font;
}

void CL_Label_Default::on_get_preferred_size(CL_Point &size)
{
	size.x = font->get_text_width(label->get_text());
	size.y = font->get_height();
}

void CL_Label_Default::on_paint()
{
	int x = 0;
	const char *text = label->get_text().c_str();

	if(label->get_alignment())
		x = (label->get_width() - font->get_text_width(text)) / 2;

	font->print_left(x, 0, text);
}
