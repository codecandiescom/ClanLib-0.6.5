#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include "frame_opengl.h"
#include "stylemanager_opengl.h"

CL_Frame_OpenGL::CL_Frame_OpenGL(
	CL_Frame *frame,
	CL_StyleManager_OpenGL *style)
:
	CL_ComponentStyle(frame),
	frame(frame),
	style(style)
{
	slot_draw = frame->sig_paint().connect(this, &CL_Frame_OpenGL::on_draw);
}

void CL_Frame_OpenGL::on_draw()
{
	CL_Display::clear_display(108.0f/255.0f, 139.0f/255.0f, 138.0f/255.0f);

/*	int width = frame->get_width();
	int height = frame->get_height();

	style->fill_rect(
		0, 0, width, height,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.05f, 0.26f, 0.28f,
		0.05f, 0.26f, 0.28f,
		0.9f);
*/
}
