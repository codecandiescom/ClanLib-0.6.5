/*
	$Id: sprite2.cpp,v 1.2 2001/05/09 15:41:39 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Simple sprite2 support.

*/

#include "Core/precomp.h"
#include <cstdio>
#include "API/Display/SurfaceProviders/sprite2.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Display/Display/palette.h"
#include "API/Core/System/cl_assert.h"

CL_Sprite2Provider::CL_Sprite2Provider(
	CL_InputSource *datafile)
{
	palette = NULL;
	surface_data = NULL;

	load_data(datafile);
}

CL_Sprite2Provider::CL_Sprite2Provider(
	CL_SurfaceProvider *src,
	int x, int y,
	int width, int height,
	int *tcols, int tcols_num)
{
	this->width = width;
	this->height = height;
	no_sprs = 1;
	if (tcols_num > 0) transcol = tcols[0];
	else transcol = -1;

	if (src->get_palette() != NULL)
		palette = new CL_Palette(
			src->get_palette()->palette,
			src->get_palette()->num_colors);

//	cout << "Palette: " << palette << endl;

//	pixelformat = src->get_pixel_format();
	m_is_indexed = src->is_indexed();
	red_mask = src->get_red_mask();
	green_mask = src->get_green_mask();
	blue_mask = src->get_blue_mask();
	alpha_mask = src->get_alpha_mask();
	bpp = src->get_depth();

//	cout << "Pixelformat: " << pixelformat << endl;

	// Quick-verify data:
	cl_assert(width > 0);
	cl_assert(height > 0);
	cl_assert(transcol >= -1);
	cl_assert(no_sprs > 0);
 
	int size_surface_data = width*height*no_sprs*((get_pitch()+7)/8);
	unsigned char *dest_data = new unsigned char[size_surface_data];
	surface_data = dest_data;

	src->lock();
	unsigned char *src_data = (unsigned char *) src->get_data();
	
	bool transcol_table[256];
	for (int i=0; i<256; i++) transcol_table[i] = false;
	for (int j=0; j<tcols_num; j++) transcol_table[tcols[j]] = true;

	for (int spr_y=0; spr_y<height; spr_y++)
	{
		for (int spr_x=0; spr_x<width; spr_x++)
		{
			unsigned char val = src_data[
				x+spr_x+
				(y+spr_y)*src->get_pitch()];

			if (transcol_table[val])
			{
				dest_data[spr_x+spr_y*get_pitch()] = transcol;
			}
			else
			{
				dest_data[spr_x+spr_y*get_pitch()] = val;
			}

/*			if (spr_x == 0 || spr_y == 0 || spr_x+1 == width || spr_y+1 == height)
				dest_data[spr_x+spr_y*width] = 16;*/
		}
	}

	src->unlock();
}

CL_Sprite2Provider::~CL_Sprite2Provider()
{
	delete[] ((char*) surface_data);
	surface_data = NULL;

	delete palette;
	palette = NULL;
}

unsigned int CL_Sprite2Provider::get_width() const
{
	return width;
}

unsigned int CL_Sprite2Provider::get_height() const
{
	return height;
}

unsigned int CL_Sprite2Provider::get_pitch() const
{
	return width*((get_depth()+7)/8);
}

unsigned int CL_Sprite2Provider::get_num_frames() const
{
	return no_sprs;
}
/*
EPixelFormat CL_Sprite2Provider::get_pixel_format() const
{
	return pixelformat;
}
*/
CL_Palette *CL_Sprite2Provider::get_palette() const
{
	return palette;
}

bool CL_Sprite2Provider::uses_src_colorkey() const
{
	return transcol != -1;
}

unsigned int CL_Sprite2Provider::get_src_colorkey() const
{
	return transcol;
}

void *CL_Sprite2Provider::get_data() const
{
	return surface_data;
}

void CL_Sprite2Provider::load_data(CL_InputSource *datafile)
{
	cl_assert(datafile != NULL);

	width = datafile->read_int32();
	height = datafile->read_int32();
	no_sprs = datafile->read_int32();
	transcol = datafile->read_int32();

	char has_palette = datafile->read_char8();
	if (has_palette)
	{
		unsigned char temp[768];
		datafile->read(temp, 768);
		palette = new CL_Palette(temp);
	}

	char indexed = datafile->read_char8();
	cl_assert(indexed == 0 || indexed == 1);
	if (indexed) m_is_indexed = true;
	else m_is_indexed = false;

//	pixelformat = (EPixelFormat) datafile->read_int32();
	red_mask = datafile->read_int32();
	green_mask = datafile->read_int32();
	blue_mask = datafile->read_int32();
	alpha_mask = datafile->read_int32();
	bpp = datafile->read_int32();
	
	// Quick-verify data:
	cl_assert(width > 0);
	cl_assert(height > 0);
	cl_assert(transcol >= -1);
	cl_assert(no_sprs > 0);
 
	int size_surface_data = width*height*no_sprs*((get_depth()+7)/8);
	surface_data = new unsigned char[size_surface_data];
	int read = datafile->read(surface_data, size_surface_data);
	cl_assert(read == size_surface_data);

//	delete datafile;
}

void CL_Sprite2Provider::perform_lock()
{
}

void CL_Sprite2Provider::perform_unlock()
{
}

bool CL_Sprite2Provider::is_indexed() const
{
	return m_is_indexed;
}

unsigned int CL_Sprite2Provider::get_red_mask() const
{
	return red_mask;
}

unsigned int CL_Sprite2Provider::get_green_mask() const
{
	return green_mask;
}

unsigned int CL_Sprite2Provider::get_blue_mask() const
{
	return blue_mask;
}

unsigned int CL_Sprite2Provider::get_alpha_mask() const
{
	return alpha_mask;
}

unsigned int CL_Sprite2Provider::get_depth() const
{
	return bpp;
}
