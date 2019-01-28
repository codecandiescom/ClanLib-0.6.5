/*
	$Id: radiobutton_default.cpp,v 1.2 2002/03/18 13:12:25 sphair Exp $
	
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
#include "radiobutton_default.h"

CL_RadioButton_Default::CL_RadioButton_Default(
	CL_RadioButton *radiobutton,
	CL_StyleManager_Default *style)
: CL_ComponentStyle(radiobutton), radiobutton(radiobutton), style(style)
{
	resources = style->get_resources();

	sur_checked = CL_Surface::load("RadioButton/sur_checked", resources);
	sur_unchecked = CL_Surface::load("RadioButton/sur_unchecked", resources);
	sur_checked_disabled = CL_Surface::load("RadioButton/sur_checked_disabled", resources);
	sur_unchecked_disabled = CL_Surface::load("RadioButton/sur_unchecked_disabled", resources);
	font = CL_Font::load("RadioButton/font", resources);

	slot_paint = radiobutton->sig_paint().connect_virtual(
		this, &CL_RadioButton_Default::on_paint);

	slot_get_preferred_size = radiobutton->sig_get_preferred_size().connect(
		this, &CL_RadioButton_Default::on_get_preferred_size);
}

CL_RadioButton_Default::~CL_RadioButton_Default()
{
	delete font;
	delete sur_checked;
	delete sur_unchecked;
	delete sur_checked_disabled;
	delete sur_unchecked_disabled;
}

void CL_RadioButton_Default::on_get_preferred_size(CL_Point &size)
{
	CL_Surface *s = sur_checked;
	size.x = s->get_width() + font->get_text_width(radiobutton->get_text()) + 8;
	size.y = font->get_height() + 4;
	if(size.y < (int)s->get_height())
		size.y = s->get_height();
}

void CL_RadioButton_Default::on_paint(CL_SlotParent_v0 &super)
{
	CL_Surface *show_surface;

	if (radiobutton->is_enabled())
	{
		if (radiobutton->is_checked())
			show_surface = sur_checked;
		else
			show_surface = sur_unchecked;

		// Focus
		if(radiobutton->has_focus())
			style->fill_rect(0, 0, radiobutton->get_width(), show_surface->get_height(), GUICOLOR_FOCUS);
	}
	else
	{
		if (radiobutton->is_checked())
			show_surface = sur_checked_disabled;
		else
			show_surface = sur_checked_disabled;
	}

	show_surface->put_screen(0, 0);
	if (radiobutton->get_text().length() > 0)
		font->print_left(4 + show_surface->get_width(), 0, radiobutton->get_text().c_str());
}
