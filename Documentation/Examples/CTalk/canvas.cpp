/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#include "canvas.h"
#include "sketch.h"

#include <ClanLib/display.h>

/////////////////////////////////////////////////////////////////////////////
// Construction:

Canvas::Canvas()
{
}

Canvas::~Canvas()
{
	clear();
}
	
/////////////////////////////////////////////////////////////////////////////
// Attributes:

/////////////////////////////////////////////////////////////////////////////
// Operations:

void Canvas::clear()
{
	std::list<Sketch *>::iterator sit;
	for (sit = sketches.begin(); sit != sketches.end(); sit++)
	{
		delete *sit;
	}

	sketches.clear();

	std::list<CL_Surface *>::iterator pit;
	for (pit = pictures.begin(); pit != pictures.end(); pit++)
	{
		delete *pit;
	}

	pictures.clear();
}

void Canvas::add_sketch(Sketch *sketch)
{
	sketches.push_back(sketch);
}

void Canvas::add_picture(CL_Surface *surface)
{
	pictures.push_back(surface);
}

void Canvas::draw()
{
	std::list<CL_Surface *>::iterator pit;
	for (pit = pictures.begin(); pit != pictures.end(); pit++)
	{
//		if((*pit)->is_visible())
			(*pit)->put_screen(0, 0);
	}

	std::list<Sketch *>::iterator sit;
	for (sit = sketches.begin(); sit != sketches.end(); sit++)
	{
		if((*sit)->is_visible())
			(*sit)->draw();
	}
}

/////////////////////////////////////////////////////////////////////////////
// Events:

/////////////////////////////////////////////////////////////////////////////
// Implementation:
