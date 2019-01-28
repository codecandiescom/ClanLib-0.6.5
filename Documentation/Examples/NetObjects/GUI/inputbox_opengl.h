#ifndef header_inputbox_style_opengl
#define header_inputbox_style_opengl

#include <ClanLib/gui.h>

class CL_Font;
class CL_InputBox;
class CL_StyleManager_OpenGL;

class CL_InputBox_OpenGL : public CL_ComponentStyle
{
public:
	CL_InputBox_OpenGL(
		CL_InputBox *inputbox,
		CL_StyleManager_OpenGL *style);
	
	virtual ~CL_InputBox_OpenGL();

private:
	CL_Slot slot_paint;
	void on_paint();
	void on_paint_texture();
	void on_paint_default();
	void on_paint_input();

	CL_InputBox *inputbox;

	CL_StyleManager_OpenGL *style;
	CL_ResourceManager *resources;
	CL_Font *font;

	int character_offset;

	bool show_cursor;
	unsigned int cursor_blink_time;

	CL_Surface *surface_left;
	CL_Surface *surface_center;
	CL_Surface *surface_right;
};

#endif
