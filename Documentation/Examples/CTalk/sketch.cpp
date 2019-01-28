/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#include "sketch.h"

#include <ClanLib/core.h>
#include <ClanLib/display.h>

/////////////////////////////////////////////////////////////////////////////
// Construction:

Sketch::Sketch()
{
	visible = true;

	red = (rand() % 100) / 100.0f;
	green = (rand() % 100) / 100.0f;
	blue = (rand() % 100) / 100.0f;
}

Sketch::~Sketch()
{
	clear();
	clear_redo();
}
	
/////////////////////////////////////////////////////////////////////////////
// Attributes:

bool Sketch::has_undo()
{
	return (lines.empty() == false);
}

bool Sketch::has_redo()
{
	return (redo_lines.empty() == false);
}

bool Sketch::is_visible()
{
	return visible;
}
	
/////////////////////////////////////////////////////////////////////////////
// Operations:

void Sketch::add_line(CL_Rect r)
{
	lines.push_back(new CL_Rect(r.x1, r.y1, r.x2, r.y2));	

	clear_redo();
}

void Sketch::clear()
{
	std::list<CL_Rect *>::iterator it;
	for (it = lines.begin(); it != lines.end(); it++)
	{
		delete *it;
	}

	lines.clear();
}

void Sketch::undo()
{
	if(has_undo())
	{
		redo_lines.push_back(lines.back());
		lines.pop_back();
	}
}

void Sketch::redo()
{
	if(has_redo())
	{
		lines.push_back(redo_lines.back());
		redo_lines.pop_back();
	}
}

void Sketch::draw()
{
	std::list<CL_Rect *>::iterator it;
	for (it = lines.begin(); it != lines.end(); it++)
	{
		CL_Rect *r = (*it);
		CL_Display::draw_line(r->x1, r->y1, r->x2, r->y2, red, green, blue);
	}
}

void Sketch::show(bool show)
{
	visible = show;
}

/////////////////////////////////////////////////////////////////////////////
// Events:

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void Sketch::clear_redo()
{
	std::list<CL_Rect *>::iterator it;
	for (it = redo_lines.begin(); it != redo_lines.end(); it++)
	{
		delete *it;
	}

	redo_lines.clear();
}