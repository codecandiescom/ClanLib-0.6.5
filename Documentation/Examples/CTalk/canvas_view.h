/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#ifndef _CANVAS_VIEW_H_
#define _CANVAS_VIEW_H_

#include "view.h"
#include "canvas.h"

class MainFrame;

class CanvasView : public View
{
public:
	// Construction:
	CanvasView(MainFrame *parent);
	~CanvasView();
	
	// Attributes:
	
	// Operations:
	void add_sketch();

	// Signals:
	
	// implementation:
private:
	CL_LayoutManager layout;

	CL_SlotContainer slots;

	Canvas canvas;
	std::list<CL_Button *> button_sketches;

	CL_Button *button_close;

	CL_Button *button_new_picture;
	CL_Button *button_new_sketch;

	CL_Button *button_undo;
	CL_Button *button_redo;

	CL_Button *button_invite;
	CL_Button *button_kick;

	CL_Component *component_sketch;

	bool drawing;

	Sketch *current_sketch;
	CL_Surface *current_picture;

	void check_undo_redo();

	// events:
	void on_mouse_down(const CL_Key &key);

	void on_mouse_up(const CL_Key &key);

	void on_mouse_move(int x, int y);

	void on_button_sketch(Sketch *sketch);
	std::list<CL_Slot> slots_button_sketch;

	void on_button_picture(CL_Surface *picture);
	std::list<CL_Slot> slots_button_picture;

	void on_button_new_sketch();

	void on_button_new_picture();

	void on_button_undo();

	void on_button_redo();

	void on_button_invite();

	void on_button_kick();

	void on_paint();

	void on_paint_sketch();

	CL_Slot slot_button_close;
};

#endif
