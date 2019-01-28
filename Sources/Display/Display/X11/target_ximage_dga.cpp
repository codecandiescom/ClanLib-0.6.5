/*
	$Id: target_ximage_dga.cpp,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <API/Display/Display/palette.h>
#include <Display/Display/X11/target_ximage_dga.h>
#include "API/Core/System/cl_assert.h"

#include <iostream>
#include <string.h>

CL_Target_XImage_DGA::CL_Target_XImage_DGA(
	XVisualInfo visual_info,
	Display *dpy,
	unsigned int width,
	unsigned int height)
{
/*	m_visual_info = visual_info;
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
	
	// Create image:
	int image_size = get_pitch()*height;
	
	m_dga_image = XDGACreateImage(
		dpy,
		visual_info.visual,
		visual_info.depth, // PTC does this: DefaultDepth(dpy,0),
		ZPixmap,
		NULL,
		&m_dgainfo,
		width,
		height);
	
	cl_assert(m_dga_image != NULL);

	// Create shared memory segment:
	m_dgainfo.dgaid = dgaget(IPC_PRIVATE, image_size, IPC_CREAT|0777);
	cl_assert(m_dgainfo.dgaid >= 0);
	
	// Get memory address to segment:
	m_dgainfo.dgaaddr = (char *) dgaat(m_dgainfo.dgaid, 0, 0);
	
	// Tell XServer that it may only read from it and attach to display:
	m_dgainfo.readOnly = True;
	XDGAAttach (dpy, &m_dgainfo);

	// Fill the XImage struct:
	m_dga_image->data = m_dgainfo.dgaaddr;*/
}
		
CL_Target_XImage_DGA::~CL_Target_XImage_DGA()
{
/*	XDestroyImage(m_dga_image);

	// Detach dga image from display:
	XDGADetach (m_dpy, &m_dgainfo);
	
	// Clean up shared memory:
	dgadt(m_dgainfo.dgaaddr);
	dgactl(m_dgainfo.dgaid, IPC_RMID, 0);*/
}

void CL_Target_XImage_DGA::put_image(int x, int y, Drawable win, GC gc)
{
/*	XDGAPutImage(
		m_dpy,
		win,
		gc,
		m_dga_image,
		0, 0,
		x, y,
		m_width,
		m_height,
		False);*/
}

void CL_Target_XImage_DGA::put_image(
	int x, int y,
	const class CL_Rect &rect,
	Drawable win, GC gc)
{
}

void CL_Target_XImage_DGA::lock()
{
//	return true;
}
	
void CL_Target_XImage_DGA::unlock()
{
}

void *CL_Target_XImage_DGA::get_data() const
{
	return m_dga_image->data;
}

unsigned int CL_Target_XImage_DGA::get_width() const
{
	return m_width;
}

unsigned int CL_Target_XImage_DGA::get_height() const
{
	return m_height;
}

unsigned int CL_Target_XImage_DGA::get_pitch() const
{
	return m_pitch;
}
	
unsigned int CL_Target_XImage_DGA::get_depth() const
{
	return m_depth;
}

unsigned int CL_Target_XImage_DGA::get_red_mask() const
{
	return m_visual_info.red_mask;
}

unsigned int CL_Target_XImage_DGA::get_green_mask() const
{
	return m_visual_info.green_mask;
}

unsigned int CL_Target_XImage_DGA::get_blue_mask() const
{
	return m_visual_info.blue_mask;
}

bool CL_Target_XImage_DGA::is_indexed() const
{
	return false;
}

unsigned int CL_Target_XImage_DGA::get_alpha_mask() const
{
//	return m_visual_info.alpha_mask; // no alpha mask in visualinfo struct!?
	return 0;
}

CL_Palette *CL_Target_XImage_DGA::get_palette() const
{
	return NULL;
}
