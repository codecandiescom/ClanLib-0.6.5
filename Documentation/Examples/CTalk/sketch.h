/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#ifndef _SKETCH_H_
#define _SKETCH_H_

#include <list>

class CL_Rect;

class Sketch
{
// Construction:
public:
	Sketch();

	~Sketch();
	
// Attributes:
public:
	bool has_undo();

	bool has_redo();

	bool is_visible();

// Operations:
public:
	void add_line(CL_Rect rect);

	void clear();

	void undo();

	void redo();

	void draw();

	void show(bool show = true);

// Implementation:
private:
	void clear_redo();

	std::list<CL_Rect *> lines;

	std::list<CL_Rect *> redo_lines;

	bool visible;

	float red,green,blue;
};

#endif
