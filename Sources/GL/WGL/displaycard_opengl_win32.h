/*
	$Id: displaycard_opengl_win32.h,v 1.5 2002/02/16 16:10:42 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_displaycard_opengl_win32
#define header_displaycard_opengl_win32

#ifdef USE_OPENGL

#include "GL/Generic/displaycard_gl_generic.h"
#include <GL/gl.h>
#include <GL/glu.h>

class CL_OpenGL_Initializer;

class CL_DisplayCard_OpenGL_Win32 : public CL_GL_DisplayCard_Generic
{
public:
	CL_DisplayCard_OpenGL_Win32(int card_no);
	virtual ~CL_DisplayCard_OpenGL_Win32();

	static void __cdecl add_display();

	// CL_GL_DisplayCard_Generic
	// -------------------------
	
	virtual int get_max_texture_size() { return max_texture_size; }

	// CL_DisplayCard functions:
	// -------------------------

	virtual void flip_display(bool sync=false);
	virtual void put_display(const class CL_Rect &rect);
	virtual void set_videomode(int width, int height, int bpp, bool fullscreen, bool allow_resize, bool video_memory);
	
	virtual void (*get_proc_address(const std::string& function_name))();

	virtual std::string get_name() { return "opengl"; }
	
private:
	friend class CL_OpenGL_Initializer;
//	bool initialized;

	static HGLRC context;
	static HDC hdc;

	GLint max_texture_size;
};

#endif // USE_OPENGL

#endif
