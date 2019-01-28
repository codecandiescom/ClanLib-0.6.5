/*
	$Id: blit_gl_dynamic.cpp,v 1.2 2001/04/12 19:58:17 vaidhy Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#ifdef USE_OPENGL

#include "blit_gl_generic.h"
#include "API/Display/Display/pixeldata.h"

// this can be deleted if the impl. is stable
void check4error()
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR)
		std::cout << gluErrorString(error) << std::endl;
}

CL_Blit_GLTexture::CL_Blit_GLTexture(
	CL_GL_DisplayCard_Generic *card,
	CL_SurfaceProvider *provider)
{
	card->begin_2d();

	this->card = card;
	this->provider = provider;

	provider->lock();
	
	width = provider->get_width();
	height = provider->get_height();
	no_sprs = provider->get_num_frames();

	int texture_size = 1;
	while (texture_size < width || texture_size < height*no_sprs) texture_size *= 2;

	texture_width = texture_size;
	texture_height = texture_size;

	cl_assert(width <= texture_width);
	cl_assert(height <= texture_height);

	unsigned int *texture_data = new unsigned int[texture_width*texture_height];
	memset(texture_data, 0, texture_width*texture_height*sizeof(int));

	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// maybe doesn't work on Linux

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		texture_width,
		texture_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		texture_data);

	delete[] texture_data;

	card->end_2d();
}

CL_Blit_GLTexture::~CL_Blit_GLTexture()
{
	provider->unlock;
	glDeleteTextures(1,&texture);
}

void CL_Blit_GLTexture::blt_noclip(
	CL_Target *target,
	int x,
	int y,
	int spr_no)
{
	card->begin_2d();

	//check4error();
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexSubImage2D(
		GL_TEXTURE_2D,
		0,
		0,
		0,
		m_width,
		m_height,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		provider->get_data());

	float h1 = ((float) height * spr_no) / texture_height;
	float h2 = ((float) height * (spr_no+1)) / texture_height;
	float w = (float) width / texture_width;
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0, h1); 	glVertex2i(x, y);
		glTexCoord2f(  w, h1); 	glVertex2i(x + width, y);
		glTexCoord2f(0.0, h2); 	glVertex2i(x, y + height);
		glTexCoord2f(  w, h2); 	glVertex2i(x + width, y + height);
	glEnd();	
	
	card->end_2d();
}

void CL_Blit_GLTexture::blt_clip(
	CL_Target *target,
	int x,
	int y,
	int spr_no,
	const CL_ClipRect &clip)
{
	// TODO: use glScissor()
	blt_noclip(target,x,y,spr_no); // this will do for now.
}

void CL_Blit_GLTexture::blt_scale_noclip(
	CL_Target *target,
	int x,
	int y,
	int dest_width,
	int dest_height,
	int spr_no)
{
	card->begin_2d();

	glBindTexture(GL_TEXTURE_2D, texture);
	float h1 = ((float) height * spr_no) / texture_height;
	float h2 = ((float) height * (spr_no+1)) / texture_height;
	float w = (float) width / texture_width;
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0, h1); glVertex2i(x, y);
		glTexCoord2f(  w, h1); glVertex2i(x + dest_width, y);
		glTexCoord2f(0.0, h2); glVertex2i(x, y + dest_height);
		glTexCoord2f(  w, h2); glVertex2i(x + dest_width, y + dest_height);
	glEnd();

	card->end_2d();	
}

void CL_Blit_GLTexture::blt_scale_clip(
	CL_Target *target,
	int x,
	int y,
	int dest_width,
	int dest_height,
	int spr_no,
	const CL_ClipRect &clip)
{
	// TODO: use glScissor() or rather calclute the other values
	blt_scale_noclip(target, x, y, dest_width, dest_height, spr_no);
}

#endif
