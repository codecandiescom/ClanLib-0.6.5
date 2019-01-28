/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#include "mainframe.h"
#include "view.h"
#include "canvas_view.h"

#include <ClanLib/display.h>

// Construction:
MainFrame::MainFrame(CL_ResourceManager *resources, CL_Component *parent, CL_StyleManager *styles)
:
	CL_Frame(CL_Rect(0,0,CL_Display::get_width(),CL_Display::get_height()), parent, styles),
	resources(resources), styles(styles), active_view(NULL), document(NULL)
{
	button_new_canvas = new CL_Button(CL_Rect(0, 20, 100, 40), "New canvas", this);

	slots.connect(button_new_canvas->sig_clicked(), this, &MainFrame::on_button_new_canvas);
	slots.connect(CL_Display::sig_resized(), this, &MainFrame::on_resize);
	slots.connect(sig_paint(), this, &MainFrame::on_paint);

	set_position(CL_Rect(0, 0, CL_Display::get_width(), CL_Display::get_height()));
}

MainFrame::~MainFrame()
{
}

View *MainFrame::get_active_view()
{
	return active_view;
}

CL_ResourceManager *MainFrame::get_resources()
{
	return resources;
}

CL_StyleManager *MainFrame::get_styles()
{
	return styles;
}

#define BUTTON_WIDTH 102
#define BUTTON_HEIGHT 20

void MainFrame::add_view(View *view)
{
	views.push_back(view);
	position_window(view);

	// Calculate position and create new button
	int count = button_views.size();
	int xpos = count * BUTTON_WIDTH;
	int ypos = 0;
	CL_Button *button = new CL_Button(
		CL_Rect(xpos, ypos, xpos + BUTTON_WIDTH - 2, ypos + BUTTON_HEIGHT),
		view->get_title(),
		this);
	button_views.push_back(button);

	CL_Slot slot1 = view->sig_new_title.connect(this, &MainFrame::on_view_new_title, button);
	slots_view_newtitle.push_back(slot1);

	CL_Slot slot2 = button->sig_clicked().connect(this, &MainFrame::on_view_button_clicked, view);
	slots_view_click.push_back(slot2);
}

void MainFrame::remove_view(View *view)
{
	views.remove(view);

	// TODO: Remove slots from slots_view_newtitle and slots_view_click
}

void MainFrame::set_active_view(View *view)
{
	if (active_view != NULL) active_view->show(false);
	active_view = view;
	if (active_view != NULL) active_view->show(true);
}

void MainFrame::position_window(View *view)
{
	// Only position maximized windows.
	if (view->is_floating()) return;

	CL_Rect pos;
	pos.x1 = 0;
	pos.y1 = 20 * 2;
	pos.x2 = get_width();
	pos.y2 = get_height();
	view->set_position(pos);
}

void MainFrame::on_resize(int width, int height)
{
	set_position(width, height);

	// Resize all maximized views.
	for (
		std::list<View *>::iterator it = views.begin();
		it != views.end();
		it++)
	{
		position_window(*it);
	}
}

void MainFrame::on_paint()
{
	CL_Display::fill_rect(0, 0, get_width(), 20, 0.7f, 0.7f, 1.0f);
	CL_Display::fill_rect(0, 20, get_width(), 40, 0.5f, 0.5f, 0.8f);
}

void MainFrame::on_view_new_title(const std::string &title, CL_Button *button)
{
	button->set_text(title);
}

void MainFrame::on_view_button_clicked(View *view)
{
	set_active_view(view);
}

void MainFrame::on_button_new_canvas()
{
	CanvasView *canvas_view = new CanvasView(this);
	set_active_view(canvas_view);
}
