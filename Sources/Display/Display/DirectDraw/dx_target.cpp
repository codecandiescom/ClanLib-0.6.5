/*
	$Id: dx_target.cpp,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include "API/Core/System/cl_assert.h"
#include "API/Core/System/error.h"

#include "dx_target.h"

CL_Target_DX::CL_Target_DX(LPDIRECTDRAWSURFACE surface)
{
	m_surface = NULL;
	m_data = NULL;
	m_surface_is_video = false;
	m_width = 0;
	m_height = 0;
	m_pitch = 0;
	m_depth = 0;
	m_red_mask = 0;
	m_green_mask = 0;
	m_blue_mask = 0;
	m_alpha_mask = 0;
	m_ref_count = 0;

	if (surface != NULL) set_surface(surface);
}

CL_Target_DX::~CL_Target_DX()
{
	if (m_surface != NULL)
	{
		m_surface->SetClipper(NULL); // make sure to remove clipper if one is attached.
		m_surface->Release();
	}
}

void CL_Target_DX::set_surface(LPDIRECTDRAWSURFACE surface)
{
	m_surface = surface;

	get_palette_from_surface();

	DDSURFACEDESC surface_desc;
	surface_desc.dwSize = sizeof(DDSURFACEDESC);

	m_surface->GetSurfaceDesc(&surface_desc);
	
	m_pitch = surface_desc.lPitch;
	m_width = surface_desc.dwWidth;
	m_height = surface_desc.dwHeight;
	m_depth = surface_desc.ddpfPixelFormat.dwRGBBitCount;
	m_red_mask = surface_desc.ddpfPixelFormat.dwRBitMask;
	m_green_mask = surface_desc.ddpfPixelFormat.dwGBitMask;
	m_blue_mask = surface_desc.ddpfPixelFormat.dwBBitMask;
	m_alpha_mask = surface_desc.ddpfPixelFormat.dwRGBAlphaBitMask;
	m_surface_is_video = false;
	if (surface_desc.ddsCaps.dwCaps | DDSCAPS_VIDEOMEMORY) m_surface_is_video = true;
}

void CL_Target_DX::lock()
{
	cl_assert(m_surface != NULL);

	m_ref_count++;
	if (m_ref_count == 1)
	{
		DDSURFACEDESC surface_desc;
		memset(&surface_desc, 0, sizeof(DDSURFACEDESC));
		surface_desc.dwSize = sizeof(DDSURFACEDESC);

		HRESULT err = m_surface->Lock(
			NULL,
			&surface_desc,
			DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,
			NULL);

		if (err != DD_OK) { throw CL_Error("DAMMIT"); }

		m_data = (unsigned char *) surface_desc.lpSurface;
		m_pitch = surface_desc.lPitch;
		m_width = surface_desc.dwWidth;
		m_height = surface_desc.dwHeight;
		m_depth = surface_desc.ddpfPixelFormat.dwRGBBitCount;
		m_red_mask = surface_desc.ddpfPixelFormat.dwRBitMask;
		m_green_mask = surface_desc.ddpfPixelFormat.dwGBitMask;
		m_blue_mask = surface_desc.ddpfPixelFormat.dwBBitMask;
		m_alpha_mask = surface_desc.ddpfPixelFormat.dwRGBAlphaBitMask;
	}
}

void CL_Target_DX::unlock()
{
	m_ref_count--;
	cl_assert(m_ref_count >= 0);

	if (m_ref_count == 0)
	{
		HRESULT err = m_surface->Unlock(m_data);
		cl_assert(err == DD_OK);
	}
}

void CL_Target_DX::set_palette(CL_Palette *pal)
{
	memcpy(m_palette.palette, pal->palette, m_palette.num_colors*3);
}

CL_Palette *CL_Target_DX::get_palette() const
{
	return (CL_Palette*) &m_palette;
}

void CL_Target_DX::get_palette_from_surface()
{
//	cl_info(0, "not impl!");
}
