/*
	$Id: target_ximage_shm.cpp,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include "API/Core/System/cl_assert.h"
#include <API/Core/System/error.h>
#include <API/Display/Display/palette.h>
#include <Display/Display/X11/target_ximage_shm.h>
#include <API/Core/Math/rect.h>

#include <iostream>
#include <string.h>

static int (*cl_shm_oldhandler)(Display *, XErrorEvent *);
static bool cl_shm_error = false;
static int cl_shm_errorhandler(Display *disp,XErrorEvent *xev)
{
	if (xev->error_code == BadAccess)
	{
		cl_shm_error = true;
		return 0;
	}
	else
	{
		return ((*cl_shm_oldhandler)(disp,xev));
	}
}

CL_Target_XImage_Shm::CL_Target_XImage_Shm(
	XVisualInfo visual_info,
	Display *dpy,
	unsigned int width,
	unsigned int height)
{
	m_visual_info = visual_info;
	m_dpy = dpy;
	m_width = width;
	m_height = height;
	
	if (XShmQueryExtension(dpy) == False)
	// does we have the extension at all?
	{
		throw CL_Error("Cannot use shared memory ximage backbuffer");
	}
	
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

	// Allocate memory:
	int image_size = get_pitch()*height;
	
	m_shm_image = XShmCreateImage(
		dpy,
		visual_info.visual,
		visual_info.depth, // PTC does this: DefaultDepth(dpy,0),
		ZPixmap,
		NULL,
		&m_shminfo,
		width,
		height);
	
	cl_assert(m_shm_image != NULL);

	// Create shared memory segment:
	m_shminfo.shmid = shmget(IPC_PRIVATE, image_size, IPC_CREAT|0777);
	cl_assert(m_shminfo.shmid >= 0);
	
	// Get memory address to segment:
	m_shminfo.shmaddr = (char *) shmat(m_shminfo.shmid, 0, 0);

	// Catch access errors:
	cl_shm_oldhandler = XSetErrorHandler(cl_shm_errorhandler);
	
	// Tell XServer that it may only read from it and attach to display:
	m_shminfo.readOnly = True;

	// "Trick" to ensure that if client dies the shared memory segment is removed 
	shmctl(m_shminfo.shmid,IPC_RMID,0);

	XShmAttach (dpy, &m_shminfo);

	// Fill the XImage struct:
	m_shm_image->data = m_shminfo.shmaddr;

	XSync(m_dpy, 0);
	XSetErrorHandler(cl_shm_oldhandler);

	if (cl_shm_error)
	{
		throw CL_Error("Cannot use shared memory ximage backbuffer");
	}
}
		
CL_Target_XImage_Shm::~CL_Target_XImage_Shm()
{
	XDestroyImage(m_shm_image);

	// Detach shm image from display:
	XShmDetach (m_dpy, &m_shminfo);
	
	// Clean up shared memory:
	shmdt(m_shminfo.shmaddr);
	shmctl(m_shminfo.shmid, IPC_RMID, 0);
}

void CL_Target_XImage_Shm::put_image(int x, int y, Drawable win, GC gc)
{
	XShmPutImage(
		m_dpy,
		win,
		gc,
		m_shm_image,
		0, 0,
		x, y,
		m_width,
		m_height,
		False);
}

void CL_Target_XImage_Shm::put_image(
	int x, int y,
	const class CL_Rect &rect,
	Drawable win, GC gc)
{
	XShmPutImage(
		m_dpy,
		win,
		gc,
		m_shm_image,
		rect.x1, rect.y1,
		x, y,
		rect.x2 - rect.x1,
		rect.y2 - rect.y1,
		False);
}

void CL_Target_XImage_Shm::lock()
{
//	return true;
}
	
void CL_Target_XImage_Shm::unlock()
{
}

void *CL_Target_XImage_Shm::get_data() const
{
	return m_shm_image->data;
}

unsigned int CL_Target_XImage_Shm::get_width() const
{
	return m_width;
}

bool CL_Target_XImage_Shm::is_indexed() const
{
	return false;
}

unsigned int CL_Target_XImage_Shm::get_height() const
{
	return m_height;
}

unsigned int CL_Target_XImage_Shm::get_pitch() const
{
	return m_pitch;
}
	
unsigned int CL_Target_XImage_Shm::get_depth() const
{
	return m_depth;
}

unsigned int CL_Target_XImage_Shm::get_red_mask() const
{
	return m_visual_info.red_mask;
}

unsigned int CL_Target_XImage_Shm::get_green_mask() const
{
	return m_visual_info.green_mask;
}

unsigned int CL_Target_XImage_Shm::get_blue_mask() const
{
	return m_visual_info.blue_mask;
}

unsigned int CL_Target_XImage_Shm::get_alpha_mask() const
{
//	return m_visual_info.alpha_mask; // no alpha mask in visualinfo struct!?
	return 0;
}

CL_Palette *CL_Target_XImage_Shm::get_palette() const
{
	return NULL;
}
