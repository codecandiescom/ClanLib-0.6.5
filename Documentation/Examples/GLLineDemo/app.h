/* $Id: app.h,v 1.3 2001/10/15 17:55:04 japj Exp $ */

#ifndef _APP_H_
#define _APP_H_

#define APP_VER_MINOR 1
#define APP_VER_MAJOR 0

#define APP_NAME "OpenGL Line Demo - Sphair 2001"

#define LINE_COUNT 15

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include "line.h"

class Application : public CL_ClanApplication
{
public:
	Application() {};

	char *get_title();
	int main(int argc, char** argv);

private:
	void run();
	void update();
	void draw();

private:
	Line lines[LINE_COUNT];

	CL_Surface *logo;
};

extern Application app;

#endif
