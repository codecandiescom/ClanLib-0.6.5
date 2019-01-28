/*
	$Id: inputbox_default.cpp,v 1.45 2002/01/16 19:02:12 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/Display/Font/font.h"
#include "API/Core/System/system.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/GUI/gui_manager.h"
#include "inputbox_default.h"

#define BORDER_SIZE 3

CL_InputBox_Default::CL_InputBox_Default(
	CL_InputBox *inputbox,
	CL_StyleManager_Default *style)
: CL_ComponentStyle(inputbox), inputbox(inputbox)
{
	this->style = style;

	resources = style->get_resources();
	font = CL_Font::load("InputBox/font", resources);
	font_disabled = CL_Font::load("InputBox/font_disabled", resources);

	CL_Rect position = inputbox->get_position();
	position.y2 = position.y1 + font->get_height() + 5;
	inputbox->set_position(position);

	show_cursor = false;
	character_offset = 0;
	cursor_blink_time = CL_System::get_time();
	
	slot_paint = inputbox->sig_paint().connect(
		this, &CL_InputBox_Default::on_paint);
	slot_activity = inputbox->sig_activity().connect(
		this, &CL_InputBox_Default::on_activity);
}

CL_InputBox_Default::~CL_InputBox_Default()
{
	delete font;
	delete font_disabled;
}

void CL_InputBox_Default::on_paint()
{
	int width = inputbox->get_width();
	int height = inputbox->get_height();

	if(inputbox->is_enabled() == false || inputbox->has_focus() == false)
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

	// For easy reference, put inputfield-text into local variable
	const char *text = inputbox->get_text().c_str();

	// Calculate scroll offset
	int character_offset = 0;
	int pixel_offset = 0;
	int text_width = (int)font->get_text_width(text);
	if (text_width > width - BORDER_SIZE * 2)
	{
		while (
			text_width >= width - BORDER_SIZE * 2 &&
			character_offset + 1 < inputbox->get_cursor_position())
		{
			int w = font->get_char_width(text[character_offset]);
			character_offset++;
			pixel_offset += w;
			text_width -= w;
		}
	}

	// Display marked text, if any
	if(inputbox->has_focus()) 
	{
		if (inputbox->has_marked_text())
		{
			int start = inputbox->get_selection_start();
			int end = start + inputbox->get_selection_length();

			int mark_x1 = 0, mark_x2 = 0;

			int i;
			for(i = 0; i < start; i++)
				mark_x1 += font->get_char_width(text[i]);

			for(i = start; i < end; i++)
				mark_x2 += font->get_char_width(text[i]);

			style->fill_rect(
				BORDER_SIZE + mark_x1 - 1,
				BORDER_SIZE - 1,
				BORDER_SIZE + mark_x1 + mark_x2,
				BORDER_SIZE + font->get_height(),
				GUICOLOR_SELECTION);
		}
	}

	// Display text
	if(inputbox->is_enabled() == false)
		font_disabled->print_left(BORDER_SIZE, 2, &text[character_offset]);
	else
		font->print_left(BORDER_SIZE, 2, &text[character_offset]);

	// Show blinking cursor
	if(inputbox->has_focus() && inputbox->is_enabled()) 
	{
		if (show_cursor)
		{
			int cursor_x = BORDER_SIZE - pixel_offset;

			for(int i = 0; i < inputbox->get_cursor_position(); i++)
				cursor_x += font->get_char_width(text[i]);
				
			style->draw_line(cursor_x, BORDER_SIZE - 1, cursor_x, BORDER_SIZE + font->get_height() - 1, GUICOLOR_CARET);
		}

		unsigned int cur_time = CL_System::get_time();
		if (cur_time >= cursor_blink_time)
		{
			cursor_blink_time = cur_time + 400;
			show_cursor = !show_cursor;
		}
	}
}

void CL_InputBox_Default::on_activity()
{
	cursor_blink_time = CL_System::get_time() + 400;
	show_cursor = true;
}

int CL_InputBox_Default::get_mouse_position(int x, int y)
{
	int delta_x = x - BORDER_SIZE;
	unsigned int i;
	for (i = 0; i < inputbox->get_text().size(); i++)
	{
		char buf[2];
		buf[0] = inputbox->get_text()[i];
		buf[1] = 0;
		int w = font->get_text_width(buf);
		delta_x -= w;
		if (delta_x <= -w/2)
		{
			return i + character_offset;
		}
	}
	return i;
}
