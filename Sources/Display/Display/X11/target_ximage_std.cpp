/*
	$Id: target_ximage_std.cpp,v 1.1 2001/03/06 15:09:19 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include "API/Core/System/cl_assert.h"
#include <API/Display/Display/palette.h>
#include <Display/Display/X11/target_ximage_std.h>
#include <API/Core/Math/rect.h>

#include <iostream>
#include <string.h>

CL_Target_XImage_Std::CL_Target_XImage_Std(
	XVisualInfo visual_info,
	Display *dpy,
	unsigned int width,
	unsigned int height)
{
	m_visual_info = visual_info;
	m_dpy = dpy;
	m_width = width;
	m_height = height;

	// Figure out what the depth and pitch is:
	int num_formats;
	XPixmapFormatValues *formats = XListPixmapFormats(m_dpy, &num_formats);
	for (int i=0; i<num_formats; i++)
	{
		if (formats[i].depth == m_visual_info.depth)
		{
			m_depth = formats[i].bits_per_pixel;
			m_pitch = (width*m_depth+formats[i].scanline_pad-1)/formats[i].scanline_pad;
			m_pitch *= formats[i].scanline_pad;
			m_pitch = (m_pitch+7)/8;
			break;
		}
	}
	XFree(formats);

	// Create XImage:
	int image_size = get_pitch()*height;
	char *image_data = new char[image_size];
	
	m_std_image = XCreateImage(
		dpy,
		visual_info.visual,
		visual_info.depth, // PTC does this: DefaultDepth(dpy,0),
		ZPixmap,
		0,
		image_data,
		width,
		height,
		8, // bitmap_pad
		0);
	
	cl_assert(m_std_image != NULL);
	
	m_delete_ximage = true;
}

CL_Target_XImage_Std::CL_Target_XImage_Std(
	XVisualInfo visual_info,
	Display *dpy,
	unsigned int width,
	unsigned int height,
	XImage *image,
	bool delete_ximage)
{
	m_visual_info = visual_info;
	m_dpy = dpy;
	m_width = width;
	m_height = height;
	
	m_std_image = image;
	m_delete_ximage = delete_ximage;
}
		
CL_Target_XImage_Std::~CL_Target_XImage_Std()
{
	if (m_delete_ximage)
		XDestroyImage(m_std_image);
}

void CL_Target_XImage_Std::put_image(int x, int y, Drawable win, GC gc)
{
	XPutImage(
		m_dpy,
		win,
		gc,
		m_std_image,
		0, 0,
		x, y,
		m_width,
		m_height);
}

void CL_Target_XImage_Std::put_image(
	int x, int y,
	const class CL_Rect &rect,
	Drawable win, GC gc)
{
	XPutImage(
		m_dpy,
		win,
		gc,
		m_std_image,
		rect.x1, rect.y1,
		x, y,
		rect.x2 - rect.x1,
		rect.y2 - rect.y1);
}

void CL_Target_XImage_Std::lock()
{
//	return true;
}
	
void CL_Target_XImage_Std::unlock()
{
}

void *CL_Target_XImage_Std::get_data() const
{
	return m_std_image->data;
}

unsigned int CL_Target_XImage_Std::get_width() const
{
	return m_width;
}

bool CL_Target_XImage_Std::is_indexed() const
{
	return false;
}

unsigned int CL_Target_XImage_Std::get_height() const
{
	return m_height;
}

unsigned int CL_Target_XImage_Std::get_pitch() const
{
	int bytes_per_pixel = (get_depth()+7)/8;

	return m_width*bytes_per_pixel;
}
	
unsigned int CL_Target_XImage_Std::get_depth() const
{
	int hack = (m_visual_info.bits_per_rgb+7)/8;
	return hack*8;
}

unsigned int CL_Target_XImage_Std::get_red_mask() const
{
	return m_visual_info.red_mask;
}

unsigned int CL_Target_XImage_Std::get_green_mask() const
{
	return m_visual_info.green_mask;
}

unsigned int CL_Target_XImage_Std::get_blue_mask() const
{
	return m_visual_info.blue_mask;
}

unsigned int CL_Target_XImage_Std::get_alpha_mask() const
{
//	return m_visual_info.alpha_mask; // no alpha mask in visualinfo struct!?
	return 0;
}

CL_Palette *CL_Target_XImage_Std::get_palette() const
{
	return NULL;
}
