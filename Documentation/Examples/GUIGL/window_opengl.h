/*
	OpenGL window gui test thingy,
	Copyright (c)2000 Magnus Norddahl / ClanSoft.
*/

#ifndef header_window_style_opengl
#define header_window_style_opengl

#include <ClanLib/GUI/component_style.h>
#include <ClanLib/GUI/button.h>
#include <ClanLib/GUI/component_move_handler.h>

class CL_Window;
class CL_StyleManager_OpenGL;
class CL_ResourceManager;
class CL_ComponentMoveHandler;

class CL_Window_OpenGL : public CL_ComponentStyle
{
public:
	CL_Window_OpenGL(
		CL_Window *window,
		CL_StyleManager_OpenGL *style);

	virtual ~CL_Window_OpenGL();

private:
	void on_paint();
	void on_resized(int width, int height);
	void on_quit();
	void on_transform_coords(int &x, int &y);

	CL_Slot slot_paint;
	CL_Slot slot_resized;
	CL_Slot slot_quit;
	CL_Slot slot_transform_coords;

	CL_Button *button_quit;
	
	CL_Window *window;
	CL_ComponentMoveHandler move_handler;
	CL_StyleManager_OpenGL *style;
	CL_ResourceManager *resources;
};

#endif
