/* $Id: line.h,v 1.2 2001/10/15 17:55:05 japj Exp $ */

#ifndef _LINE_H_
#define _LINE_H_

class Line
{
public:
	Line();

	void update();
	void draw();

private:
	bool vertical;

	float x1;
	float y1;
	float x2;
	float y2;

	float speed_x1;
	float speed_y1;
	float speed_x2;
	float speed_y2;
};

#endif
