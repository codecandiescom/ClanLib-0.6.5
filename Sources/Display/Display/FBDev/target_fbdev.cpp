/*
	$Id: target_fbdev.cpp,v 1.2 2001/03/10 17:26:28 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <API/Display/Display/palette.h>
#include <Display/Display/FBDev/target_fbdev.h>
#include "API/Core/System/cl_assert.h"

#include <iostream>
#include <string.h>
#include <math.h>
#include <sys/mman.h>

#include "regs.h"
#include "mmio.h"

CL_Target_FBDev::CL_Target_FBDev(
	bool doublebuffer,
	char *fb,
	unsigned int width,
	unsigned int height,
	fb_var_screeninfo* var,
	volatile unsigned char *mmioaddr )
{
	m_var = var;
	m_fb = fb;
	m_width = width;
	m_height = height;
	m_doublebuffer = doublebuffer;
	m_mmioaddr = mmioaddr;
	
	image_size = m_width*m_height * ((get_depth()+7)/8);

	if (m_doublebuffer)
	{
		buffer = 1;
	} else
	{
		m_skip = get_pitch() - (m_width * ((get_depth()+7)/8));
		image_data = new char[image_size];
	}
}

CL_Target_FBDev::~CL_Target_FBDev()
{
	if (!m_doublebuffer)
	{
		delete image_data;
	}
}

bool CL_Target_FBDev::is_video()
{
	return m_doublebuffer;
}

void CL_Target_FBDev::put_image()
{
	if (m_doublebuffer)
	{
		buffer = !buffer;
	} else
	{
		if (m_skip)
		{
			char* to = m_fb;
			char* from = image_data;
			int line_length = m_width * ((get_depth()+7)/8);
			for (unsigned int y=0; y<m_height; y++)
			{
				memcpy( to, from, line_length );
				to += get_pitch();
				from += line_length;
			}
		} else
		{
			memcpy( m_fb, image_data, image_size );
		}
	}
}

void CL_Target_FBDev::lock()
{
	if (m_mmioaddr)
	{
		mga_waitidle( m_mmioaddr );
	}
}
	
void CL_Target_FBDev::unlock()
{
}

void *CL_Target_FBDev::get_data() const
{
	if (m_doublebuffer)
	{
		return m_fb + buffer*get_pitch()*m_height;
	} else
	{
		return image_data;
	}
}

unsigned int CL_Target_FBDev::get_width() const
{
	return m_width;
}

unsigned int CL_Target_FBDev::get_height() const
{
	return m_height;
}

unsigned int CL_Target_FBDev::get_pitch() const
{
	int bytes_per_pixel = (get_depth()+7)/8;

	return m_var->xres_virtual *bytes_per_pixel;
}
	
unsigned int CL_Target_FBDev::get_depth() const
{
	return 16; //FIXME
}

unsigned int CL_Target_FBDev::get_red_mask() const
{
	return ((int)((pow(2,m_var->red.length))-1) << (m_var->blue.length + m_var->green.length));
}

unsigned int CL_Target_FBDev::get_green_mask() const
{
	return ((int)(pow(2,m_var->green.length))-1) << m_var->blue.length;
}

unsigned int CL_Target_FBDev::get_blue_mask() const
{
	return (int)(pow(2,m_var->blue.length))-1;
}

bool CL_Target_FBDev::is_indexed() const
{
	return false;
}

unsigned int CL_Target_FBDev::get_alpha_mask() const
{
	return 0;
}

CL_Palette *CL_Target_FBDev::get_palette() const
{
	return NULL;
}
