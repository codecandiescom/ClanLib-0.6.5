/*
	$Id: button_default.cpp,v 1.50 2002/01/17 14:38:44 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include <cstdio>
#include "API/Display/Display/surface.h"
#include "API/Display/Font/font.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/GUI/stylemanager_default.h"
#include "button_default.h"

CL_Button_Default::CL_Button_Default(
	CL_Button *_button,
	CL_StyleManager_Default *_style)
:
	CL_ComponentStyle(_button),
	button(_button),
	style(_style)
{
	resources = style->get_resources();

	font = CL_Font::load("Button/font", resources);
	font_disabled = CL_Font::load("Button/font_disabled", resources);

	slot_set_options = button->sig_set_options().connect(
		this, &CL_Button_Default::on_set_options);
	slot_paint = button->sig_paint().connect(
		this, &CL_Button_Default::on_paint);
	slot_get_preferred_size = button->sig_get_preferred_size().connect(
		this, &CL_Button_Default::on_get_preferred_size);
}

CL_Button_Default::~CL_Button_Default()
{
	delete font;
	delete font_disabled;
}

void CL_Button_Default::on_set_options(const CL_ComponentOptions &options)
{
	if(options.exists("surface_up"))
		button->set_surface_up(CL_Surface::load(options.get_value("surface_up").c_str(), resources));

	if(options.exists("surface_down"))
		button->set_surface_down(CL_Surface::load(options.get_value("surface_down").c_str(), resources));

	if(options.exists("surface_disabled"))
		button->set_surface_disabled(CL_Surface::load(options.get_value("surface_disabled").c_str(), resources));

	if(options.exists("surface_highlighted"))
		button->set_surface_highlighted(CL_Surface::load(options.get_value("surface_highlighted").c_str(), resources));
}

void CL_Button_Default::on_get_preferred_size(CL_Point &size)
{
	CL_Surface *s = button->get_surface_up();
	if(s)
	{
		size.x = s->get_width();
		size.y = s->get_height();
	}
	else
	{
		size.x = font->get_text_width(button->get_text()) + 8;
		size.y = font->get_height() + 4;
	}
}

void CL_Button_Default::on_paint()
{
	int text_width = font->get_text_width(button->get_text().c_str());
	int button_width = button->get_width();
	int text_height = font->get_height();
	int button_height = button->get_height();

	int x_pos = (button_width - text_width) / 2;
	int y_pos = (button_height - text_height) / 2;

	int width = button->get_width();
	int height = button->get_height();

	if(button->is_enabled() == false)
	{
		CL_Surface *surface = button->get_surface_disabled();
		if(surface)
			surface->put_screen(0, 0);
		else
		{
			// Fill
			style->fill_rect(2, 2, width - 2, height - 2, GUICOLOR_BUTTON_DISABLED);

			// Outline
			style->draw_rect(0, 0, width, height, GUICOLOR_DARK_OUTLINE_DISABLED);

			// Shade box
			style->draw_box(1, 1, width - 1 , height - 1, GUICOLOR_BRIGHT_SHADE_DISABLED, GUICOLOR_DARK_SHADE_DISABLED);
		}

		font_disabled->print_left(x_pos, y_pos, button->get_text().c_str());
	}
	else 
	{
		if(button->is_down())
		{
			CL_Surface *surface = button->get_surface_down();
			if(surface)
				surface->put_screen(0, 0);
			else
			{
				// Fill
				style->fill_rect(2, 2, width - 2, height - 2, GUICOLOR_BUTTON_TOGGLED);

				// Outline
				style->draw_rect(0, 0, width, height, GUICOLOR_DARK_OUTLINE);

				// Shade box
				style->draw_box(1, 1, width - 1 , height - 1, GUICOLOR_DARK_SHADE, GUICOLOR_BRIGHT_SHADE);
			}

			font->print_left(x_pos + 1, y_pos + 1, button->get_text().c_str());
		}
		else
		{
			CL_Surface *surface = button->get_surface_up();
			CL_Surface *surface_highlighted = button->get_surface_highlighted();
			if(button->has_mouse_over() && surface_highlighted)
				surface_highlighted->put_screen(0, 0);
			else if(surface)
				surface->put_screen(0, 0);
			else
			{
				// Fill
				style->fill_rect(2, 2, width - 2, height - 2, GUICOLOR_BUTTON);

				// Outline
				style->draw_rect(0, 0, width, height, GUICOLOR_DARK_OUTLINE);

				// Shade box
				style->draw_box(1, 1, width - 1 , height - 1, GUICOLOR_BRIGHT_SHADE, GUICOLOR_DARK_SHADE);

				// Focus
				if(button->has_focus())
					style->draw_rect(2, 2, width - 2, height - 2, GUICOLOR_FOCUS);
			}

			font->print_left(x_pos, y_pos, button->get_text().c_str());
		}
	}
}
