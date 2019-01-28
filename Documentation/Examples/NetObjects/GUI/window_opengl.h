#ifndef header_window_style_opengl
#define header_window_style_opengl

#include <ClanLib/gui.h>

class CL_Window;
class CL_ComponentOptions;
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
	void on_quit();

	CL_Slot slot_paint;
	CL_Slot slot_quit;

	CL_Button *button_quit;
	CL_Window *window;
	CL_ComponentMoveHandler move_handler;
	CL_ResourceManager *resources;
	CL_StyleManager_OpenGL *style;
};

#endif
