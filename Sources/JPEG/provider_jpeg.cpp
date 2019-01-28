/*
	$Id: provider_jpeg.cpp,v 1.10 2002/01/22 10:29:33 sphair Exp $
	
	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#define WIN32_LEAN_AND_MEAN

#include "API/JPEG/provider_jpeg.h"
#include "provider_jpeg_generic.h"
#include "API/Display/Display/surface.h"
#include "API/Display/Display/res_surface.h"

CL_Surface *CL_JPEGProvider::create(
	const CL_String &file,
	CL_InputSourceProvider *provider, 
	bool transparent,
	bool ignore_alphachannel)
{
	return CL_Surface::create(
		new CL_JPEGProvider(
			file,
			provider, 
			transparent,
			ignore_alphachannel),
		true);
}

CL_JPEGProvider::CL_JPEGProvider(
	CL_String name, 
	CL_InputSourceProvider *provider,
	bool transparent,
	bool ignore_alphachannel)
: impl(NULL)
{
	impl = new CL_JPEGProvider_Generic(name, provider, transparent, ignore_alphachannel);
}

CL_JPEGProvider::~CL_JPEGProvider()
{
	delete impl;
}

unsigned int CL_JPEGProvider::get_pitch() const
{
	return impl->get_pitch();
}

unsigned int CL_JPEGProvider::get_width() const
{
	return impl->get_width();
}

unsigned int CL_JPEGProvider::get_height() const
{
	return impl->get_height();
}

unsigned int CL_JPEGProvider::get_num_frames() const
{
	return impl->get_num_frames();
}

EPixelFormat CL_JPEGProvider::get_pixel_format() const
{
	return impl->get_pixel_format();
}

CL_Palette *CL_JPEGProvider::get_palette() const
{
	return impl->get_palette();
}

unsigned int CL_JPEGProvider::get_src_colorkey() const
{
	return impl->get_src_colorkey();
}

bool CL_JPEGProvider::uses_src_colorkey() const
{
	return impl->uses_src_colorkey();
}

bool CL_JPEGProvider::is_indexed() const
{
	return impl->is_indexed();
}

unsigned int CL_JPEGProvider::get_red_mask() const
{
	return impl->get_red_mask();
}

unsigned int CL_JPEGProvider::get_green_mask() const
{
	return impl->get_green_mask();
}

unsigned int CL_JPEGProvider::get_blue_mask() const
{
	return impl->get_blue_mask();
}

unsigned int CL_JPEGProvider::get_alpha_mask() const
{
	return impl->get_alpha_mask();
}

void *CL_JPEGProvider::get_data() const
{
	return impl->get_data();
}

void CL_JPEGProvider::lock()
{
	impl->perform_lock();
}

void CL_JPEGProvider::unlock()
{
	impl->perform_unlock();
}

/////////////////////////////////////////////////////////////////////////////
// Resource support:

class CL_JPEG_ResourceSource : public CL_ResourceSource_Surface
{
public:
	virtual const char *get_name() { return "jpg"; }

	virtual bool can_create(
		std::string ext,
		CL_ResourceOptions &options)
		{
			if (ext == ".jpg") return true;
			if (options.exists("jpg")) return true;

			return false;
		}

	virtual CL_SurfaceProvider *create(
		CL_Resource &resource,
		CL_ResourceOptions &options,
		CL_ResourceManager &parent)
	{
		return new CL_JPEGProvider(resource.get_full_location().c_str(), NULL);
	}

	virtual CL_SurfaceProvider *create(const std::string &filename)
	{
		return new CL_JPEGProvider(filename.c_str(), NULL);
	}
};

static CL_JPEG_ResourceSource *res_source_jpeg = NULL;

void CL_SetupJPEG::init(bool register_resources_only)
{
	res_source_jpeg = new CL_JPEG_ResourceSource;
}

void CL_SetupJPEG::deinit()
{
	delete res_source_jpeg;
	res_source_jpeg = NULL;
}
