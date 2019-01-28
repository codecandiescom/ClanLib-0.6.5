#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "stylemanager_opengl.h"
#include "inputbox_opengl.h"

#define BORDER_SIZE 12

CL_InputBox_OpenGL::CL_InputBox_OpenGL(
	CL_InputBox *inputbox,
	CL_StyleManager_OpenGL *style)
: CL_ComponentStyle(inputbox), inputbox(inputbox)
{
	this->style = style;

	resources = style->get_resources();
	font = CL_Font::load("InputBox/font", resources);

	if(inputbox->get_height() == 35)
	{
		int width = inputbox->get_width() / 16;
		inputbox->set_width(width * 16);

		surface_left = new CL_Surface("Inputbox/left", resources);
		surface_center = new CL_Surface("Inputbox/center", resources);
		surface_right = new CL_Surface("Inputbox/right", resources);
	}

	show_cursor = false;
	character_offset = 0;
	cursor_blink_time = CL_System::get_time();
	
	slot_paint = inputbox->sig_paint().connect(this, &CL_InputBox_OpenGL::on_paint);
}

CL_InputBox_OpenGL::~CL_InputBox_OpenGL()
{
//	delete font;
}

void CL_InputBox_OpenGL::on_paint()
{
	if(surface_left)
		on_paint_texture();
	else
		on_paint_default();
}

void CL_InputBox_OpenGL::on_paint_texture()
{
	surface_left->put_screen(0, 0);

	int x;
	for(x = 16; x <= inputbox->get_width() - 32; x += 16)
		surface_center->put_screen(x, 0);

	surface_right->put_screen(inputbox->get_width() - 16, 0);

	on_paint_input();
}

void CL_InputBox_OpenGL::on_paint_default()
{
	// Draw frames
	CL_Display::draw_rect(0, 0, inputbox->get_width(), inputbox->get_height(), 0.0f, 0.0f, 0.0f, 0.7f);

	CL_Display::fill_rect(0, 0, inputbox->get_width(), inputbox->get_height(), 0.1f, 1.0f, 0.0f, 0.1f);
	CL_Display::fill_rect(1, 1, inputbox->get_width() - 1, inputbox->get_height() - 1, 0.1f, 1.0f, 0.0f, 0.1f);

	on_paint_input();
}

void CL_InputBox_OpenGL::on_paint_input()
{
	// For easy reference, put inputfield-text into local variable
	const char *text = inputbox->get_text().c_str();

//	int pixel_y_offset = (inputbox->get_height() - font->get_height()) / 2;
	int pixel_y_offset = 6;

	// Calculate scroll offset
	int character_offset = 0;
	int pixel_offset = 0;
	int width = (int)font->get_text_width(text);
	if (width > inputbox->get_width() - BORDER_SIZE * 2)
	{
		while (
			width >= inputbox->get_width() - BORDER_SIZE * 2 &&
			character_offset + 1 < inputbox->get_cursor_position())
		{
			int w = font->get_char_width(text[character_offset]);
			character_offset++;
			pixel_offset += w;
			width -= w;
		}
	}

	// Display marked text, if any
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

		CL_Display::fill_rect(
			BORDER_SIZE + mark_x1 - 1,
			pixel_y_offset,
			BORDER_SIZE + mark_x1 + mark_x2,
			pixel_y_offset + font->get_height() + 1,
			0.0f, 0.0f, 0.0f, 0.5f);
	}

	// Display text
	font->print_left(BORDER_SIZE, pixel_y_offset, &text[character_offset]);

	// Show blinking cursor
	if(inputbox->has_focus()) 
	{
		if (show_cursor)
		{
			int cursor_x = BORDER_SIZE - pixel_offset;

			for(int i = 0; i < inputbox->get_cursor_position(); i++)
				cursor_x += font->get_char_width(text[i]);
				
			style->draw_line(cursor_x, pixel_y_offset, cursor_x, pixel_y_offset + font->get_height() + 1, GUICOLOR_CARET);
		}
		unsigned int cur_time = CL_System::get_time();
		if (cur_time >= cursor_blink_time)
		{
			cursor_blink_time = cur_time + 400;
			show_cursor = !show_cursor;
		}
	}
}
