/*
	$Id: blit_dynamic.cpp,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/


#include "Core/precomp.h"

#include <Display/Display/Generic/blit_dynamic.h>
#include <Hermes/H_Blit.h>
#include "API/Core/System/cl_assert.h"
#include "API/Display/Display/palette.h"
#include "API/Display/Display/display.h"
#include "API/Display/Display/surfaceprovider.h"

CL_Blit_Dynamic::CL_Blit_Dynamic(
	CL_SurfaceProvider *provider,
	CL_Target *target)
{
	this->provider = provider;
	
	src_fmt = Hermes_FormatNewEmpty();
	dest_fmt = Hermes_FormatNewEmpty();

	handle = Hermes_BlitterInstance(HERMES_CONVERT_NORMAL);
	cl_assert(handle!=0);
	
	use_palette = false;

	if (provider->is_indexed())
	{
		src_fmt->bits = 8;
		src_fmt->indexed = true;
		src_fmt->r = 0;
		src_fmt->g = 0;
		src_fmt->b = 0;
		src_fmt->a = 0; 
		  
		palette = Hermes_PaletteInstance();
		cl_assert(palette!=0);
		use_palette = true;

		int pal[256];
		
		if (provider->get_palette() != NULL)
		{
			for (int i=0; i< provider->get_palette()->num_colors; i++)
			{
				pal[i] = (provider->get_palette()->palette[3*i] << 16) |
					 (provider->get_palette()->palette[3*i+1] << 8) |
					 (provider->get_palette()->palette[3*i+2]);
			}
		} 
		else
		{
			for (int i=0; i< CL_Display::get_palette()->num_colors; i++)
			{
				pal[i] = (CL_Display::get_palette()->palette[3*i] << 16) |
					 (CL_Display::get_palette()->palette[3*i+1] << 8) |
					 (CL_Display::get_palette()->palette[3*i+2]);
			}
		}
		
		Hermes_PaletteSet(palette, (int32*) pal);
	}
	else
	{
		src_fmt->bits = provider->get_depth();
		src_fmt->indexed = false;
		src_fmt->r = provider->get_red_mask();
		src_fmt->g = provider->get_green_mask();
		src_fmt->b = provider->get_blue_mask();
		src_fmt->a = provider->get_alpha_mask();
	}

	if (provider->uses_src_colorkey())
	{
		src_fmt->has_colorkey = 1;
		src_fmt->colorkey = provider->get_src_colorkey();
	}

	cl_assert(target!=NULL);
	dest_fmt->bits = target->get_depth();
	dest_fmt->indexed = false;
	dest_fmt->r = target->get_red_mask();
	dest_fmt->g = target->get_green_mask();
	dest_fmt->b = target->get_blue_mask();
	dest_fmt->a = target->get_alpha_mask();

	
/*
	cout << "src bits      " << src_fmt->bits << endl;
	if (src_fmt->indexed) cout << "src indexed" << endl;
	cout << "src r         " << src_fmt->r << endl;
	cout << "src g         " << src_fmt->g << endl;
	cout << "src b         " << src_fmt->b << endl;
	cout << "src a         " << src_fmt->a << endl;
	if (src_fmt->has_colorkey==1)
		cout << "src colorkey  " << src_fmt->colorkey << endl;

	cout << endl;
	
	
	cout << "dst bits      " << dest_fmt->bits << endl;
	if (dest_fmt->indexed) cout << "dst indexed" << endl;
	cout << "dst r         " << dest_fmt->r << endl;
	cout << "dst g         " << dest_fmt->g << endl;
	cout << "dst b         " << dest_fmt->b << endl;
	cout << "dst a         " << dest_fmt->a << endl;
	if (dest_fmt->has_colorkey==1)
		cout << "dest colorkey " << dest_fmt->colorkey << endl;


	cout << endl;
	
	cout << "handle     " << handle << endl;
	cout <<	"src data   " << provider->get_data() << endl;
	cout << "src x	   0" << endl;
	cout << "src y      " << provider->get_height() << endl;
	cout <<	"src width  " << provider->get_width() << endl;
	cout <<	"src height " << provider->get_height() << endl;
	cout <<	"src pitch  " << provider->get_pitch() << endl;
	cout <<	"dst data   " << target->get_data() << endl;
	cout <<	"dst pitch  " << target->get_pitch() << endl;
*/

}

CL_Blit_Dynamic::~CL_Blit_Dynamic()
{
	Hermes_BlitterReturn(handle);
	if (use_palette)
		Hermes_PaletteReturn(palette);

	Hermes_FormatFree(src_fmt);
	Hermes_FormatFree(dest_fmt);
}

void CL_Blit_Dynamic::blt_noclip(
	CL_Target *target,
	int x, 
	int y,
	int spr_no)
{
	blt_scale_noclip(target, x, y, provider->get_width(), provider->get_height(), spr_no);
}

void CL_Blit_Dynamic::blt_scale_noclip(
	CL_Target *target,
	int x,
	int y,
	int dest_width,
	int dest_height,
	int spr_no)
{
	target->lock();
	provider->lock();
	
	int res = Hermes_BlitterRequest(handle, src_fmt, dest_fmt);
	cl_assert(res != 0);

	if (use_palette)
	{
		// as described in api.txt
		int res = Hermes_BlitterPalette(handle, palette, palette);
		cl_assert(res != 0);
	}
	
	res = Hermes_BlitterBlit(
		handle,
		provider->get_data(),
		0,
		provider->get_height()*spr_no,
		provider->get_width(),
		provider->get_height(),
		provider->get_pitch(),
		target->get_data(),
		x,
		y,
		dest_width,
		dest_height,
		target->get_pitch());

	cl_assert(res != 0);

	provider->unlock();
	target->unlock();
}

void CL_Blit_Dynamic::blt_clip(
	CL_Target *target,
	int x,
	int y,
	int spr_no,
	const CL_ClipRect & clip)
{
	CL_ClipRect dest_clip(
		x,
		y,
		x + provider->get_width(),
		y + provider->get_height());

	CL_ClipRect clipped = clip.clip(dest_clip);
	
	if (clipped.m_x1 >= clipped.m_x2 ||
	    clipped.m_y1 >= clipped.m_y2) return;

	target->lock();
	provider->lock();
	
	int res = Hermes_BlitterRequest(handle, src_fmt, dest_fmt);
	cl_assert(res != 0);
	
	res = Hermes_BlitterBlit(
		handle,
		provider->get_data(),
		clipped.m_x1 - x,
		provider->get_height()*spr_no + clipped.m_y1 - y,
		clipped.m_x2 - clipped.m_x1,
		clipped.m_y2 - clipped.m_y1,
		provider->get_pitch(),
		target->get_data(),
		clipped.m_x1,
		clipped.m_y1,
		clipped.m_x2 - clipped.m_x1,
		clipped.m_y2 - clipped.m_y1,
		target->get_pitch());

	cl_assert(res != 0);
	provider->unlock();
	target->unlock();
}

void CL_Blit_Dynamic::blt_scale_clip(
	CL_Target * target,
	int x,
	int y,
	int dest_width,
	int dest_height,
	int spr_no,
	const CL_ClipRect & clip)
{
	static bool warning = true;
	if (warning)
	{
		cl_info(info_display, "CL_Blit_Dynamic::blt_scale_clip() not implemented yet.");
		warning = false;
	}
}
