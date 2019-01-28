/*
	$Id: target_ggi_put.cpp,v 1.2 2001/03/17 23:53:24 vaidhy Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_GGI

#include <API/Display/Display/palette.h>
#include <Display/Display/GGI/target_ggi.h>
#include <Display/Display/GGI/target_ggi_put.h>
#include "API/Core/System/cl_assert.h"
#include "Core/System/Generic/string_asm.h"

#include <iostream>
#include <string.h>
#include <math.h>
#include <sys/mman.h>

CL_Target_GGI_Put::CL_Target_GGI_Put(ggi_visual_t vis) : CL_Target_GGI(vis)
{
	m_pixelformat = ggiGetPixelFormat(m_vis);

	image_size = m_mode.virt.x * m_mode.virt.y * GT_DEPTH(m_mode.graphtype);
	image_data = new char[image_size];
}

CL_Target_GGI_Put::~CL_Target_GGI_Put()
{
	delete ((char*) image_data);
}

void CL_Target_GGI_Put::lock()
{
//	return image_data!=NULL;
}
	
void CL_Target_GGI_Put::unlock()
{
}

void *CL_Target_GGI_Put::get_data() const
{
	return image_data;
}

void CL_Target_GGI_Put::to_front()
{
	ggiPutBox( m_vis, 0, 0, m_mode.virt.x, m_mode.virt.x, image_data );
	ggiFlush( m_vis );
}

void CL_Target_GGI_Put::to_back()
{
}

unsigned int CL_Target_GGI_Put::get_width() const
{
	return m_mode.virt.x;
}

unsigned int CL_Target_GGI_Put::get_height() const
{
	return m_mode.virt.y;
}

unsigned int CL_Target_GGI_Put::get_pitch() const
{
	return m_mode.virt.x * ((get_depth()+7)/8);
}

unsigned int CL_Target_GGI_Put::get_depth() const
{
	return GT_DEPTH(m_mode.graphtype);
}

unsigned int CL_Target_GGI_Put::get_red_mask() const
{
	return m_pixelformat->red_mask;
}

unsigned int CL_Target_GGI_Put::get_green_mask() const
{
	return m_pixelformat->green_mask;
}

unsigned int CL_Target_GGI_Put::get_blue_mask() const
{
	return m_pixelformat->blue_mask;
}

unsigned int CL_Target_GGI_Put::get_alpha_mask() const
{
	return m_pixelformat->alpha_mask;
}

CL_Palette *CL_Target_GGI_Put::get_palette() const
{
	return (CL_Palette *) &m_palette;
}

#endif
