/*
	$Id: blit_dx.cpp,v 1.4 2001/09/08 19:12:46 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <windows.h>
#include "blit_dx.h"
#include <API/Display/Display/surfaceprovider.h>
#include "../Generic/colormap.h"
#include "../Generic/blit_alphamask_rle.h"
#include "API/Display/Display/pixeldata.h"

std::list<CL_Blit_DX*> CL_Blit_DX::m_loaded_surfaces;

CL_Blit_DX::CL_Blit_DX(
	CL_DisplayCard_DirectDraw *target,
	CL_SurfaceProvider *provider)
{
	m_target = target;
	m_provider = provider;
	m_surface = NULL;
	m_alphablitter = NULL;
}

CL_Blit_DX::~CL_Blit_DX()
{
	if (m_surface != NULL) 
	{
		m_surface->Release();
		m_loaded_surfaces.remove(this);
	}
	if (m_alphablitter != NULL) delete m_alphablitter;
}

bool CL_Blit_DX::init_surface(CL_Blitters *blitters)
{
	if (!reload(false, true)) return false;

	blitters->set_clip(this);
	blitters->set_noclip(this);
	blitters->set_scale_noclip(this);
	blitters->set_scale_clip(this);

	m_loaded_surfaces.push_back(this);

	return true;
}

void CL_Blit_DX::blt_noclip(
	CL_Target *target,
	int x,
	int y,
	int spr_no)
{
	DWORD flags = DDBLTFAST_WAIT;
	if (m_transparent)
	{
		flags|=DDBLTFAST_SRCCOLORKEY; 
	}
	else
	{
		flags|=DDBLTFAST_NOCOLORKEY;
	}

	RECT source;
	int delta = spr_no*m_height;

	source.left=0;
	source.top=delta;
	source.right=m_width;
	source.bottom=m_height+delta;

	m_target->get_back_buffer()->get_surface()->BltFast(x, y, m_surface, &source, flags);

	if (m_alphablitter != NULL) m_alphablitter->blt_noclip(target, x, y, spr_no);
}

void CL_Blit_DX::blt_clip(
	CL_Target *target,
	int x,
	int y,
	int spr_no,
	const CL_ClipRect &clip)
{
	DDBLTFX ddbltfx;
	ddbltfx.dwSize = sizeof(ddbltfx);

	DWORD flags = DDBLT_WAIT;
	if (m_transparent)
	{
		flags|=DDBLT_KEYSRC; 
	}

	RECT dest, source;
	source.left=0;
	source.top=0;
	source.right=m_width;
	source.bottom=m_height;

	dest.left = x;
	dest.top = y;
	dest.right = dest.left + m_width;
	dest.bottom = dest.top + m_height;

	if (dest.left < clip.m_x1)
	{
		source.left = clip.m_x1-dest.left;
		dest.left = clip.m_x1;
	}

	if (dest.top < clip.m_y1)
	{
		source.top = clip.m_y1-dest.top;
		dest.top = clip.m_y1;
	}

	if (dest.right > clip.m_x2)
	{
		source.right -= dest.right-clip.m_x2;
		dest.right = clip.m_x2;
	}

	if (dest.bottom > clip.m_y2)
	{
		source.bottom -= dest.bottom-clip.m_y2;
		dest.bottom = clip.m_y2;
	}

	if (dest.right<dest.left || dest.bottom<dest.top) return; // Out of clipping borders

	int delta = spr_no*m_height;
	source.top += delta;
	source.bottom += delta;

	m_target->get_back_buffer()->get_surface()->Blt(&dest, m_surface, &source, flags, &ddbltfx);
	if (m_alphablitter != NULL) m_alphablitter->blt_clip(target, x, y, spr_no, clip);
}

void CL_Blit_DX::blt_scale_noclip(
	CL_Target *target,
	int x,
	int y,
	int dest_width,
	int dest_height,
	int spr_no)
{
	DDBLTFX ddbltfx;
	ddbltfx.dwSize = sizeof(ddbltfx);

	DWORD flags = DDBLT_WAIT;
	if (m_transparent)
	{
		flags|=DDBLT_KEYSRC; 
	}

	RECT dest, source;
	source.left=0;
	source.top=0;
	source.right=m_width;
	source.bottom=m_height;

	dest.left = x;
	dest.top = y;
	dest.right = dest.left + dest_width;
	dest.bottom = dest.top + dest_height;

	int delta = spr_no*m_height;
	source.top += delta;
	source.bottom += delta;

	m_target->get_back_buffer()->get_surface()->Blt(&dest, m_surface, &source, flags, &ddbltfx);
	if (m_alphablitter != NULL) m_alphablitter->blt_scale_noclip(target, x, y, dest_width, dest_height, spr_no);
}

void CL_Blit_DX::blt_scale_clip(
	CL_Target *target,
	int x,
	int y,
	int dest_width,
	int dest_height,
	int spr_no,
	const CL_ClipRect &clip)
{
	DDBLTFX ddbltfx;
	ddbltfx.dwSize = sizeof(ddbltfx);

	DWORD flags = DDBLT_WAIT;
	if (m_transparent)
	{
		flags|=DDBLT_KEYSRC; 
	}

	RECT dest, source;
	source.left=0;
	source.top=0;
	source.right=m_width;
	source.bottom=m_height;

	dest.left = x;
	dest.top = y;
	dest.right = dest.left + dest_width;
	dest.bottom = dest.top + dest_height;

	float dx = float(m_width)/dest_width;
	float dy = float(m_height)/dest_height;

	if (dest.left < clip.m_x1)
	{
		source.left = int((clip.m_x1-dest.left)*dx+0.5);
		dest.left = clip.m_x1;
	}

	if (dest.top < clip.m_y1)
	{
		source.top = int((clip.m_y1-dest.top)*dy+0.5);
		dest.top = clip.m_y1;
	}

	if (dest.right > clip.m_x2)
	{
		source.right -= int((dest.right-clip.m_x2)*dx+0.5);
		dest.right = clip.m_x2;
	}

	if (dest.bottom > clip.m_y2)
	{
		source.bottom -= int((dest.bottom-clip.m_y2)*dy+0.5);
		dest.bottom = clip.m_y2;
	}

	if (dest.right<dest.left || dest.bottom<dest.top) return; // Out of clipping borders

	int delta = spr_no*m_height;
	source.top += delta;
	source.bottom += delta;

	m_target->get_back_buffer()->get_surface()->Blt(&dest, m_surface, &source, flags, &ddbltfx);
	if (m_alphablitter != NULL) m_alphablitter->blt_scale_clip(target, x, y, dest_width, dest_height, spr_no, clip);
}

bool CL_Blit_DX::reload(bool lock_provider, bool create_surface)
{
	unsigned int y;

	if (lock_provider) m_provider->lock();

	unsigned int num_lines = m_provider->get_height()*m_provider->get_num_frames();

	CL_PixelData alpha(
		0,
		0,
		0,
		255,
		m_provider,
		1);

	int bytes_pr_line = alpha.get_bytes_per_line();
	unsigned char *alpha_data = new unsigned char[num_lines*bytes_pr_line];

	bool has_alpha = false;
	m_transparent = false;

	int alpha_width = alpha.get_width();
	for (y=0;y<num_lines;y++)
	{
		unsigned char *l = &alpha_data[y*bytes_pr_line];
		alpha.get_line_pixel_to_dest(y, l);

		for (int x=0;x<alpha_width;x++)
		{
			if (l[x] == 0) m_transparent = true;
			if (l[x] != 0 && l[x] != 255) 
			{
				m_transparent = true;
				has_alpha = true;
				break;
			}
		}
	}

	CL_PixelData input(
		m_target->get_target()->get_red_mask(),
		m_target->get_target()->get_green_mask(),
		m_target->get_target()->get_blue_mask(),
		m_target->get_target()->get_alpha_mask(),
		m_provider,
		(m_target->get_target()->get_depth()+7)/8);

	m_width = m_provider->get_width();
	m_height = m_provider->get_height();

	if (create_surface)
	{
		DDSURFACEDESC sur_desc;
		sur_desc.dwSize = sizeof(DDSURFACEDESC);
		sur_desc.dwFlags=DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
		sur_desc.dwWidth=m_width;
		sur_desc.dwHeight=m_height*m_provider->get_num_frames();
		sur_desc.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN;
/*		
		if (m_target->m_target->m_system_backbuffer != NULL)
		{
			sur_desc.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
		}
		else*/
		{
			sur_desc.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
		}

		HRESULT err;
		err = m_target->get_directdraw()->CreateSurface(&sur_desc, &m_surface, NULL);
		if (err != DD_OK)
		{
			delete[] alpha_data;
			return false;
		}
	}
	else
	{
		m_surface->Restore();
	}

	DDSURFACEDESC surface_desc;
	memset(&surface_desc, 0, sizeof(DDSURFACEDESC));
	surface_desc.dwSize = sizeof(DDSURFACEDESC);

	HRESULT err = m_surface->Lock(
		NULL,
		&surface_desc,
		DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,
		NULL);
	if (err != DD_OK) 
	{
		delete[] alpha_data;
		m_surface->Release();
		m_surface = NULL;
		return false;
	}

	CL_ColorMap color_map(m_target->get_target());
	unsigned int fill_color = color_map.calc_color(1, 0, 1, 0);

	unsigned char *surface_data = (unsigned char *) surface_desc.lpSurface;

	if (m_transparent)
	{
		int input_bytes_pr_line = input.get_bytes_per_line();
		unsigned char *image_data = new unsigned char[num_lines*input_bytes_pr_line];

		switch ((m_target->get_target()->get_depth()+7)/8)
		{
		case 1:
			{
				unsigned char col = (unsigned char) fill_color;

				for (y=0;y<num_lines;y++)
				{
					unsigned char *input_data = &image_data[y*input_bytes_pr_line];
					input.get_line_pixel_to_dest(y, input_data);

					unsigned char *a = &alpha_data[y*bytes_pr_line];
					for (unsigned int x=0;x<m_width;x++)
					{
						if (a[x] == 255)
						{
							surface_data[x] = input_data[x];
						}
						else
						{
							surface_data[x] = col;
						}
					}
					surface_data += surface_desc.lPitch;
				}
//				m_alphablitter = new CL_Blit_Transparent_RLE_8(m_provider, 1, m_target->get_target());
			}
			break;
		case 2:
			{
				unsigned short col = (unsigned short) fill_color;

				for (y=0;y<num_lines;y++)
				{
					unsigned char *input_data = &image_data[y*input_bytes_pr_line];
					input.get_line_pixel_to_dest(y, input_data);
					unsigned char *a = &alpha_data[y*bytes_pr_line];

					for (unsigned int x=0;x<m_width;x++)
					{
						if (a[x] == 255)
						{
							((unsigned short *) surface_data)[x] = ((unsigned short *) input_data)[x];
						}
						else
						{
							((unsigned short *) surface_data)[x] = col;
						}
					}
					surface_data += surface_desc.lPitch;
				}
				if (has_alpha) m_alphablitter = new CL_Blit_AlphaMask_RLE(
					m_target,
					m_provider,
					image_data,
					input_bytes_pr_line,
					alpha_data,
					bytes_pr_line);
			}
			break;
		case 3:
			{
				// Our 24 bit support really sucks!
				cl_assert(false);
			}
			break;
		case 4:
			{
				for (y=0;y<num_lines;y++)
				{
					unsigned char *input_data = &image_data[y*input_bytes_pr_line];
					input.get_line_pixel_to_dest(y, input_data);
					unsigned char *a = &alpha_data[y*bytes_pr_line];

					for (unsigned int x=0;x<m_width;x++)
					{
						if (a[x] == 255)
						{
							((unsigned int *) surface_data)[x] = ((unsigned int *) input_data)[x];
						}
						else
						{
							((unsigned int *) surface_data)[x] = fill_color;
						}
					}
					surface_data += surface_desc.lPitch;
				}
				if (has_alpha) m_alphablitter = new CL_Blit_AlphaMask_RLE(
					m_target,
					m_provider,
					image_data,
					input_bytes_pr_line,
					alpha_data,
					bytes_pr_line);
			}
			break;
		}
		delete[] image_data;
	}
	else
	{
		int bytes_per_line = input.get_bytes_per_pixel()*input.get_width();
		for (unsigned int y=0;y<num_lines;y++)
		{
			memcpy(
				&surface_data[y*surface_desc.lPitch],
				input.get_line_pixel(y),
				bytes_per_line);
		}
	}

	err = m_surface->Unlock(surface_data);
	cl_assert(err == DD_OK);

	if (m_transparent)
	{
		DDCOLORKEY key;
		key.dwColorSpaceLowValue = fill_color;
		key.dwColorSpaceHighValue = fill_color;

		err = m_surface->SetColorKey(DDCKEY_SRCBLT, &key);
		cl_assert(err == DD_OK);
	}

	if (lock_provider) m_provider->unlock();
	delete[] alpha_data;

	return true;
}

void CL_Blit_DX::reload_all_surfaces()
{
	for (
		std::list<CL_Blit_DX*>::iterator counter = m_loaded_surfaces.begin();
		counter != m_loaded_surfaces.end();
		counter++)
	{
		(*counter)->reload(true, false);
	}
}

void CL_Blit_DX::release_all_surfaces()
{
	for (
		std::list<CL_Blit_DX*>::iterator counter = m_loaded_surfaces.begin();
		counter != m_loaded_surfaces.end();
		counter++)
	{
		(*counter)->m_surface->Release();
		(*counter)->m_surface = NULL;
	}
}

void CL_Blit_DX::create_all_surfaces()
{
	for (
		std::list<CL_Blit_DX*>::iterator counter = m_loaded_surfaces.begin();
		counter != m_loaded_surfaces.end();
		counter++)
	{
		(*counter)->reload(true, true);
	}
}
