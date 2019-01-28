/*
	$Id: window_default.cpp,v 1.31 2002/01/16 19:50:41 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "window_default.h"
#include "API/Display/Font/font.h"

#include "API/display.h"

CL_Window_Default::CL_Window_Default(
	CL_Window *_window,
	CL_StyleManager_Default *style) 
:
	CL_ComponentStyle(_window),
	window(_window),
	titlebar_height(21)
{
	this->style = style;
	
	resources = style->get_resources();
	font = CL_Font::load("Window/font", resources);
	font_disabled = CL_Font::load("Window/font_disabled", resources);
	
	// Make the window movable by dragging titlebar
	move_handler = new CL_ComponentMoveHandler(
		CL_Rect(0, 0, window->get_width(), titlebar_height),
		window,
		style);

	// Make the window resizable by dragging corner
/*	resize_handler = new CL_ComponentResizeHandler(
		CL_Rect(window->get_width() - 3, window->get_height() - 3, window->get_width(), window->get_height()),
		window,
		style);
*/
	// Create a close button on the titlebar
	CL_Rect rect(window->get_width() - 19, 3, window->get_width() - 3, 19);
	button_close = new CL_Button(rect, "X", window, style);

	slot_close = button_close->sig_clicked().connect(this, &CL_Window_Default::on_close);
	slot_paint = window->sig_paint().connect(this, &CL_Window_Default::on_paint);
	slot_resize = window->sig_resize().connect(this, &CL_Window_Default::on_resize);

	// Move close button on window resize
	layout.add_resize_position(button_close, CL_LayoutManager::x1, window, CL_LayoutManager::x2);
	layout.add_resize_position(button_close, CL_LayoutManager::x2, window, CL_LayoutManager::x2);

	// Resize client area on window resize
	CL_Component *client_area = window->get_client_area();

	layout.add_resize_position(client_area, CL_LayoutManager::x2, window, CL_LayoutManager::x2);
	layout.add_resize_position(client_area, CL_LayoutManager::y2, window, CL_LayoutManager::y2);
}

CL_Window_Default::~CL_Window_Default()
{
	delete font;
	delete font_disabled;
//	delete resize_handler;
	delete move_handler;
	delete button_close;
}

void CL_Window_Default::on_paint()
{
	int width = window->get_width();
	int height = window->get_height();
	
	style->draw_rect(0, 0, width, height, GUICOLOR_DARK_OUTLINE);

	style->draw_box(1, 1, width - 1, titlebar_height, GUICOLOR_BRIGHT_SHADE, GUICOLOR_DARK_SHADE);
	style->draw_box(1, titlebar_height, width - 1, height - 1, GUICOLOR_BRIGHT_SHADE, GUICOLOR_DARK_SHADE);

	if(window->is_enabled() == false)
		style->fill_rect(2, 2, width - 2, titlebar_height - 1, GUICOLOR_WINDOW_TITLEBAR_DISABLED);
	else
		style->fill_rect(2, 2, width - 2, titlebar_height - 1, GUICOLOR_WINDOW_TITLEBAR);
	style->fill_rect(2, titlebar_height + 1, width - 2, height - 2, GUICOLOR_WINDOW_NORMAL);

	int y = (titlebar_height + 2 - font->get_height()) / 2;

	if(window->is_enabled() == false)
		font_disabled->print_left(7, y, window->get_title().c_str());
	else
		font->print_left(7, y, window->get_title().c_str());

	CL_Display::push_clip_rect();
	CL_Display::set_clip_rect(CL_ClipRect(0, 0, CL_Display::get_width(), CL_Display::get_height()));

	CL_Display::push_translate_offset();
	CL_Display::set_translate_offset(0, 0);
	
	CL_Rect rect = window->get_screen_rect();

	// Vertical shadows
	CL_Display::draw_line(rect.x2 + 0, rect.y1 + 5, rect.x2 + 0, rect.y2 + 0, 0.0f, 0.0f, 0.0f, 0.25f);
	CL_Display::draw_line(rect.x2 + 1, rect.y1 + 5, rect.x2 + 1, rect.y2 + 1, 0.0f, 0.0f, 0.0f, 0.20f);
	CL_Display::draw_line(rect.x2 + 2, rect.y1 + 5, rect.x2 + 2, rect.y2 + 2, 0.0f, 0.0f, 0.0f, 0.15f);
	CL_Display::draw_line(rect.x2 + 3, rect.y1 + 5, rect.x2 + 3, rect.y2 + 3, 0.0f, 0.0f, 0.0f, 0.10f);
	CL_Display::draw_line(rect.x2 + 4, rect.y1 + 5, rect.x2 + 4, rect.y2 + 4, 0.0f, 0.0f, 0.0f, 0.05f);

	// Horizontal shadows
	CL_Display::draw_line(rect.x1 + 5, rect.y2 + 0, rect.x2 - 1, rect.y2 + 0, 0.0f, 0.0f, 0.0f, 0.25f);
	CL_Display::draw_line(rect.x1 + 5, rect.y2 + 1, rect.x2 + 0, rect.y2 + 1, 0.0f, 0.0f, 0.0f, 0.20f);
	CL_Display::draw_line(rect.x1 + 5, rect.y2 + 2, rect.x2 + 1, rect.y2 + 2, 0.0f, 0.0f, 0.0f, 0.15f);
	CL_Display::draw_line(rect.x1 + 5, rect.y2 + 3, rect.x2 + 2, rect.y2 + 3, 0.0f, 0.0f, 0.0f, 0.10f);
	CL_Display::draw_line(rect.x1 + 5, rect.y2 + 4, rect.x2 + 3, rect.y2 + 4, 0.0f, 0.0f, 0.0f, 0.05f);

	CL_Display::pop_clip_rect();
	CL_Display::pop_translate_offset();
}

void CL_Window_Default::on_close()
{
//	window->quit();
	window->close();
}

void CL_Window_Default::on_resize(int old_width, int old_height)
{
	delete move_handler;
	move_handler = new CL_ComponentMoveHandler(
		CL_Rect(0, 0, window->get_width(), titlebar_height),
		window,
		window->get_style_manager());
}
