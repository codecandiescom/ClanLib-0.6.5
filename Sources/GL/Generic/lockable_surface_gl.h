/*
	$Id: lockable_surface_gl.h,v 1.1.1.1 2000/04/09 12:18:02 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_dynamic_surface_gl
#define header_dynamic_surface_gl

/*
#ifdef WIN32
	#include <windows.h>
#endif
*/
#include <GL/gl.h>
#include "API/Core/Display/surface.h"

class CL_GL_DisplayCard_Generic;
class CL_DynamicSurface_GL : public CL_Surface
{
public:
	CL_LockableSurface_GL(
		int width,
		int height,
		int no_sprs,
		int minimum_alpha_mask_size,
		CL_GL_DisplayCard_Generic *card);

	virtual ~CL_LockableSurface_GL();

	// From CL_Target:
	// ---------------
	virtual void lock() { return; }
	virtual void unlock();

	virtual void *get_data() { return m_data; }

	virtual unsigned int get_width() { return m_width; }
	virtual unsigned int get_height() { return m_height; }
	virtual unsigned int get_pitch() { return m_width*sizeof(int); }

	virtual unsigned int get_depth() { return sizeof(int)*8; }

	virtual unsigned int get_red_mask() { return 0xff000000; }
	virtual unsigned int get_green_mask() { return 0x00ff0000; }
	virtual unsigned int get_blue_mask() { return 0x0000ff00; }
	virtual unsigned int get_alpha_mask() { return m_alpha_mask; }

	virtual CL_Palette *get_palette() { return NULL; }

	// From CL_LockableSurface:
	// ------------------------
	virtual void set_colorkey(int colorkey);
	virtual int get_colorkey() const;

	virtual unsigned int get_no_sprs() const;

	virtual void put_screen(int x, int y, int spr_no);
	virtual void put_screen(int x, int y, float scale_x, float scale_y, int spr_no);
	virtual void put_screen(int x, int y, int size_x, int size_y, int spr_no);

private:
	int *m_data;
	int m_colorkey, 
	    m_no_sprs, 
	    m_width, 
	    m_height,
	    m_alpha_mask,
	    m_texture_width,
	    m_texture_height;

	GLuint m_texture;

	CL_GL_DisplayCard_Generic *m_card;
};

#endif
