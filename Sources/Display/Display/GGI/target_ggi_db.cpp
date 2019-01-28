/*
	$Id: target_ggi_db.cpp,v 1.2 2001/03/17 23:53:24 vaidhy Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_GGI

#include <API/Core/System/error.h>
#include <API/Display/Display/palette.h>
#include <Display/Display/GGI/target_ggi.h>
#include <Display/Display/GGI/target_ggi_db.h>
#include "API/Core/System/cl_assert.h"
#include "Core/System/Generic/string_asm.h"

#include <iostream>
#include <string.h>
#include <math.h>
#include <sys/mman.h>

CL_Target_GGI_DB::CL_Target_GGI_DB(ggi_visual_t vis, int frame) : CL_Target_GGI(vis)
{
	m_frame = frame;

	if (! (m_db = ggiDBGetBuffer (vis, frame)))
	{
		throw CL_Error("CL_Target_GGI: No DirectBuffer available.");
	}

	if (! (m_db->type & GGI_DB_SIMPLE_PLB))
	{
		throw CL_Error("CL_Target_GGI: non-standard display buffer!");
	}
}

CL_Target_GGI_DB::~CL_Target_GGI_DB()
{
}

void CL_Target_GGI_DB::lock()
{
	if (ggiResourceAcquire( m_db->resource, GGI_ACTYPE_WRITE ))
	{
		throw CL_Error("CL_Target_GGI: Error acquiring DirectBuffer");
	}
	m_fb = m_db->write;
}
	
void CL_Target_GGI_DB::unlock()
{
	ggiResourceRelease( m_db->resource );
}

void *CL_Target_GGI_DB::get_data() const
{
	return m_fb;
}

void CL_Target_GGI_DB::to_front()
{
	ggiSetDisplayFrame( m_vis, m_frame );
//	ggiSetDisplayFrame( m_vis, 2 );
	ggiFlush( m_vis );
}

void CL_Target_GGI_DB::to_back()
{
	ggiSetWriteFrame( m_vis, m_frame );
}

unsigned int CL_Target_GGI_DB::get_width() const
{
	return m_mode.virt.x;
}

unsigned int CL_Target_GGI_DB::get_height() const
{
	return m_mode.virt.y;
}

unsigned int CL_Target_GGI_DB::get_pitch() const
{
	return m_db->buffer.plb.stride;
}

unsigned int CL_Target_GGI_DB::get_depth() const
{
	return m_db->buffer.plb.pixelformat->depth;
}

unsigned int CL_Target_GGI_DB::get_red_mask() const
{
	return m_db->buffer.plb.pixelformat->red_mask;
}

unsigned int CL_Target_GGI_DB::get_green_mask() const
{
	return m_db->buffer.plb.pixelformat->green_mask;
}

unsigned int CL_Target_GGI_DB::get_blue_mask() const
{
	return m_db->buffer.plb.pixelformat->blue_mask;
}

unsigned int CL_Target_GGI_DB::get_alpha_mask() const
{
	return m_db->buffer.plb.pixelformat->alpha_mask;
}

CL_Palette *CL_Target_GGI_DB::get_palette() const
{
	return NULL;
}

#endif
