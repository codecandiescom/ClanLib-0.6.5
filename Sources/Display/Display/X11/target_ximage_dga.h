/*
	$Id: target_ximage_dga.h,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_target_ximage_dga
#define header_target_ximage_dga

#ifdef USE_X11

#include "target_ximage.h"

// #include <X11/extensions/XDGA.h>

class CL_Target_XImage_DGA : public CL_Target_XImage
{
public:
	CL_Target_XImage_DGA(
		XVisualInfo visual_info,
		Display *dpy,
		unsigned int width,
		unsigned int height);
		
	virtual ~CL_Target_XImage_DGA();

	virtual void lock();
	virtual void unlock();

	virtual void *get_data() const;

	virtual unsigned int get_width() const;
	virtual unsigned int get_height() const;
	virtual unsigned int get_pitch() const;
	
	virtual unsigned int get_depth() const;
	virtual unsigned int get_red_mask() const;
	virtual unsigned int get_green_mask() const;
	virtual unsigned int get_blue_mask() const;
	virtual unsigned int get_alpha_mask() const;
	virtual bool is_indexed() const;
		
	virtual unsigned int get_num_frames() const { return 1; }

	virtual CL_Palette *get_palette() const;
	
	virtual void put_image(int x, int y, Drawable win, GC gc);
	virtual void put_image(int x, int y, const class CL_Rect &rect, Drawable win, GC gc);
	
protected:
	XVisualInfo m_visual_info;
	Display *m_dpy;
	
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_depth, m_pitch;

	XImage *m_dga_image;
};

#endif /*USE_X11*/

#endif
