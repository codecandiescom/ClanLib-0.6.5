/*
	$Id: sprite_subarray_provider.cpp,v 1.3 2002/03/02 19:43:46 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Sprite subarray provider

*/

#include "Core/precomp.h"
#include "API/Display/SurfaceProviders/sprite_subarray_provider.h"
#include "API/Core/System/cl_assert.h"

CL_Surface *CL_SpriteSubarrayProvider::create(CL_SurfaceProvider *parent_provider, 
	int start_x, int start_y, 
	int width, int height, 
	int array_width, int array_height)
{
	return CL_Surface::create(new CL_SpriteSubarrayProvider(parent_provider, start_x, start_y, width, height, array_width, array_height), true);
}

CL_SpriteSubarrayProvider::CL_SpriteSubarrayProvider(CL_SurfaceProvider *_parent_provider, 
	int _start_x, int _start_y, 
	int _width, int _height,
	int _array_width, int _array_height)
{
	parent_provider = _parent_provider;
	start_x = _start_x;
	start_y = _start_y;
	width = _width;
	height = _height;
	array_width = _array_width;
	array_height = _array_height;
	locked = false;
	surface_data = NULL;
}

CL_SpriteSubarrayProvider::~CL_SpriteSubarrayProvider()
{
//	delete parent_provider;
}

unsigned int CL_SpriteSubarrayProvider::get_pitch() const
{
	return width*((get_depth()+7)/8);
}

int CL_SpriteSubarrayProvider::get_translate_x() const
{
	return 0;
}

int CL_SpriteSubarrayProvider::get_translate_y() const
{
	return 0;
}

unsigned int CL_SpriteSubarrayProvider::get_depth() const
{
	return parent_provider->get_depth();
}

unsigned int CL_SpriteSubarrayProvider::get_width() const
{
	return width;
}

unsigned int CL_SpriteSubarrayProvider::get_height() const
{
	return height;
}

unsigned int CL_SpriteSubarrayProvider::get_num_frames() const
{
	return array_width*array_height;
}
/*
EPixelFormat CL_SpriteSubarrayProvider::get_pixel_format() const
{
	return parent_provider->get_pixel_format();
}
*/

bool CL_SpriteSubarrayProvider::is_indexed() const
{
	return parent_provider->is_indexed();
}

unsigned int CL_SpriteSubarrayProvider::get_red_mask() const
{
	return parent_provider->get_red_mask();
}

unsigned int CL_SpriteSubarrayProvider::get_green_mask() const
{
	return parent_provider->get_green_mask();
}

unsigned int CL_SpriteSubarrayProvider::get_blue_mask() const
{
	return parent_provider->get_blue_mask();
}

unsigned int CL_SpriteSubarrayProvider::get_alpha_mask() const
{
	return parent_provider->get_alpha_mask();
}

CL_Palette *CL_SpriteSubarrayProvider::get_palette() const
{
	return parent_provider->get_palette();
}

bool CL_SpriteSubarrayProvider::uses_src_colorkey() const
{
	return parent_provider->uses_src_colorkey();
}

unsigned int CL_SpriteSubarrayProvider::get_src_colorkey() const
{
	return parent_provider->get_src_colorkey();
}

void *CL_SpriteSubarrayProvider::get_data() const
{
	cl_assert(surface_data != NULL);
	return surface_data;
}

void CL_SpriteSubarrayProvider::perform_lock()
{
	if (locked) return;

	parent_provider->lock();
	
	int bytes_per_pixel = ((parent_provider->get_depth()+7)/8);

	surface_data = new unsigned char[
		bytes_per_pixel*width*height*array_width*array_height];

	int bytes_pr_line = bytes_per_pixel*width;

	unsigned char *parent_data = (unsigned char *) parent_provider->get_data();
	int pos = 0;

	int offset_source =
		start_x*bytes_per_pixel +
		start_y*parent_provider->get_pitch();

	for (int y=0;y<array_height;y++)
	{
		int cur_offset = offset_source + y*height*parent_provider->get_pitch();

		for (int x=0;x<array_width;x++)
		{
			for (int yp=0;yp<height;yp++)
			{
				memcpy(
					&surface_data[pos], 
					&parent_data[cur_offset+yp*parent_provider->get_pitch()], bytes_pr_line);

				pos += bytes_pr_line;
			}
			cur_offset += width*bytes_per_pixel;
		}
	}

	locked = true;
}

void CL_SpriteSubarrayProvider::perform_unlock()
{
	parent_provider->unlock();
	delete[] surface_data;
	locked = false;
}

