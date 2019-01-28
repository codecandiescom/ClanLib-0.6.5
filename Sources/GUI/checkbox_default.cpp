/*
	$Id: checkbox_default.cpp,v 1.29 2002/03/18 13:12:25 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include <cstdio>
#include "API/Display/Display/surface.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Display/Font/font.h"
#include "checkbox_default.h"

CL_CheckBox_Default::CL_CheckBox_Default(
	CL_CheckBox *checkbox,
	CL_StyleManager_Default *style)
: CL_ComponentStyle(checkbox), checkbox(checkbox), style(style)
{
	resources = style->get_resources();

	sur_checked = CL_Surface::load("CheckBox/sur_checked", resources);
	sur_unchecked = CL_Surface::load("CheckBox/sur_unchecked", resources);
	sur_checked_disabled = CL_Surface::load("CheckBox/sur_checked_disabled", resources);
	sur_unchecked_disabled = CL_Surface::load("CheckBox/sur_unchecked_disabled", resources);
	font = CL_Font::load("CheckBox/font", resources);

	slot_paint = checkbox->sig_paint().connect_virtual(
		this, &CL_CheckBox_Default::on_paint);

	slot_get_preferred_size = checkbox->sig_get_preferred_size().connect(
		this, &CL_CheckBox_Default::on_get_preferred_size);
}

CL_CheckBox_Default::~CL_CheckBox_Default()
{
	delete font;
	delete sur_checked;
	delete sur_unchecked;
	delete sur_checked_disabled;
	delete sur_unchecked_disabled;
}

void CL_CheckBox_Default::on_get_preferred_size(CL_Point &size)
{
	CL_Surface *s = sur_checked;
	size.x = s->get_width() + font->get_text_width(checkbox->get_text()) + 8;
	size.y = font->get_height() + 4;
	if(size.y < (int)s->get_height())
		size.y = s->get_height();
}

void CL_CheckBox_Default::on_paint(CL_SlotParent_v0 &super)
{
	CL_Surface *show_surface;

	if (checkbox->is_enabled())
	{
		if (checkbox->is_checked())
			show_surface = sur_checked;
		else
			show_surface = sur_unchecked;

		// Focus
		if(checkbox->has_focus())
			style->fill_rect(0, 0, checkbox->get_width(), show_surface->get_height(), GUICOLOR_FOCUS);
	}
	else
	{
		if (checkbox->is_checked())
			show_surface = sur_checked_disabled;
		else
			show_surface = sur_unchecked_disabled;
	}

	show_surface->put_screen(0, 0);
	if (checkbox->get_text().length() > 0)
		font->print_left(4 + show_surface->get_width(), 0, checkbox->get_text().c_str());
}
