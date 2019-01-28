/*
	$Id: sprite.cpp,v 1.3 2001/12/11 20:44:22 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Simple sprite support.

*/

#include "Core/precomp.h"
#include <cstdio>

#include "API/Display/Display/palette.h"
#include "API/Display/SurfaceProviders/sprite.h"
#include "API/Core/System/error.h" 
#include "API/Core/System/cl_assert.h"

/****************
	CL_SpriteProvider class
*****************/

CL_Surface *CL_SpriteProvider::create(const std::string &surface_id, CL_InputSourceProvider *datafile)
{
	return CL_Surface::create(new CL_SpriteProvider(surface_id, datafile), true);
}

CL_SpriteProvider::CL_SpriteProvider(const std::string &_surface_id, CL_InputSourceProvider *datafile)
{
	surface_id = _surface_id;
	provider = datafile != NULL ? datafile->clone() : CL_InputSourceProvider::create_file_provider(".");

	palette = NULL;
	surface_data = NULL;
}

CL_SpriteProvider::~CL_SpriteProvider()
{
	perform_unlock();
	delete provider;
}

unsigned int CL_SpriteProvider::get_width() const
{
	return width;
}

unsigned int CL_SpriteProvider::get_height() const
{
	return height;
}

unsigned int CL_SpriteProvider::get_pitch() const
{
	return width*((bpp+7)/8);
}

unsigned int CL_SpriteProvider::get_num_frames() const
{
	return no_sprs;
}
/*
EPixelFormat CL_SpriteProvider::get_pixel_format() const
{
	return pixelformat;
}
*/
CL_Palette *CL_SpriteProvider::get_palette() const
{
	return palette;
}

bool CL_SpriteProvider::is_indexed() const
{
	return m_is_indexed;
}

bool CL_SpriteProvider::uses_src_colorkey() const
{
	return transcol != -1;
}

unsigned int CL_SpriteProvider::get_src_colorkey() const
{
	return transcol;
}

unsigned int CL_SpriteProvider::get_red_mask() const
{
	return red_mask;
}

unsigned int CL_SpriteProvider::get_green_mask() const
{
	return green_mask;
}

unsigned int CL_SpriteProvider::get_blue_mask() const
{
	return blue_mask;
}

unsigned int CL_SpriteProvider::get_alpha_mask() const
{
	return alpha_mask;
}

unsigned int CL_SpriteProvider::get_depth() const
{
	return bpp;
}

void *CL_SpriteProvider::get_data() const
{
	return surface_data;
}

void CL_SpriteProvider::load_data()
{
//	cout << "Locking spriteprovider from location " << surface_id << endl;

	CL_InputSource *datafile = provider->open_source(surface_id.c_str());
	cl_assert(datafile != NULL);

//	cout << "Hello?" << endl;
	width = datafile->read_int32();
	height = datafile->read_int32();
	no_sprs = datafile->read_int32();
	transcol = datafile->read_int32();

//	printf("width %d, height %d, no_sprs %d, transcol %d\n", width, height, no_sprs, transcol);

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
//	printf("pixelformat %d", (int) pixelformat);

	// Quick-verify data:
	cl_assert(width > 0);
	cl_assert(height > 0);
	cl_assert(transcol >= -1);
	cl_assert(no_sprs > 0);
 
	int size_surface_data = width*height*no_sprs*((bpp+7)/8);
//	cout << "Surface data size: " << size_surface_data << endl;
	surface_data = new unsigned char[size_surface_data];
	int read = datafile->read(surface_data, size_surface_data);
	cl_assert(read == size_surface_data);

	delete datafile;
}

void CL_SpriteProvider::perform_lock()
{
	load_data();
}

void CL_SpriteProvider::perform_unlock()
{
	delete[] ((char*) surface_data);
	surface_data = NULL;

	delete palette;
	palette = NULL;
}

/***********************
	CL_SpriteTexture class
************************/
/*
CL_Texture *CL_SpriteTexture::load(char *surface_id, CL_InputSourceProvider *datafile)
{
	return CL_Texture::create(new CL_SpriteTexture(surface_id, datafile), true);
}

CL_SpriteTexture::CL_SpriteTexture(char *_surface_id, CL_InputSourceProvider *datafile)
{
	surface_id = _surface_id;
	provider = datafile != NULL ? datafile->clone() : CL_InputSourceProvider::create_file_provider(".");

	palette = NULL;
	surface_data = NULL;

	load_data();
}

CL_SpriteTexture::~CL_SpriteTexture()
{
	unlock();
}

unsigned int CL_SpriteTexture::get_width()
{
	return width;
}

unsigned int CL_SpriteTexture::get_height()
{
	return height;
}

EPixelFormat CL_SpriteTexture::get_pixel_format()
{
	return pixelformat;
}

CL_Palette *CL_SpriteTexture::get_palette()
{
	return palette;
}

int CL_SpriteTexture::get_transcol()
{
	return transcol;
}

void *CL_SpriteTexture::get_data()
{
	return surface_data;
}

void CL_SpriteTexture::load_data()
{
	CL_InputSource *datafile = provider->open_source(surface_id);
	cl_assert(datafile != NULL);

	width = datafile->read_short16();
	height = datafile->read_short16();
	transcol = datafile->read_short16();
	no_sprs = datafile->read_short16();
	int format = datafile->read_short16();

	// Quick-verify data:
	cl_assert(width>0&&height>0&&transcol>=-1&&no_sprs>0&&format>=0);

	if (format == 0) // 8 bit, indexed/palette format
	{
		pixelformat = PAL8;

		char *palette_id = datafile->read_string();
		
		datafile->push_position();
		palette = new CL_Palette(palette_id, provider);
		datafile->pop_position();

		delete palette_id;
		
		unsigned char *surface_indexed = new unsigned char[width*height*no_sprs];
		cl_assert(surface_indexed != NULL);
		datafile->read((char *) surface_indexed, width*height*no_sprs);

		surface_data = surface_indexed;
	}
	else // RGBA format
	{
		cout << "RGBA surface format not supported" << endl;
		exit(1);

//		throw new CL_Error_ClanLib_Sprite_Load("RGBA surface format not supported in this version of ClanLib!", __FILE__, __LINE__);
	}

	delete datafile;
}

void CL_SpriteTexture::lock()
{
	if (surface_data != NULL) return; // Data already loaded

	load_data();
}

void CL_SpriteTexture::unlock()
{
	delete surface_data;
	surface_data = NULL;

	delete palette;
	palette = NULL;
}
*/
