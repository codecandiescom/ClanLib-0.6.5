#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include <ClanLib/GUI/window.h>
#include <ClanLib/Core/Resources/resource_manager.h>
#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Input/input.h>
#include <ClanLib/gl.h>

#include "stylemanager_opengl.h"
#include "window_opengl.h"

CL_Window_OpenGL::CL_Window_OpenGL(
	CL_Window *window,
	CL_StyleManager_OpenGL *style)
:
	CL_ComponentStyle(window),
	window(window),
	move_handler(CL_Rect(18, 0, window->get_width()-18, 20), window, style),
	style(style),
	resources(style->get_resources())
{
	slot_paint = window->sig_paint().connect(this, &CL_Window_OpenGL::on_paint);
	slot_resized = window->sig_resize().connect(this, &CL_Window_OpenGL::on_resized);
	slot_transform_coords = window->sig_transform_coords().connect(this, &CL_Window_OpenGL::on_transform_coords);

	on_resized(window->get_width(), window->get_height());

	button_quit = new CL_Button(CL_Rect(3,3,17,17), "", window);

	slot_quit = button_quit->sig_clicked().connect(this, &CL_Window_OpenGL::on_quit);
}

CL_Window_OpenGL::~CL_Window_OpenGL()
{
}

void CL_Window_OpenGL::on_paint()
{
	CL_OpenGL::begin_2d();

	CL_Display::fill_rect(
		0,
		0,
		window->get_width(),
		window->get_height(),
		0.0f, 0.0f, 1.0f, 0.4f);

	CL_Display::fill_rect(
		1,
		1,
		window->get_width()-1,
		19,
		0.0f, 0.0f, 1.0f, 0.4f);

	CL_Display::draw_line(1, 19, window->get_width()-1, 19, 1, 1, 1, 0.8f);

	CL_Display::draw_line(0, 0, window->get_width()-1, 0, 1, 1, 1, 0.8f);
	CL_Display::draw_line(0, 0, 0, window->get_height()-1, 1, 1, 1, 0.8f);
	CL_Display::draw_line(window->get_width()-1, 0, window->get_width()-1, window->get_height()-1, 1, 1, 1, 0.8f);
	CL_Display::draw_line(0, window->get_height()-1, window->get_width(), window->get_height()-1, 1, 1, 1, 0.8f);

	CL_OpenGL::end_2d();
}

void CL_Window_OpenGL::on_resized(int width, int height)
{
	window->get_client_area()->set_position(CL_Rect(1, 20, width-2, height-21));
//	move_handler.set_move_area(CL_Rect(18, 0, width-18, 20));
}

void CL_Window_OpenGL::on_quit()
{
	window->close();
}

static void solve_matrix(GLdouble matrix[3*2], GLdouble &res_x, GLdouble &res_y)
{
/*
	m00*x + m01*y = m02
	m10*x + m11*y = m12

	(m01-m11*m10/m00)*y = m02 - m12*m10/m00
	y = (m02-m12*m10/m00) / (m01-m11*m10/m00)

    (m00-m10*m11/m01)*x = m02-m12*m11/m01
	x = (m02-m12*m11/m01) / (m00-m10*m11/m01)
*/

	#define M(y,x) matrix[y*3+x]

	res_x = (M(0,2)-M(1,2)*M(1,1)/M(0,1)) / (M(0,0)-M(1,0)*M(1,1)/M(0,1));
	res_y = (M(0,2)-M(1,2)*M(1,0)/M(0,0)) / (M(0,1)-M(1,1)*M(1,0)/M(0,0));
}

void CL_Window_OpenGL::on_transform_coords(int &scr_x, int &scr_y)
{
	return;


	// Project screen coordinates (x,y) down on the gui plane.
	/* A few calculations on paper:

	If you need help to understand this, ask me (mbn) :-)

	(a0, b0, c0) is the first 'enhedsvektor' of the gui plane's coordinate system.
	(a1, b1, c1) is the second 'enhedsvektor'.
	(x0, y0, z0) is the origin of the gui plane.

	(x,y,z,1) is the point before it is transformed into view and proj matrices.

	Vyx (where y and x are numbers, eg. V00) are the values of the combined view/proj matrix.
	(scrX, scrY) is the screen coordinate that we got.
	(tx, ty) are the gui coordinates that we want.

    Qyx is the matrix that needs to be solved to get the gui coords from screen cords.
	Ry is the 'result side' of the matrix that needs to be solved.

	x = x0 + tx*a0 + ty*a1
	y = y0 + tx*b0 + ty*b1
	z = z0 + tx*c0 + ty*c1

					   [ x
						 y
						 z
						 1 ]
	[ V00 V01 V02 V03  [ V00*x + V01*y + V02*z + V03
	  V10 V11 V12 V13    V10*x + V11*y + V12*z + V13
	  V20 V21 V22 V23    V20*x + V21*y + V22*z + V23
	  V30 V31 V32 V33 ]  V30*x + V31*y + V32*z + V33 ]


	scrX - V00*x0 - V01*y0 - V02*z0 - V03
		=
		tx(V00*a0 + V01*b0 + V02*c0) + ty(V00*a1 + V01*b1 + V02*c1)

	scrY - V10*x0 - V11*y0 - V12*z0 - V13
		=
		tx(V10*a0 + V11*b0 + V12*c0) + ty(V10*a1 + V11*b1 + V12*c1)

	Q00 = V00*a0 + V01*b0 + V02*c0
	Q01 = V00*a1 + V01*b1 + V02*c1
	Q10 = V10*a0 + V11*b0 + V12*c0
	Q11 = V10*a1 + V11*b1 + V12*c1
	R0  = scrX - V00*x0 - V01*y0 - V02*z0 - V03
	R1  = scrY - V10*x0 - V11*y0 - V12*z0 - V13

	Using these constants for the plane's coordinates and origin:

	a0 = 1, b0 = 0, c0 = 0
	a1 = 0, b1 = 1, c1 = 0
	x0 = 0, y0 = 0, z0 = 0

	We get:

	Q00 = V00
	Q01 = V01
	Q10 = V10
	Q11 = V11
	R0  = scrX - V03
	R1  = scrY - V13

	Now, when you get a result as simple as that, you hate that you did so
	much calculations (when you could have saved yourself the trouble of
	half of it). Oh well, my linear algebra sucks, okay!? :)
*/

	GLdouble model_matrix[16];
	GLdouble proj_matrix[16];
	GLdouble v_matrix[16];
	GLdouble q_matrix[3*2];

	#define Q(y,x) q_matrix[y*3+x]
	#define R(y) q_matrix[y*3+2]
	#define V(y,x) v_matrix[y*3+y]

	glGetDoublev(GL_MODELVIEW_MATRIX, model_matrix);
	glGetDoublev(GL_PROJECTION_MATRIX, proj_matrix);

	// Multiplay model & project matrices together and store in v_matrix:
	int old_mode;
	glGetIntegerv(GL_MATRIX_MODE, &old_mode);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMultMatrixd(model_matrix);
	glGetDoublev(GL_PROJECTION_MATRIX, v_matrix);
	glPopMatrix();
	glMatrixMode(old_mode);

	// Setup the problem in a matrix:
	Q(0,0) = V(0,0); // V(0,0)*a0 + V(0,1)*b0 + V(0,2)*c0;
	Q(0,1) = V(0,1); // V(0,0)*a1 + V(0,1)*b1 + V(0,2)*c1;
	Q(1,0) = V(1,0); // V(1,0)*a0 + V(1,1)*b0 + V(1,2)*c0;
	Q(1,1) = V(1,1); // V(1,0)*a1 + V(1,1)*b1 + V(1,2)*c1;
	R(0) = scr_x - V(0,3); // scr_x - V(0,0)*x0 - V(0,1)*y0 - V(0,2)*z0 - V(0,3);
	R(1) = scr_y - V(1,3); // scr_y - V(1,0)*x0 - V(1,1)*y0 - V(1,2)*z0 - V(1,3);

	GLdouble plane_x, plane_y;
	solve_matrix(q_matrix, plane_x, plane_y);

	scr_x = (int) (plane_x+0.5);
	scr_y = (int) (plane_y+0.5);

	std::cout << "scr_x: " << scr_x << " scr_y: " << scr_y << std::endl;
}
