/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#include "canvas_view.h"
#include "sketch.h"

#include <ClanLib/display.h>
#include <ClanLib/gui.h>

/////////////////////////////////////////////////////////////////////////////
// Construction:

CanvasView::CanvasView(MainFrame *parent)
: View(parent)
{
	current_sketch = NULL;
	drawing = false;

	int xlen = get_width();
	int ylen = get_height();

	component_sketch = new CL_Component(CL_Rect(0, 0, xlen, ylen - 20), this);

	button_close = new CL_Button(
		CL_Rect(0, ylen - 20, 70, ylen), "Close", this);

	button_new_picture = new CL_Button(
		CL_Rect(90, ylen - 20, 160, ylen), "Add picture", this);
	button_new_sketch = new CL_Button(
		CL_Rect(160, ylen - 20, 230, ylen), "Add sketch", this);

	button_invite = new CL_Button(
		CL_Rect(250, ylen - 20, 320, ylen), "Invite", this);
	button_kick = new CL_Button(
		CL_Rect(320, ylen - 20, 390, ylen), "Kick", this);

	button_undo = new CL_Button(
		CL_Rect(410, ylen - 20, 480, ylen), "Undo", this);
	button_redo = new CL_Button(
		CL_Rect(480, ylen - 20, 550, ylen), "Redo", this);
	check_undo_redo();

/*	layout.add_resize_position(
		component_sketch, CL_LayoutManager::y1, this, CL_LayoutManager::y2);
	layout.add_resize_position(
		component_sketch, CL_LayoutManager::y2, this, CL_LayoutManager::y2);
	layout.add_resize_position(
		component_sketch, CL_LayoutManager::x2, this, CL_LayoutManager::x2);

	layout.add_resize_position(
		button_close, CL_LayoutManager::y1, this, CL_LayoutManager::y2);
	layout.add_resize_position(
		button_close, CL_LayoutManager::y2, this, CL_LayoutManager::y2);

	layout.add_resize_position(
		button_new_sketch, CL_LayoutManager::x1, this, CL_LayoutManager::x2);
	layout.add_resize_position(
		button_new_sketch, CL_LayoutManager::x2, this, CL_LayoutManager::x2);
	layout.add_resize_position(
		button_new_sketch, CL_LayoutManager::y1, this, CL_LayoutManager::y2);
	layout.add_resize_position(
		button_new_sketch, CL_LayoutManager::y2, this, CL_LayoutManager::y2);
*/
	// Connect events to signals:
	slots.connect(sig_paint(), this, &CanvasView::on_paint);
	slots.connect(component_sketch->sig_paint(), this, &CanvasView::on_paint_sketch);
	slots.connect(button_new_picture->sig_clicked(), this, &CanvasView::on_button_new_picture);
	slots.connect(button_new_sketch->sig_clicked(), this, &CanvasView::on_button_new_sketch);
	slots.connect(button_kick->sig_clicked(), this, &CanvasView::on_button_kick);
	slots.connect(button_invite->sig_clicked(), this, &CanvasView::on_button_invite);
	slots.connect(button_undo->sig_clicked(), this, &CanvasView::on_button_undo);
	slots.connect(button_redo->sig_clicked(), this, &CanvasView::on_button_redo);
	slots.connect(button_close->sig_clicked(), (CL_Component*)this, &CL_Component::quit);
	slots.connect(sig_mouse_down(), this, &CanvasView::on_mouse_down);
	slots.connect(sig_mouse_up(), this, &CanvasView::on_mouse_up);
	slots.connect(sig_mouse_move(), this, &CanvasView::on_mouse_move);

	set_title("Canvas");
}

CanvasView::~CanvasView()
{
}
	
/////////////////////////////////////////////////////////////////////////////
// Attributes:
	
/////////////////////////////////////////////////////////////////////////////
// Operations:

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 20

void CanvasView::add_sketch()
{
	current_sketch = new Sketch;
	canvas.add_sketch(current_sketch);

	int count = button_sketches.size();

	// Calculate position and create new button
	int xpos = get_width();
	int ypos = count * BUTTON_HEIGHT;
	CL_Button *button = new CL_Button(
		CL_Rect(xpos - BUTTON_WIDTH, ypos, xpos, ypos + BUTTON_HEIGHT), "Sketch", this);
	button_sketches.push_back(button);

	CL_Slot slot = button->sig_clicked().connect(this, &CanvasView::on_button_sketch, current_sketch);
	slots_button_sketch.push_back(slot);

	layout.add_resize_position(
		button, CL_LayoutManager::x1, this, CL_LayoutManager::x2);
	layout.add_resize_position(
		button, CL_LayoutManager::x2, this, CL_LayoutManager::x2);
}

/////////////////////////////////////////////////////////////////////////////
// Events:

void CanvasView::on_button_new_sketch()
{
	add_sketch();

	check_undo_redo();
}

void CanvasView::on_button_new_picture()
{
	std::string filename = CL_FileDialog::open(this);
	if(filename.length() == 0)
		return;

	CL_Surface *surface;
	try
	{
		surface = new CL_Surface(filename);
	}
	catch(CL_Error err)
	{
		CL_MessageBox::info(err.message, this);
		return;
	}

	current_picture = surface;
	canvas.add_picture(surface);

	int count = button_sketches.size();

	// Calculate position and create new button
	int xpos = get_width();
	int ypos = count * BUTTON_HEIGHT;
	CL_Button *button = new CL_Button(
		CL_Rect(xpos - BUTTON_WIDTH, ypos, xpos, ypos + BUTTON_HEIGHT), "Picture", this);
	button_sketches.push_back(button);

	CL_Slot slot = button->sig_clicked().connect(this, &CanvasView::on_button_picture, current_picture);
	slots_button_picture.push_back(slot);

	layout.add_resize_position(
		button, CL_LayoutManager::x1, this, CL_LayoutManager::x2);
	layout.add_resize_position(
		button, CL_LayoutManager::x2, this, CL_LayoutManager::x2);
}

void CanvasView::on_button_sketch(Sketch *sketch)
{
	if(sketch->is_visible())
		sketch->show(false);
	else
		sketch->show(true);

	current_sketch = sketch;

	check_undo_redo();
}

void CanvasView::on_button_picture(CL_Surface *picture)
{
/*	if(picture->is_visible())
		picture->show(false);
	else
		picture->show(true);
*/
	current_picture = picture;
}

void CanvasView::on_button_undo()
{
	if(current_sketch)
		current_sketch->undo();

	check_undo_redo();
}

void CanvasView::on_button_redo()
{
	if(current_sketch)
		current_sketch->redo();

	check_undo_redo();
}

void CanvasView::on_mouse_down(const CL_Key &key)
{
	if(current_sketch)
		drawing = true;
}

void CanvasView::on_mouse_up(const CL_Key &key)
{
	drawing = false;
}

void CanvasView::on_mouse_move(int x, int y)
{
	static int old_x = -1;
	static int old_y = -1;

	if(drawing)
	{
		current_sketch->add_line(CL_Rect(old_x, old_y, x, y));

		check_undo_redo();
	}

	old_x = x;
	old_y = y;
}

void CanvasView::on_paint()
{
	// Background
	CL_Display::fill_rect(0, 0, get_width(), get_height() - 20, 1.0f, 1.0f, 1.0f);

	// Lower button-bar
	CL_Display::fill_rect(0, get_height() - 20, get_width(), get_height(), 141/255.0f, 161/255.0f, 189/255.0f);
}

void CanvasView::on_paint_sketch()
{
	canvas.draw();
}

void CanvasView::check_undo_redo()
{
	bool undo = false, redo = false;

	if(current_sketch)
	{
		if(current_sketch->has_undo())
			undo = true;
		if(current_sketch->has_redo())
			redo = true;
	}

	button_undo->enable(undo);
	button_redo->enable(redo);
}

void CanvasView::on_button_invite()
{
	CL_Rect rect = button_invite->get_position();
	int xpos = rect.x1;
	int ypos = rect.y1 - 63;

/*	CL_PopupMenu *popup = new CL_PopupMenu(CL_Point(xpos, ypos), this);

	CL_Button *button1 = new CL_Button("Everyone", popup);
	CL_Button *button2 = new CL_Button("Sphair", popup);
	CL_Button *button3 = new CL_Button("Mbn", popup);

	popup->set_focus();
*/
}

void CanvasView::on_button_kick()
{
}
