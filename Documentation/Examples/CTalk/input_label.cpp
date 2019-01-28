/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#include "input_label.h"

// Construction:
InputLabel::InputLabel(const CL_Rect &pos, const std::string &text, CL_Component *parent)
: CL_Label(pos, text, parent)
{
	slot_key_down = sig_key_down().connect(this, &InputLabel::on_key_down);
}

InputLabel::~InputLabel()
{
}

// Attributes:

// Operations:

// Signals:

// implementation:

void InputLabel::on_key_down(const CL_Key &key)
{
	if (key.id == CL_KEY_ENTER) sig_enter();
	if (key.ascii > 0) set_text(get_text() + std::string((char)key.ascii, 1));
}
