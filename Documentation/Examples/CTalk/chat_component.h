/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#ifndef _CHAT_COMPONENT_H_
#define _CHAT_COMPONENT_H_

#include <ClanLib/gui.h>
#include <ClanLib/display.h>

class InputLabel;

class ChatComponent : public CL_Frame
{
public:
	// Construction:
	ChatComponent(CL_Component *parent);

	~ChatComponent();
	
	// Attributes:
	
	// Operations:
	
	// Signals:
	
	// implementation:
private:
	int print_word_wrapped(int y, const std::string &str);

	// Lines of chat text.
	std::list<CL_Component *> lines;

	// Current line we are writing.
	InputLabel *input;

	CL_SlotContainer slots;

	// Resources:
	CL_Font font;

	// events:
	void on_paint();
};

#endif
