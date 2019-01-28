/*
	$Id: surfaceprovider.cpp,v 1.4 2002/06/26 11:54:19 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <API/Core/Resources/resource_manager.h>
#include <API/Display/Display/surface.h>
#include <API/Display/Display/surfaceprovider.h>
#include <API/Core/System/cl_assert.h>
#include <API/Display/Display/palette.h>
#include <Display/Display/Generic/resource_surface_generic.h>

CL_SurfaceProvider *CL_SurfaceProvider::load(
	const std::string &resource_id,
	CL_ResourceManager *manager)
{
	CL_Resource &resource = manager->get_resource(resource_id);

	CL_ResourceData_Surface *data =
		(CL_ResourceData_Surface *) resource.get_data("surface");

	resource.load();

	CL_SurfaceProvider *provider = data->surface->get_provider();
	return provider;

//	resource.unload();
}
/*
bool CL_SurfaceProvider::pixel_transparent(int x, int y) const
{
	int transcol = get_transcol();
	if (transcol == -1) return false;

	cl_assert(x>=0 && y>=0 && (unsigned int) x<get_width() && (unsigned int) y<get_height());

	int bytes_pr_pixel = get_bytes_pr_pixel();
	unsigned char *surface_data = (unsigned char *) get_data();

	unsigned char *pos = &surface_data[x*bytes_pr_pixel+y*get_pitch()];

	switch (bytes_pr_pixel)
	{
	case 1:
		return *pos == transcol;
	case 2:
		return *((unsigned short *) pos) == transcol;
	case 3:
		{
			// This may not work at all (can't test it!)
			unsigned long val = *((unsigned long *) pos);
			val &= 0xffffff00;
			return val == (unsigned long) transcol;
		}
	case 4:
		return *((unsigned long *) pos) == (unsigned long) transcol;
	default:
		cl_assert(false); // Nutty business!
		return false;
	}
}
*/

void CL_SurfaceProvider::get_pixel(int x, int y, float *r, float *g, float *b, float *a)
{
	if (is_indexed ())
	{
		// Translate coords into position:
		int trans_x = get_translate_offset_x();
		int trans_y = get_translate_offset_y();
		x += trans_x;
		y += trans_y;


		lock();
		unsigned char* data = (unsigned char*) get_data();
		int color = 0;
	
		switch (get_bytes_per_pixel())
		{
		case	1 :
		{
			unsigned char *d = data + y * get_pitch() + x;
			color = *d;
			break;
		}
		default:
		{
			cl_assert(false);
			break;
		}
		}

		*r = get_palette()->palette[color*3 +0] / 255.0f;
		*g = get_palette()->palette[color*3 +1] / 255.0f;
		*b = get_palette()->palette[color*3 +2] / 255.0f;

		if (color == get_src_colorkey ())
			*a = 0.0f;
		else
			*a = 1.0f;
		
		unlock();
	} 
	else 
	{
		CL_Target::get_pixel (x, y, r, g, b, a);
	}
}

// EOF //
