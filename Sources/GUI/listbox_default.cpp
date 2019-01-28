/*
	$Id: listbox_default.cpp,v 1.46 2002/03/01 19:36:55 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "listbox_default.h"
#include "API/Display/Font/font.h"
#include "API/GUI/scrollbar.h"
#include "API/GUI/listitem.h"
#include "API/Display/Display/display.h"

CL_ListBox_Default::CL_ListBox_Default(
	CL_ListBox *_listbox,
	CL_StyleManager_Default *style)
: CL_ComponentStyle(_listbox), listbox(_listbox)
{
	this->style = style;

	resources = style->get_resources();
	font = CL_Font::load("ListBox/font", resources);

	CL_Component *client_area = listbox->get_client_area();

//	int height = client_area->get_height();
//	int max = height / font->get_height();

	int font_height = font->get_height();
	listbox->set_item_height(font_height);
//	listbox->set_max_visible_items(max);

/*	int height;
	if (options.exists("height"))
	{
		height = options.get_value_as_int("height");
		int max = height / font->get_height();
		listbox->set_max_visible_items(max);
	}
	else
	{
		height = listbox->get_max_visible_items() * font->get_height();
	}
*/
	slot_paint_background = listbox->sig_paint().connect(
		this, &CL_ListBox_Default::on_paint_background);
	slot_paint_listbox = client_area->sig_paint().connect(
		this, &CL_ListBox_Default::on_paint_listbox);
}

CL_ListBox_Default::~CL_ListBox_Default()
{
	delete font;
}

void CL_ListBox_Default::on_paint_background()
{
	int width = listbox->get_width();
	int height = listbox->get_height();

	bool focus = listbox->has_child(listbox->get_focus());

	if(listbox->is_enabled() == false || focus == false)
	{
		// Fill
		style->fill_rect(1, 1, width - 1, height - 1, GUICOLOR_WHITE);

		// Outline
		style->draw_rect(0, 0, width, height, GUICOLOR_DARK_OUTLINE);
	}
	else
	{
		// Fill
		style->fill_rect(2, 2, width - 2, height - 2, GUICOLOR_WHITE);

		// Outline
		style->draw_rect(0, 0, width, height, GUICOLOR_DARK_OUTLINE);

		// Shade box
		style->draw_box(1, 1, width - 1, height - 1, GUICOLOR_DARKER_SHADE, GUICOLOR_MEDIUM_SHADE);
	}
}

void CL_ListBox_Default::on_paint_listbox()
{
	int width = listbox->get_client_area()->get_width();
//	int height = listbox->get_client_area()->get_height();

	int pos = 0;

	bool focus = listbox->has_child(listbox->get_focus());

	std::vector<CL_ListItem> items = listbox->get_items();
	std::vector<CL_ListItem>::iterator it;
	int offset = listbox->get_top_item();
	for (it = items.begin(); it != items.end(); it++)
	{
		if(pos < listbox->get_top_item())
		{
			pos++;
			continue;
		}

		if((*it).selected)
//		if(listbox->is_selected(pos))
		{
//			int text_width = font->get_text_width((*it).c_str());
		
			if(focus)
			{
				style->fill_rect(
					0,
					(pos - offset) * font->get_height(),
					width,
					(pos + 1 - offset) * font->get_height(),
					GUICOLOR_SELECTION);
			}
			else
			{
				style->draw_rect(
					0,
					(pos - offset) * font->get_height(),
					width,
					(pos + 1 - offset) * font->get_height(),
					GUICOLOR_SELECTION);
			}
		}

		font->print_left(
			0,
			(pos - offset) * font->get_height(),
			(*it).str.c_str());

		pos++;
	}
}
