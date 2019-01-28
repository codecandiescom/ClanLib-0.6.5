/*
	$Id: setupdisplay_generic.cpp,v 1.14 2001/11/30 11:05:19 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#include "Core/precomp.h"

#include <cstdlib>
#include "Display/Display/Generic/resource_surface_generic.h"
#include "Display/Font/resource_font_generic.h"
#include "API/Core/Resources/resourceoptions.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/Resources/resourcetype.h"
#include "API/Display/SurfaceProviders/provider_pcx.h"
#include "API/Display/SurfaceProviders/provider_bmp.h"
#include "API/Display/SurfaceProviders/provider_targa.h"
#include "API/Display/Display/res_surface.h"

#include "setupdisplay_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Resource support: (sorry for the messy code)

static class CL_PCX_ResourceSource : public CL_ResourceSource_Surface
{
public:
	virtual const char *get_name() { return "pcx"; }

	virtual bool can_create(
		std::string ext,
		CL_ResourceOptions &options)
	{
		if (ext == ".pcx") return true;
		if (options.exists("pcx")) return true;
		
		return false;
	}
	
	virtual CL_SurfaceProvider *create(
		CL_Resource &resource,
		CL_ResourceOptions &options,
		CL_ResourceManager &parent)
	{
		CL_InputSourceProvider *provider =
			parent.get_resource_provider();

		return new CL_PCXProvider(resource.get_location().c_str(), provider);
	}

	virtual CL_SurfaceProvider *create(
		const std::string &filename)
	{
		return new CL_PCXProvider(filename.c_str(), NULL);
	}

} *pcx_resource_source = NULL;

static class CL_BMP_ResourceSource : public CL_ResourceSource_Surface
{
public:
	virtual const char *get_name() { return "bmp"; }

	virtual bool can_create(
		std::string ext,
		CL_ResourceOptions &options)
	{
		if (ext == ".bmp") return true;
		if (options.exists("bmp")) return true;
		
		return false;
	}
	
	virtual CL_SurfaceProvider *create(
		CL_Resource &resource,
		CL_ResourceOptions &options,
		CL_ResourceManager &parent)
	{
		CL_InputSourceProvider *provider =
			parent.get_resource_provider();

		return new CL_BMPProvider(resource.get_location().c_str(), provider);
	}

	virtual CL_SurfaceProvider *create(
		const std::string &filename)
	{
		return new CL_BMPProvider(filename.c_str(), NULL);
	}

} *bmp_resource_source = NULL;

static class CL_Targa_ResourceSource : public CL_ResourceSource_Surface
{
public:
	virtual const char *get_name() { return "tga"; }

	virtual bool can_create(
		std::string ext,
		CL_ResourceOptions &options)
	{
		if (ext == ".tga") return true;
		if (options.exists("tga")) return true;

		return false;
	}

	virtual CL_SurfaceProvider *create(
		CL_Resource &resource,
		CL_ResourceOptions &options,
		CL_ResourceManager &parent)
	{
		CL_InputSourceProvider *provider =
			parent.get_resource_provider();

		return new CL_TargaProvider(resource.get_location().c_str(), provider);
	}

	virtual CL_SurfaceProvider *create(
		const std::string &filename)
	{
		return new CL_TargaProvider(filename.c_str(), NULL);
	}

} *targa_resource_source = NULL;

/////////////////////////////////////////////////////////////////////////////
// CL_SetupDisplay_Generic initialization:

static CL_RegisterResourceType<CL_ResourceData_Surface> *restype_surface = NULL;
static CL_RegisterResourceType<CL_ResourceData_Font> *restype_font = NULL;

void CL_SetupDisplay_Generic::init()
{
	restype_surface = new CL_RegisterResourceType<CL_ResourceData_Surface>("surface");
	restype_font = new CL_RegisterResourceType<CL_ResourceData_Font>("font");

	pcx_resource_source = new CL_PCX_ResourceSource;
	bmp_resource_source = new CL_BMP_ResourceSource;
	targa_resource_source = new CL_Targa_ResourceSource;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SetupDisplay_Generic deinitialization:

void CL_SetupDisplay_Generic::deinit()
{
	delete pcx_resource_source;
	delete bmp_resource_source;
	delete targa_resource_source;

	pcx_resource_source = NULL;
	bmp_resource_source = NULL;
	targa_resource_source = NULL;

	delete restype_surface;
	delete restype_font;

	restype_surface = NULL;
	restype_font = NULL;
}
