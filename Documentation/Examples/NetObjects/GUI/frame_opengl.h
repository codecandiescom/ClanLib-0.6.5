#ifndef header_frame_style_opengl
#define header_frame_style_opengl

#include <ClanLib/gui.h>

class CL_ComponentOptions;
class CL_StyleManager_OpenGL;

class CL_Frame_OpenGL : public CL_ComponentStyle
{
public:
	CL_Frame_OpenGL(
		CL_Frame *frame,
		CL_StyleManager_OpenGL *style);

private:
	void on_draw();

	CL_Frame *frame;
	CL_Slot slot_draw;
	CL_StyleManager_OpenGL *style;
};

#endif
