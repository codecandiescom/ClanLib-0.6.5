/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#ifndef _VIEW_H_
#define _VIEW_H_

#include <ClanLib/gui.h>

class MainFrame;

class View : public CL_Frame
{
// Construction:
public:
	View(MainFrame *mainframe);
	// Constructs a view.

	virtual ~View();

// Attributes:
public:
	bool is_floating() const { return floating; }
	// Returns true if view is currently floating.

	MainFrame *get_mainframe() const { return mainframe; }
	// Returns the mainframe the view is attached to.

	const std::string &get_title() const { return title; }
	// Returns the title of the view used in the notebook pane.

	CL_Signal_v1<bool> sig_floating;
	// Signal invoked when view switches floating mode.

	CL_Signal_v1<const std::string &> sig_new_title;
	// Signal invoked when a new title is set.

// Operations:
public:
	void set_floating(bool floating = true);
	// Change view between floating and maximized mode.

	void set_title(const std::string &new_title);
	// Sets the title of the view used in the notebook pane.

// Implementation:
private:
	MainFrame *mainframe;
	// Mainframe the view is attached to.

	bool floating;
	// True if view is in floating mode.

	std::string title;
	// Title of view.
};

#endif
