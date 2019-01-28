#include <stdlib.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "line.h"

Line::Line()
{
	if(rand() % 2 == 0) {
		x1 = rand() % 640;
		x2 = rand() % 640;
		y1 = 0.0f;
		y2 = 479.0f;

		vertical = true;
	}
	else {
		y1 = rand() % 480;
		y2 = rand() % 480;
		x1 = 0.0f;
		x2 = 639.0f;

		vertical = false;
	}

	speed_x1 = (rand() % 100) / 100.0f + 0.1f;
	speed_x2 = (rand() % 100) / 100.0f + 0.1f;
	speed_y1 = (rand() % 100) / 100.0f + 0.1f;
	speed_y2 = (rand() % 100) / 100.0f + 0.1f;
}

void Line::update()
{
	if(vertical) {
		x1 += speed_x1;
		if(x1 >= 640 || x1 < 0)
			speed_x1 = -speed_x1;

		x2 += speed_x2;
		if(x2 >= 640 || x2 < 0)
			speed_x2 = -speed_x2;
	}
	else {
		y1 += speed_y1;
		if(y1 >= 480 || y1 < 0)
			speed_y1 = -speed_y1;

		y2 += speed_y2;
		if(y2 >= 480 || y2 < 0)
			speed_y2 = -speed_y2;
	}
}

void Line::draw()
{
	CL_Display::draw_line((int)x1, (int)y1, (int)x2, (int)y2, 0.0f, 0.0f, 0.0f, 0.1f);
	CL_Display::draw_line((int)x1 + 1, (int)y1 - 1, (int)x2 - 1, (int)y2 + 1, 0.0f, 0.0f, 0.0f, 0.1f);
	CL_Display::draw_line((int)x1 - 1, (int)y1 + 1, (int)x2 + 1, (int)y2 - 1, 0.0f, 0.0f, 0.0f, 0.1f);
}
