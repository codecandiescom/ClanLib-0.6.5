/*
	$Id: resource_surface_generic.cpp,v 1.13 2002/02/03 20:56:06 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#include "Core/precomp.h"
#include "resource_surface_generic.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/Resources/resourceoptions.h"
#include "API/Core/Resources/resourceoption.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"
#include "API/Core/IOData/outputsource.h"
#include "API/Core/IOData/outputsource_provider.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/System/error.h"
#include "API/Core/System/cl_assert.h"
#include "API/Display/Display/surface.h"
#include "API/Display/Display/surfaceprovider.h"
#include "API/Display/Display/res_surface.h"
#include "API/Display/Display/palette.h"
#include <API/Display/SurfaceProviders/sprite.h>
#include <API/Display/SurfaceProviders/sprite_subarray_provider.h>
#include <API/Display/SurfaceProviders/sprite_subsection_provider.h>
#include "Display/SurfaceProviders/masktranscol_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Surface construction:

CL_ResourceData_Surface::CL_ResourceData_Surface(class CL_Resource &resource)
: CL_ResourceData(resource), surface(0)
{
	slot_load_file = resource.sig_load_file().connect(this, &CL_ResourceData_Surface::on_load_file);
	slot_load_datafile = resource.sig_load_datafile().connect(this, &CL_ResourceData_Surface::on_load_datafile);
	slot_save_datafile = resource.sig_save_datafile().connect(this, &CL_ResourceData_Surface::on_save_datafile);
	slot_unload = resource.sig_unload().connect(this, &CL_ResourceData_Surface::on_unload);

	resource.attach_data("surface", this);
}

CL_ResourceData_Surface::~CL_ResourceData_Surface()
{
	delete surface;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Surface attributes:

std::list<CL_ResourceSource_Surface*> CL_ResourceSource_Surface::surface_sources;

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Surface implementation:

void CL_ResourceData_Surface::on_load_file()
{
	CL_Resource resource = get_resource();
	CL_ResourceOptions &options = resource.get_options();

	CL_SurfaceProvider *provider = 0;

	CL_String ext = CL_String(resource.get_location()).right(4);
	ext.to_lower();

	for (
		std::list<CL_ResourceSource_Surface*>::iterator it = CL_ResourceSource_Surface::surface_sources.begin();
		it != CL_ResourceSource_Surface::surface_sources.end();
		it++)
	{
		if (options.exists((*it)->get_name()))
		{
			if ((*it)->can_create(ext.get_string(), options))
			{
				CL_ResourceManager manager = resource.get_manager();
				provider = (*it)->create(resource, options, manager);
				break;
			}
			else
			{
				CL_String err;
				err << "Surface provider " << (*it)->get_name() << " cannot handle input format!";
				throw CL_Error(err.get_string());
			}
		}
	}
	
	if (provider == NULL)
	{
		for (
			std::list<CL_ResourceSource_Surface*>::iterator it = CL_ResourceSource_Surface::surface_sources.begin();
			it != CL_ResourceSource_Surface::surface_sources.end();
			it++)
		{
			if ((*it)->can_create(ext.get_string(), options))
			{
				CL_ResourceManager manager = resource.get_manager();
				provider = (*it)->create(resource, options, manager);
				break;
			}
		}
	}
	if (provider == NULL)
	{
		throw CL_Error("Unknown surface image format");
	}

	if (options.exists("x") && options.exists("y") &&
		options.exists("width") && options.exists("height"))
	{
		if (options.exists("array"))
		{
			CL_String array_option = options.get_option("array").get_value();
			int sep = array_option.find('x');
			if (sep == -1)
			{
				throw CL_Error("Invalid 'array' parameter on surface resource");
			}

			provider = new CL_SpriteSubarrayProvider(
				provider,
				CL_String(options.get_option("x").get_value()).get_as_int(),
				CL_String(options.get_option("y").get_value()).get_as_int(),
				CL_String(options.get_option("width").get_value()).get_as_int(),
				CL_String(options.get_option("height").get_value()).get_as_int(),
				CL_String(options.get_option("array").get_value()).mid(0, sep).get_as_int(),
				CL_String(options.get_option("array").get_value()).mid(sep+1).get_as_int());
		}
		else
		{
			provider = new CL_SpriteSubsectionProvider(
				provider,
				CL_String(options.get_option("x").get_value()).get_as_int(),
				CL_String(options.get_option("y").get_value()).get_as_int(),
				CL_String(options.get_option("width").get_value()).get_as_int(),
				CL_String(options.get_option("height").get_value()).get_as_int());
		}
	}

	if (options.exists("tcol"))
	{
		CL_ResourceOption &tcol_option = options.get_option("tcol");
		if (tcol_option.get_values().size() > 1)
		{
			std::vector<std::string> &tcol_list = tcol_option.get_values();
			unsigned int *trans_table = new unsigned int[tcol_list.size()];

			int pos = 0;
			for (
				std::vector<std::string>::iterator it = tcol_list.begin();
				it != tcol_list.end();
				it++)
			{
				int tval = CL_String(*it).get_as_int();
				{
					trans_table[pos++] = tval;
				}
			}

			provider = new CL_MaskTranscolProvider(
					provider,
					true,
					trans_table,
					tcol_list.size());
			delete[] trans_table;
		}
		else
		{
			CL_String tcol = tcol_option.get_value();
			int tval = tcol.get_as_int();
			if (tval >= 0 && tval < 256)
			{
				unsigned int v = tval;
				provider = new CL_MaskTranscolProvider(
						provider,
						true,
						&v,
						1);
			}
		}
	}

	surface = new CL_Surface(provider, true, false);
}

void CL_ResourceData_Surface::on_load_datafile(class CL_InputSourceProvider *input_provider)
{
	//NOT USED?? CL_InputSource *input = input_provider->open_source(get_resource().get_name().c_str());

	CL_SurfaceProvider *provider =
		new CL_SpriteProvider(
			get_resource().get_name().c_str(),
			get_resource().get_manager().get_resource_provider());

	surface = new CL_Surface(provider, true, false);
}

void CL_ResourceData_Surface::on_save_datafile(class CL_OutputSourceProvider *output_provider)
{
	cl_assert(surface != 0);

	CL_OutputSource *output = output_provider->open_source(get_resource().get_name().c_str());

	CL_SurfaceProvider *provider = surface->get_provider();
	provider->lock();

	int width = provider->get_width();
	int height = provider->get_height();
	int no_sprs = provider->get_num_frames();
	int bytes_per_pixel = (provider->get_depth()+7)/8;
	int transcol = provider->uses_src_colorkey() ? (int) provider->get_src_colorkey() : -1;
	int red_mask = provider->get_red_mask();
	int green_mask = provider->get_green_mask();
	int blue_mask = provider->get_blue_mask();
	int alpha_mask = provider->get_alpha_mask();
	int depth = provider->get_depth();
//	int format = provider->get_pixel_format();
	int translate_x = provider->get_translate_x();
	int translate_y = provider->get_translate_y();
	int pitch = provider->get_pitch();

	CL_Palette *pal = provider->get_palette();
	unsigned char *data = (unsigned char *) provider->get_data();
	cl_assert(data != 0);

	output->write_int32(width);
	output->write_int32(height);
	output->write_int32(no_sprs);
	output->write_int32(transcol);
	
	char palette_bool = (pal != 0) ? 1 : 0;
	output->write_char8(palette_bool);
	
	if (palette_bool) output->write(pal->palette, 256*3);

	char indexed_bool = (provider->is_indexed()) ? 1 : 0;
	output->write_char8(indexed_bool);
	
//	output->write_int32(format);
	output->write_int32(red_mask);
	output->write_int32(green_mask);
	output->write_int32(blue_mask);
	output->write_int32(alpha_mask);
	output->write_int32(depth);

	if (!provider->uses_src_colorkey())
	{
		int bytes_pr_line = width*bytes_per_pixel;
		for (int y=0; y<height*no_sprs; y++)
		{
			int img_y = y+translate_y;

			output->write(
				data+translate_x*bytes_per_pixel+img_y*pitch,
				bytes_pr_line);
		}
	}
	else
	{
		for (int y=0; y<height*no_sprs; y++)
		{
			int img_y = y+translate_y;

			output->write(data+translate_x*bytes_per_pixel+img_y*pitch, bytes_per_pixel*width);
		}
	}

	provider->unlock();

	delete output;
}

void CL_ResourceData_Surface::on_unload()
{
	delete surface;
	surface = 0;
}
