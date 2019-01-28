/*
	$Id: displaycard_gl_generic.cpp,v 1.15 2001/10/18 09:23:05 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#ifdef USE_OPENGL

#include "displaycard_gl_generic.h"
#include "blit_gl_generic.h"
#include "API/Core/System/cl_assert.h"
#include "API/Application/clanapp.h"

void CL_GL_DisplayCard_Generic::set_palette(CL_Palette *palette)
{
	memcpy(
		this->palette.palette,
		palette->palette,
		palette->num_colors);
}

CL_Palette *CL_GL_DisplayCard_Generic::get_palette()
{
	return &palette;
}

const std::list<CL_VidMode*> &CL_GL_DisplayCard_Generic::get_videomodes()
{
	static std::list<CL_VidMode*> modes;
	return modes;
}

CL_Target *CL_GL_DisplayCard_Generic::get_target()
{
//	cl_info(1, "get_target not working in generic GL impl");
	return NULL;
}

CL_Target *CL_GL_DisplayCard_Generic::get_frontbuffer()
{
//	cl_info(1, "get_frontbuffer not working in generic GL impl");
	return NULL;
}

void CL_GL_DisplayCard_Generic::sync_buffers()
{
//	cl_info(1, "sync not working in generic GL impl... yet");
}

void CL_GL_DisplayCard_Generic::clear_display(
	float red,
	float green,
	float blue,
	float alpha)
{
	glClearColor(red, green, blue, 1-alpha);
	glClear(GL_COLOR_BUFFER_BIT);
}

void CL_GL_DisplayCard_Generic::fill_rect(
	int x1,
	int y1,
	int x2,
	int y2,
	float r,
	float g,
	float b,
	float a)
{
	begin_2d();
	glDisable(GL_TEXTURE_2D);

	glColor4f(r,g,b,a);
	glRectf(x1 + 0.375f, y1 + 0.375f, x2 + 0.375f, y2 + 0.375f);
	glColor4f(1.0, 1.0, 1.0, 1.0);
/*
	glBegin(GL_QUADS);
		glColor4f(r,g,b,a);
		glVertex2i(x1, y1);
		glVertex2i(x2, y1);
		glVertex2i(x2, y2);
		glVertex2i(x1, y2);
		glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnd();
*/
	glEnable(GL_TEXTURE_2D);
	end_2d();
}

void CL_GL_DisplayCard_Generic::draw_rect(
	int x1,
	int y1,
	int x2,
	int y2,
	float r,
	float g,
	float b,
	float a)
{
	begin_2d();

	// Because of buggy OpenGL drivers, I couldn't use LINE_LOOP
	draw_line(x1, y1, x2 - 1, y1, r, g, b, a);
	draw_line(x2 - 1, y1, x2 - 1, y2 - 1, r, g, b, a);
	draw_line(x1 + 1, y2 - 1, x2, y2 - 1, r, g, b, a);
	draw_line(x1, y1 + 1, x1, y2, r, g, b, a);

/*	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINE_STRIP);
		glColor4f(r,g,b,a);
		glVertex2f(x1 + 0.375, y1 + 0.375);
		glVertex2f(x2 + 0.375 - 1, y1 + 0.375);
		glVertex2f(x2 + 0.375 - 1, y2 + 0.375 - 1);
//		glVertex2f(x1 + 0.375, y2 + 0.375 - 1);
		glVertex2f(x1, y2 + 0.375 - 1);
		glVertex2f(x1 + 0.375, y1 + 0.375);
		glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnd();
	glEnable(GL_TEXTURE_2D);
*/
	end_2d();
}

void CL_GL_DisplayCard_Generic::draw_line(
	int x1,
	int y1,
	int x2,
	int y2,
	float r,
	float g,
	float b,
	float a)
{
	begin_2d();
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINES);
		glColor4f(r,g,b,a);
		glVertex2f(x1 + 0.375, y1 + 0.375);
		glVertex2f(x2 + 0.375, y2 + 0.375);
		glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	end_2d();
}

void CL_GL_DisplayCard_Generic::enable_2d_projection()
{
	cl_assert(uses_count == 0); // dont call this function inside a begin_2d/end_2d session.
	use_2d_projection = true;
}

void CL_GL_DisplayCard_Generic::disable_2d_projection()
{
	cl_assert(uses_count == 0); // dont call this function inside a begin_2d/end_2d session.
	use_2d_projection = false;
}

void CL_GL_DisplayCard_Generic::begin_2d()
{
	uses_count--;
	if (uses_count==-1)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);	// todo: narrow range of what is saved

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glDisable(GL_FOG);
		glDisable(GL_DEPTH_TEST);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
		glColor4f(1.0, 1.0, 1.0, 1.0);

		if (use_2d_projection)
		{
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();

			GLdouble viewport[4];
			glGetDoublev(GL_VIEWPORT, viewport);

			GLdouble &width = viewport[2];
			GLdouble &height = viewport[3];

			gluOrtho2D(0.0, width, height, 0.0);
		}

		glMatrixMode(GL_MODELVIEW);
		if (use_2d_projection)
		{
			glPushMatrix();
			glLoadIdentity();
		}

		glPushMatrix();
		glTranslatef(get_translate_offset_x(), get_translate_offset_y(), 0);

		m_uses_gl = false;
	}
}

void CL_GL_DisplayCard_Generic::end_2d()
{
	uses_count++;
	if (uses_count==0)
	{
		glPopMatrix();
		if (use_2d_projection) glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glPopAttrib();

		m_uses_gl = true;
	}
}

CL_Blitters CL_GL_DisplayCard_Generic::create_hw_blitters(CL_SurfaceProvider *provider)
{
	// TODO: Use another blitter if texture is larger than max_texture_size...
	CL_Blit_GLTexture *blitter = new CL_Blit_GLTexture(this, provider);

	CL_Blitters hw;
	hw.set_noclip(blitter);
	hw.set_clip(blitter);
	hw.set_scale_noclip(blitter);
	hw.set_scale_clip(blitter);

	return hw;
}

CL_Blitters CL_GL_DisplayCard_Generic::create_hw_dynamic_blitters(CL_SurfaceProvider *provider)
{
	// TODO: Use another blitter if texture is larger than max_texture_size...
	CL_Blit_GLTexture *blitter = new CL_Blit_GLTexture(this, provider);

	CL_Blitters hw;
	hw.set_noclip(blitter);
	hw.set_clip(blitter);
	hw.set_scale_noclip(blitter);
	hw.set_scale_clip(blitter);

	return hw;
}

void CL_GL_DisplayCard_Generic::push_translate_offset()
{
	translate_stack.push(translate_stack.top());
}

void CL_GL_DisplayCard_Generic::push_translate_offset(int x, int y)
{
	std::pair<int,int> cur = translate_stack.top();
	cur.first += x;
	cur.second += y;

	translate_stack.push(cur);

	if (uses_count != 0)
	{
		glPopMatrix();
		glPushMatrix();
		glTranslatef(get_translate_offset_x(), get_translate_offset_y(), 0);
	}
}

int  CL_GL_DisplayCard_Generic::get_translate_offset_x()
{
	return translate_stack.top().first;
}

int  CL_GL_DisplayCard_Generic::get_translate_offset_y()
{
	return translate_stack.top().second;
}

void CL_GL_DisplayCard_Generic::set_translate_offset(int x, int y)
{
	translate_stack.top() = std::pair<int,int>(x, y);

	if (uses_count != 0)
	{
		glPopMatrix();
		glPushMatrix();
		glTranslatef(get_translate_offset_x(), get_translate_offset_y(), 0);
	}
}

void CL_GL_DisplayCard_Generic::pop_translate_offset()
{
	translate_stack.pop();

	if (uses_count != 0)
	{
		glPopMatrix();
		glPushMatrix();
		glTranslatef(get_translate_offset_x(), get_translate_offset_y(), 0);
	}
}

#endif
