#ifndef header_button_style_opengl
#define header_button_style_opengl

#include <ClanLib/gui.h>
#include <ClanLib/core.h>

class CL_Button;
class CL_StyleManager_OpenGL;
class CL_ResourceManager;
class CL_Font;

class CL_Button_OpenGL : public CL_ComponentStyle
{
public:
	CL_Button_OpenGL(
		CL_Button *button,
		CL_StyleManager_OpenGL *style);

	virtual ~CL_Button_OpenGL();

private:
	CL_Slot slot_paint;
	void on_paint();
	void on_paint_texture();
	void on_paint_anim();

	CL_Slot slot_mouse_enter;
	void on_mouse_enter();

	CL_Slot slot_mouse_leave;
	void on_mouse_leave();

	CL_Button *button;
	
	bool highlighted;

	int highlightSizeUpper;
	int highlightSizeLower;

	float highlightUpperRGB[3];
	float highlightLowerRGB[3];
	float highlightUpperDeltaRGB[3];
	float highlightLowerDeltaRGB[3];

	CL_Timer timer;
	CL_Slot slot_timer;
	void on_timer();

	CL_StyleManager_OpenGL *style;
	CL_ResourceManager *resources;

	CL_Font *font;

	CL_Surface *surface_up_left;
	CL_Surface *surface_up_center;
	CL_Surface *surface_up_right;

	CL_Surface *surface_down_left;
	CL_Surface *surface_down_center;
	CL_Surface *surface_down_right;
};

#endif
