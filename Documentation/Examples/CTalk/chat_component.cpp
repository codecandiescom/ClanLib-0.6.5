/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#include "chat_component.h"

#include <ClanLib/display.h>

/////////////////////////////////////////////////////////////////////////////
// Construction:

ChatComponent::ChatComponent(CL_Component *parent)
: input(0), CL_Frame(CL_Rect(0,0,0,0), parent), font("Window/font", get_style_manager()->get_resources())
{
	// Connect events to signals:
	slots.connect(sig_paint(), this, &ChatComponent::on_paint);
}

ChatComponent::~ChatComponent()
{
}
	
/////////////////////////////////////////////////////////////////////////////
// Attributes:
	
/////////////////////////////////////////////////////////////////////////////
// Operations:

/////////////////////////////////////////////////////////////////////////////
// Events:

void ChatComponent::on_paint()
{
	CL_Display::fill_rect(0, 0, get_width(), get_height(), 141/255.0f, 161/255.0f, 189/255.0f);

	for(int yl = 0; yl < get_height(); yl += 3)
		CL_Display::draw_line(0, yl, get_width(), yl, 107/255.0f, 131/255.0f, 164/255.0f);

//	CL_Display::fill_rect(0, 0, get_width(), get_height(), 1.0f, 1.0f, 1.0f);

	std::list<std::string> text;
	text.push_back("<mbn> Hi Sphair :)");
	text.push_back("<sphair> Hey hey");
	text.push_back("<mbn> Wazzap??");
	text.push_back("<sphair> Life's good, I guess. Ok, enough pseudo conversation for now :)");
	text.push_back("<sphair> This is a very long text string that is supposed to be word wrapped over several lines to test the very cool and awesome word wrapper routine in the chat component. I think this string should do it... no wait, let's type slightly more just to be sure. Yes, I think this should somewhat do it!");

	int y = get_height() - 10;
	
	std::list<std::string>::iterator it;
	for (it = text.end(); it != text.begin();)
	{
		it--;
		y = print_word_wrapped(y, *it);
	}
}

int ChatComponent::print_word_wrapped(int y, const std::string &str)
{
/*	int length = str.length();
	int line_begin = 0;
	int word_begin = 0;
	for (int i=0; i<length; i++)
	{
		if (str[i] == ' ')
		{
			int width_line = font.get_text_width(str.substr(line_begin, i - line_begin).c_str());
		}
	}
*/
	y -= font.get_height()+6;

	CL_Display::fill_rect(8, y-2, get_width()-8, y+font.get_height()+2, 0.9f, 0.9f, 1.0f);

	font.print_left(10, y, str.c_str());
	return y;
}