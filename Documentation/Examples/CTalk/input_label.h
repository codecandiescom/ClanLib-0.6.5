/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#ifndef _INPUT_LABEL_H_
#define _INPUT_LABEL_H_

#include "mainframe.h"

class InputLabel : public CL_Label
{
public:
	// Construction:
	InputLabel(const CL_Rect &pos, const std::string &text, CL_Component *parent);
	~InputLabel();

	// Attributes:

	// Operations:

	// Signals:
	CL_Signal_v0 sig_enter;

	// implementation:
private:
	void on_key_down(const CL_Key &key);
	CL_Slot slot_key_down;
};

#endif
