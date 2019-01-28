/*
	$Id: blit_gl_dynamic.h,v 1.1.1.1 2000/04/09 12:18:02 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_blit_gl_dynamic
#define header_blit_gl_dynamic

#ifdef USE_OPENGL

#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include "Core/Display/Generic/blitters.h"
#include "displaycard_gl_generic.h"

class CL_Blit_GLTexture_Dynamic :
	public CL_Blit_NoClip,
	public CL_Blit_Clip,
	public CL_Blit_Scale_NoClip,
	public CL_Blit_Scale_Clip
{
public:
	CL_Blit_GLTexture_Dynamic(
		CL_GL_DisplayCard_Generic *card,
		CL_SurfaceProvider *surf);

	virtual ~CL_Blit_GLTexture_Dynamic();

	virtual void blt_noclip(
		CL_Target *target,
		int x,
		int y,
		int spr_no);

	virtual void blt_clip(
		CL_Target *target,
		int x,
		int y,
		int spr_no,
		const CL_ClipRect &clip);

	virtual void blt_scale_noclip(
		CL_Target *target,
		int x,
		int y,
		int dest_width,
		int dest_height,
		int spr_no);

	virtual void blt_scale_clip(
		CL_Target *target,
		int x,
		int y,
		int dest_width,
		int dest_height,
		int spr_no,
		const CL_ClipRect &clip);

private:
	CL_GL_DisplayCard_Generic *card;

	int width, height, no_sprs;
	int texture_width, texture_height;

	CL_SurfaceProvider* provider;
    
	GLuint texture;
};

#endif
#endif
