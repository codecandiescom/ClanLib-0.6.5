/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#ifndef _CANVAS_H_
#define _CANVAS_H_

#include <list>

class Sketch;
class CL_Surface;

class Canvas
{
public:
	// Construction:
	Canvas();
	~Canvas();
	
	// Operations:
	void clear();

	void add_picture(CL_Surface *surface);
	void add_sketch(Sketch *sketch);

	void draw();

	// Signals:
	
	// implementation:
private:
	std::list<Sketch *> sketches;
	std::list<CL_Surface *> pictures;
};

#endif
