#ifndef header_scrollbar_style_opengl
#define header_scrollbar_style_opengl

#include <ClanLib/gui.h>
#include <ClanLib/core.h>

class CL_ScrollBar;
class CL_ComponentOptions;
class CL_StyleManager_OpenGL;
class CL_ResourceManager;

class CL_ScrollBar_OpenGL : public CL_ComponentStyle
{
public:
	CL_ScrollBar_OpenGL(
		CL_ScrollBar *scrollbar,
		CL_StyleManager_OpenGL *style);
	
	virtual ~CL_ScrollBar_OpenGL();

private:
	void start_scroll(int delta);
	void stop_scroll();

	void on_paint();
	void on_timer_scroll();

	CL_Slot slot_paint;
	CL_Slot slot_increase_pressed;
	CL_Slot slot_decrease_pressed;
	CL_Slot slot_increase_released;
	CL_Slot slot_decrease_released;
	CL_Slot slot_timer;

	CL_Timer timer_scroll;
	int scroll_delta;

	CL_ScrollBar *scrollbar;
	CL_LayoutManager layout;

	CL_Button *button_increase;
	CL_Button *button_decrease;

	CL_StyleManager_OpenGL *style;
	CL_ResourceManager *resources;
	CL_Surface *sur_increase_normal;
	CL_Surface *sur_increase_toggled;
	CL_Surface *sur_decrease_normal;
	CL_Surface *sur_decrease_toggled;
};

#endif

