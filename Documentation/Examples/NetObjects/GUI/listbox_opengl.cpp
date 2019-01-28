#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include "listbox_opengl.h"
#include "stylemanager_opengl.h"

CL_ListBox_OpenGL::CL_ListBox_OpenGL(
	CL_ListBox *_listbox,
	CL_StyleManager_OpenGL *style)
:
	CL_ComponentStyle(_listbox),
	listbox(_listbox),
	style(style),
	resources(style->get_resources())
{
	font = CL_Font::load("ListBox/font", resources);

	CL_Component *client_area = listbox->get_client_area();

	int font_height = font->get_height();
	listbox->set_item_height(font_height);

	slot_paint_background = listbox->sig_paint().connect(
		this, &CL_ListBox_OpenGL::on_paint_background);
	slot_paint_listbox = client_area->sig_paint().connect(
		this, &CL_ListBox_OpenGL::on_paint_listbox);
}

CL_ListBox_OpenGL::~CL_ListBox_OpenGL()
{
//	resources->get_resource("Listbox/font")->unload();
}

void CL_ListBox_OpenGL::on_paint_background()
{
	int width = listbox->get_width();
	int height = listbox->get_height();

	// Outline
	CL_Display::draw_rect(0, 0, width, height, 0.5f, 0.5f, 0.5f);

	// Fill
	CL_Display::fill_rect(1, 1, width - 1, height - 1, 0.0f, 0.0f, 0.0f);
}

void CL_ListBox_OpenGL::on_paint_listbox()
{
	int width = listbox->get_client_area()->get_width();
	int height = listbox->get_client_area()->get_height();

	int pos = 0;
	int offset = listbox->get_top_item();

	std::vector<CL_ListItem> items = listbox->get_items();
	std::vector<CL_ListItem>::iterator it;
	for (it = items.begin(); it != items.end(); it++)
	{
		if (pos < offset)
		{
			pos++;
			continue;
		}

		if (pos == listbox->get_current_item())
		{
			CL_Display::fill_rect(
				0, (pos - offset) * font->get_height(),
				width, (pos + 1 - offset) * font->get_height(),
				0.1f, 0.1f, 0.1f);
		}

		font->print_left(
			0,
			(pos - offset) * font->get_height(),
			(*it).str.c_str());

		pos++;

		if((pos - offset + 1) * font->get_height() >= height)
			break;
	}
}
