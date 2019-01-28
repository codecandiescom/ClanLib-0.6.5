/*
	$Id: surface.cpp,v 1.11 2002/06/20 10:27:13 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <API/Display/Display/display.h>
#include <API/Display/Display/displaycard.h>
#include <API/Display/Display/surface.h>
#include <API/Display/Display/surfaceprovider.h>
#include <API/Display/Display/res_surface.h>
#include <Display/Display/Generic/displaycard_generic.h>
#include <Display/Display/Generic/surface_generic.h>
#include <Display/Display/Generic/resource_surface_generic.h>
#include <Display/Display/Generic/blit_dynamic.h>
#include <API/Core/Resources/resource_manager.h>
#include <API/Core/Resources/resourceoptions.h>
#include "cardsurface_blitter.h"
#include "colormap.h"
#include "blit_macros.h"
#include <API/Core/System/clanstring.h>

extern "C"
{
	#include <Hermes/H_Conv.h>
	#include <Hermes/H_Pal.h>
}

CL_Surface *CL_Surface::load(
	const std::string &resource_id,
	CL_ResourceManager *manager)
{
	return new CL_Surface(resource_id, manager);
}

CL_Surface *CL_Surface::create(
	CL_SurfaceProvider *provider,
	bool delete_provider)
{
	return new CL_Surface(provider, delete_provider);
}

CL_Surface *CL_Surface::create_dynamic(
	CL_SurfaceProvider *provider,
	bool delete_provider)
{
	return new CL_Surface(new CL_Surface_Generic(provider, delete_provider, true));
}

CL_Surface::CL_Surface() : impl (0)
{
}

CL_Surface::CL_Surface(
	const std::string &resource_id,
	CL_ResourceManager *manager)
{
	CL_Resource &resource = manager->get_resource(resource_id);

	CL_ResourceData_Surface *data =
		(CL_ResourceData_Surface *) resource.get_data("surface");

	resource.load();

	impl = data->surface->impl;
	impl->add_reference();
}

CL_Surface::CL_Surface(
	CL_SurfaceProvider *provider,
	bool delete_provider,
	bool dynamic)
{
	provider->lock();
	impl = new CL_Surface_Generic(provider, delete_provider, dynamic);
	impl->add_reference();
	provider->unlock();
}

CL_Surface::CL_Surface(const std::string &filename, bool dynamic)
: impl(0)
{
	std::list<CL_ResourceSource_Surface*>::iterator it;
	
	CL_String ext = CL_String(filename).right(4);
	ext.to_lower();

	for (it = CL_ResourceSource_Surface::surface_sources.begin(); it != CL_ResourceSource_Surface::surface_sources.end(); it++)
	{
		CL_ResourceSource_Surface *src = *it;

		CL_ResourceOptions options;
		if (src->can_create(ext.get_string(), options))
		{
			CL_SurfaceProvider *provider = src->create(filename);

			provider->lock();
			impl = new CL_Surface_Generic(provider, true, dynamic);
			impl->add_reference();
			provider->unlock();

			return;
		}
	}

	throw CL_Error("Unknown image format");
}

CL_Surface::CL_Surface(class CL_Surface_Generic *_impl) : impl(_impl)
{
	if (impl) impl->add_reference();
}

CL_Surface::CL_Surface(const CL_Surface &surface) : impl(surface.impl)
{
	if (impl) impl->add_reference();
}

CL_Surface::~CL_Surface()
{
	if (impl && impl->release_reference() == 0) delete impl;
	resource.unload();
}

void CL_Surface::reload()
{
	impl->reload();
}

CL_Surface::operator bool () const
{
	return (impl != 0);
}

const CL_Surface &CL_Surface::operator=(const CL_Surface& surface) 
{
	resource.unload();
	if (impl && impl->release_reference() == 0) delete impl;

	resource = surface.resource;
	impl = surface.impl;

	resource.load();
	if (impl) impl->add_reference();

	return *this;
}

CL_SurfaceProvider *CL_Surface::get_provider() const
{
	return impl->get_provider();
}

void CL_Surface::put_screen(
	int x,
	int y,
	int spr_no,
	CL_DisplayCard *card)
{
	impl->put_screen(x, y, spr_no, card);
}

void CL_Surface::put_screen(
	int x,
	int y,
	float scale_x,
	float scale_y,
	int spr_no,
	CL_DisplayCard *card)
{
	impl->put_screen(x, y, scale_x, scale_y, spr_no, card);
}

void CL_Surface::put_screen(
	int x,
	int y,
	int size_x,
	int size_y,
	int spr_no,
	CL_DisplayCard *card)
{
	impl->put_screen(x, y, size_x, size_y, spr_no, card);
}

void CL_Surface::put_target(
	int x,
	int y,
	int spr_no,
	CL_Target *target)
{
	impl->put_target(x, y, spr_no, target);
}

unsigned int CL_Surface::get_width() const
{
	return impl->get_width();
}

unsigned int CL_Surface::get_height() const
{
	return impl->get_height();
}

unsigned int CL_Surface::get_num_frames() const
{
	return impl->get_num_frames();
}

bool CL_Surface::is_video(CL_DisplayCard *card) const
{
	return impl->is_video(card);
}

bool CL_Surface::is_loaded(CL_DisplayCard *card) const
{
	return impl->is_loaded(card);
}

bool CL_Surface::convert_video(CL_DisplayCard *card)
{
	return impl->convert_video(card);
}

bool CL_Surface::convert_system(CL_DisplayCard *card)
{
	return impl->convert_system(card);
}

void CL_Surface::flush(CL_DisplayCard *card)
{
	impl->flush(card);
}

int CL_Surface::get_reference_count ()
{
	return impl->get_reference_count ();
}
