/*
	$Id: sprite_subsection_provider.cpp,v 1.1 2001/03/06 15:09:22 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Sprite subsection provider

*/

#include "Core/precomp.h"
#include "API/Display/SurfaceProviders/sprite_subsection_provider.h"

CL_Surface *CL_SpriteSubsectionProvider::create(
	CL_SurfaceProvider *parent_provider, 
	int start_x,
	int start_y, 
	int width,
	int height)
{
	return CL_Surface::create(new CL_SpriteSubsectionProvider(parent_provider, start_x, start_y, width, height), true);
}

CL_SpriteSubsectionProvider::CL_SpriteSubsectionProvider(CL_SurfaceProvider *_parent_provider, 
	int _start_x, int _start_y, 
	int _width, int _height)
{
	parent_provider = _parent_provider;
	start_x = _start_x;
	start_y = _start_y;
	width = _width;
	height = _height;
	locked = false;
}

CL_SpriteSubsectionProvider::~CL_SpriteSubsectionProvider()
{
	delete parent_provider;
}

unsigned int CL_SpriteSubsectionProvider::get_pitch() const
{
	return parent_provider->get_pitch();
}

int CL_SpriteSubsectionProvider::get_translate_x() const
{
	return start_x;
}

int CL_SpriteSubsectionProvider::get_translate_y() const
{
	return start_y;
}

unsigned int CL_SpriteSubsectionProvider::get_width() const
{
	return width;
}

unsigned int CL_SpriteSubsectionProvider::get_height() const
{
	return height;
}

unsigned int CL_SpriteSubsectionProvider::get_num_frames() const
{
	return 1;
}
/*
EPixelFormat CL_SpriteSubsectionProvider::get_pixel_format() const
{
	return parent_provider->get_pixel_format();
}
*/

bool CL_SpriteSubsectionProvider::is_indexed() const
{
	return parent_provider->is_indexed();
}

unsigned int CL_SpriteSubsectionProvider::get_red_mask() const
{
	return parent_provider->get_red_mask();
}

unsigned int CL_SpriteSubsectionProvider::get_green_mask() const
{
	return parent_provider->get_green_mask();
}

unsigned int CL_SpriteSubsectionProvider::get_blue_mask() const
{
	return parent_provider->get_blue_mask();
}

unsigned int CL_SpriteSubsectionProvider::get_alpha_mask() const
{
	return parent_provider->get_alpha_mask();
}

unsigned int CL_SpriteSubsectionProvider::get_depth() const
{
	return parent_provider->get_depth();
}

CL_Palette *CL_SpriteSubsectionProvider::get_palette() const
{
	return parent_provider->get_palette();
}

bool CL_SpriteSubsectionProvider::uses_src_colorkey() const
{
	return parent_provider->uses_src_colorkey();
}

unsigned int CL_SpriteSubsectionProvider::get_src_colorkey() const
{
	return parent_provider->get_src_colorkey();
}

void *CL_SpriteSubsectionProvider::get_data() const
{
	return parent_provider->get_data();
}

void CL_SpriteSubsectionProvider::perform_lock()
{
	if (locked) return;

	parent_provider->lock();
	locked = true;
}

void CL_SpriteSubsectionProvider::perform_unlock()
{
	parent_provider->unlock();
	locked = false;
}

