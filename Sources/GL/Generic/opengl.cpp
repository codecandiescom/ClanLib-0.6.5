/*
	$Id: opengl.cpp,v 1.7 2002/03/05 21:52:14 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#include "Core/precomp.h"
#include "API/GL/opengl.h"
#include "displaycard_gl_generic.h"
#include "API/Display/Display/display.h"

#include <string>

void CL_OpenGL::begin_2d()
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();

	card->begin_2d();
}

void CL_OpenGL::end_2d()
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();
	
	card->end_2d();
}

void CL_OpenGL::enable_2d_projection()
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();

	card->enable_2d_projection();
}

void CL_OpenGL::disable_2d_projection()
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();
	
	card->disable_2d_projection();
}

void CL_OpenGL::set_use_gl(int v)
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();
	
	card->set_use_gl(v);
}

void CL_OpenGL::set_buffer_size(int v)
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();
	
	card->set_buffer_size(v);
}

void CL_OpenGL::set_level(int v)
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();
	
	card->set_rgba(v);
}

void CL_OpenGL::set_doublebuffer(int v)
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();

      	card->set_doublebuffer(v);
}
	
void CL_OpenGL::set_stereo(int v)
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();
	
      	card->set_stereo(v);
}

void CL_OpenGL::set_aux_buffers(int v)
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();
	
	card->set_aux_buffers(v);
}

void CL_OpenGL::set_red_size(int v)
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();
	
	card->set_red_size(v);
}

void CL_OpenGL::set_green_size(int v)
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();
	
	card->set_blue_size(v);
}

void CL_OpenGL::set_alpha_size(int v)
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();
	
	card->set_alpha_size(v);
}

void CL_OpenGL::set_depth_size(int v)
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();
	
	card->set_depth_size(v);
}

void CL_OpenGL::set_stencil_size(int v)
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();
	
	card->set_stencil_size(v);
}

void CL_OpenGL::set_accum_red_size(int v)
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();
	
	card->set_accum_red_size(v);
}

void CL_OpenGL::set_accum_blue_size(int v)
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();
	
	card->set_accum_blue_size(v);
}

void CL_OpenGL::set_accum_green_size(int v)
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();
	
	card->set_accum_green_size(v);
}

void CL_OpenGL::set_accum_alpha_size(int v)
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();
	
	card->set_accum_alpha_size(v);
}

CL_ProcAddress *CL_OpenGL::get_proc_address(const std::string& function_name)
{
	CL_GL_DisplayCard_Generic *card =
		(CL_GL_DisplayCard_Generic *) CL_Display::get_current_card();
		
	return (CL_ProcAddress *) card->get_proc_address(function_name);
}
