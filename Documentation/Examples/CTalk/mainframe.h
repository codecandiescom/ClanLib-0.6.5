/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#ifndef _MAINFRAME_H_
#define _MAINFRAME_H_

#include <ClanLib/gui.h>

class View;
class Document;

class MainFrame : public CL_Frame
{
// Construction:
public:
	MainFrame(
		CL_ResourceManager *resources,
		CL_Component *parent,
		CL_StyleManager *styles);

	~MainFrame();

// Attributes:
public:
	View *get_active_view();

	CL_ResourceManager *get_resources();

	CL_StyleManager *get_styles();

	Document *get_document() { return document; }

// Operations:
public:
	void add_view(View *view);

	void remove_view(View *view);

	void set_active_view(View *view);

	void set_document(Document *doc) { document = doc; }

// Implementation:
private:
	CL_ResourceManager *resources;
	CL_StyleManager *styles;
	View *active_view;
	Document *document;
	std::list<View *> views;
	std::list<CL_Button *> button_views;
	std::list<CL_Slot> slots_view_newtitle;
	std::list<CL_Slot> slots_view_click;
	CL_SlotContainer slots;

	CL_Button *button_new_canvas;

	void position_window(View *view);

	// events:
	void on_resize(int width, int height);
	
	void on_paint();

	void on_view_new_title(const std::string &title, CL_Button *button);
	// Called when a view changes its title.

	void on_view_button_clicked(View *view);
	// Called when a view's notebook pane button is clicked.

	void on_button_new_canvas();
	// Called when the new canvas button is clicked
};

#endif
