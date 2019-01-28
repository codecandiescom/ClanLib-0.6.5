/*
	$Id: lockable_surface_gl.cpp,v 1.3 2000/05/03 18:29:00 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
// THIS IS OUTDATED CRAP, IT'S ONLY PURPOSE HERE IS THAT I CAN COPY & PASTE CODE
// FOR THE NEW DYNAMIC SURFACES

#include "Core/precomp.h"

#ifdef VOGEL_VOGEL_VOGEL

#ifdef USE_OPENGL
#include "displaycard_gl_generic.h"
#include "lockable_surface_gl.h"
#include "API/Core/System/cl_assert.h"
#include "Core/Display/Generic/displaycard_generic.h"

CL_LockableSurface_GL::CL_LockableSurface_GL(
	int width,
	int height,
	int no_sprs,
	int minimum_alpha_mask_size,
	CL_GL_DisplayCard_Generic *card)
{
	m_width = width;
	m_height = height;
	m_no_sprs = no_sprs;
	m_colorkey = -1;
	m_card = card;

	if (minimum_alpha_mask_size > 0) m_alpha_mask = 0x000000ff;
	else m_alpha_mask = 0;
	
	m_data = new int[m_width*m_height*m_no_sprs];
	
//	TODO: use TEXTURE PROXY

	m_texture_width = 2;
	while (m_texture_width<m_width)
		m_texture_width *= 2;

	m_texture_height = 2;
	while (m_texture_height<m_height*m_no_sprs)
		m_texture_height *= 2;
		
	int* dummy = new int[m_texture_width*m_texture_height];		

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		m_texture_width,
		m_texture_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		dummy);

	delete[] dummy;
}

CL_LockableSurface_GL::~CL_LockableSurface_GL()
{
	glDeleteTextures(1,&m_texture);
	delete[] m_data;
}

#ifdef VOGEL_IF_IT_IS_OUTDATED_THEN_REMOVE_IT_GODDAMN_YEAH

void CL_LockableSurface_GL::unlock()
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexSubImage2D(
		GL_TEXTURE_2D,
		0,
		0,
		0,
		m_width,
		m_height,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		m_data);			
}

void CL_LockableSurface_GL::set_colorkey(int colorkey)
{
	m_colorkey = colorkey;
}

int CL_LockableSurface_GL::get_colorkey() const
{
	return m_colorkey;
}

unsigned int CL_LockableSurface_GL::get_no_sprs() const
{
	return m_no_sprs;
}

void CL_LockableSurface_GL::put_screen(
	int x,
	int y,
	int spr_no)
{
	put_screen(x, y, (int) m_width, (int) m_height, spr_no);
}

void CL_LockableSurface_GL::put_screen(
	int x,
	int y,
	float scale_x,
	float scale_y,
	int spr_no)
{
	put_screen(x, y, (int) (m_width * scale_x), (int) (m_height * scale_y), spr_no);
}

void CL_LockableSurface_GL::put_screen(
	int x,
	int y,
	int size_x,
	int size_y,
	int spr_no)
{
	if (m_colorkey == -1)
	{
		if (m_card->uses_gl())
		{
			cl_assert(false);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
			float h1 = ((float) m_height * spr_no) / m_texture_height;
			float h2 = ((float) m_height * (spr_no+1)) / m_texture_height;
			float w = (float) m_width / m_texture_width;
			glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2f(0.0, h1); glVertex2i(x, y);
				glTexCoord2f(  w, h1); glVertex2i(x + size_x, y);
				glTexCoord2f(0.0, h2); glVertex2i(x, y + size_y);
				glTexCoord2f(  w, h2); glVertex2i(x + size_x, y + size_y);
			glEnd();
			
		}
	}
	else
	{
		cl_info(info_display, "GL impl. cannot blit colorkey lockable surfaces... yet");
	}
}

#endif //USE_OPENGL
*/

#endif // VOGEL_VOGEL_VOGEL
#endif
