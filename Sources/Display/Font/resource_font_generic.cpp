/*
	$Id: resource_font_generic.cpp,v 1.20 2002/03/02 19:43:46 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#include "Core/precomp.h"

#include "resource_font_generic.h"
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
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_description.h"
#include "API/Display/Display/surfaceprovider.h"
#include "API/Display/Display/palette.h"
#include "API/Display/Display/res_surface.h"
#include "Display/Display/Generic/resource_surface_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Font Construction:

CL_ResourceData_Font::CL_ResourceData_Font(CL_Resource &resource)
: CL_ResourceData(resource), font(0), font_desc(0)
{
	slot_load_file = resource.sig_load_file().connect(this, &CL_ResourceData_Font::on_load_file);
	slot_load_datafile = resource.sig_load_datafile().connect(this, &CL_ResourceData_Font::on_load_datafile);
	slot_save_datafile = resource.sig_save_datafile().connect(this, &CL_ResourceData_Font::on_save_datafile);
	slot_unload = resource.sig_unload().connect(this, &CL_ResourceData_Font::on_unload);

	resource.attach_data("font", this);
}

CL_ResourceData_Font::~CL_ResourceData_Font()
{
	delete font;
	delete font_desc;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Font Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Font Implementation:

void CL_ResourceData_Font::on_load_file()
{
	CL_Resource resource = get_resource();
	CL_ResourceOptions &options = resource.get_options();

	CL_String ext = CL_String(resource.get_location()).right(4);
	ext.to_lower();

	// the alphafont options should probally be required to differ
	// indexed png's from rgba ones
	if (ext == ".tga" || options.exists("alphafont"))
	{
		if (options.exists("spacelen") == false)
			throw CL_Error("Font missing spacelen option.");

		if (options.exists("trans_limit") == false)
			throw CL_Error("Font missing trans_limit option.");

		if (options.exists("subtract_width") == false)
			throw CL_Error("Font missing subtract_width option.");

		if (options.exists("letters") == false)
			throw CL_Error("Font missing letters option.");

		int space_len = CL_String(options.get_option("spacelen").get_value()).get_as_int();
		float trans_limit = CL_String(options.get_option("trans_limit").get_value()).get_as_float();
		int subtract_width = CL_String(options.get_option("subtract_width").get_value()).get_as_int();
		CL_SurfaceProvider* provider = 0;

		for (std::list<CL_ResourceSource_Surface*>::iterator it = CL_ResourceSource_Surface::surface_sources.begin();
		     it != CL_ResourceSource_Surface::surface_sources.end();
		     it++)
		{
			if ((*it)->can_create (ext.get_string(), options))
			{
				CL_ResourceManager manager = resource.get_manager();
				provider = (*it)->create (resource, options, manager);
				break;
			}
		}
		cl_assert (provider != NULL);

		font_desc = new CL_Font_Description(
			provider,
			trans_limit,
			space_len,
			subtract_width,
			options.get_option("letters").get_value().c_str());
		list_delete.push_back(provider);
	}

	else if(options.get_option("type").get_value() == "TTF")
	{
	//This is really just a cheap hack
	return;
	}

  else
	{
		if (options.exists("x") == false)
			throw CL_Error("Font missing x coordinate option.");

		if (options.exists("y") == false)
			throw CL_Error("Font missing y coordinate option.");

		if (options.exists("spacelen") == false)
			throw CL_Error("Font missing spacelen option.");

		if (options.exists("letters") == false)
			throw CL_Error("Font missing letters option.");

		int x = CL_String(options.get_option("x").get_value()).get_as_int();
		int y = CL_String(options.get_option("y").get_value()).get_as_int();
		int space_len = CL_String(options.get_option("spacelen").get_value()).get_as_int();
		int subtract_width = 0;
		if (options.exists("subtract_width"))
			subtract_width = CL_String(
				options.get_option("subtract_width").get_value()).get_as_int();

		int num_tcols = 1;
		int *tcols = NULL;
		if (num_tcols > 0)
		{
			tcols = new int[num_tcols];
			tcols[0] = 0;
		}

		CL_SurfaceProvider* provider = 0;

		for (std::list<CL_ResourceSource_Surface*>::iterator it = CL_ResourceSource_Surface::surface_sources.begin();
		     it != CL_ResourceSource_Surface::surface_sources.end();
		     it++)
		{
			if ((*it)->can_create (ext.get_string(), options))
			{
				CL_ResourceManager manager = resource.get_manager();
				provider = (*it)->create(resource, options, manager);
				break;
			}
		}
		cl_assert (provider != NULL);

		font_desc = new CL_Font_Description(
			provider,
			x, y,
			tcols, num_tcols,
			space_len,
			subtract_width,
			options.get_option("letters").get_value().c_str());
		list_delete.push_back(provider);
		delete[] tcols;
	}

	font = new CL_Font(font_desc);
}

void CL_ResourceData_Font::on_load_datafile(class CL_InputSourceProvider *input_provider)
{
	CL_InputSource *input = input_provider->open_source(get_resource().get_name().c_str());
	try
	{
		font_desc = new CL_Font_Description(input);
		font = new CL_Font(font_desc);
	}
	catch (...)
	{
		delete input;
		throw;
	}
	delete input;
}

void CL_ResourceData_Font::on_save_datafile(class CL_OutputSourceProvider *output_provider)
{
	CL_OutputSource *output = output_provider->open_source(get_resource().get_name().c_str());
	try
	{
		output->write_int32(font_desc->space_len);
		output->write_int32(font_desc->subtract_width);
		output->write_string(font_desc->letters.c_str());
	/*
		output->write_short16(font_desc->letters.length()+1);
		output->write(font_desc->letters.c_str(), font_desc->letters.length()+1);
	*/	
		unsigned int len = font_desc->letters.length();
		if (len > font_desc->letter_providers.size())
		{
			CL_String err;
			err << "Font loading error - ";
			err << "Letters expected:  " << len;
			err << ", letters available: " << CL_String((int)          font_desc->letter_providers.size());
			//This just makes it palatable to the compiler
			throw CL_Error(err.get_string());
		}
		else if (len < font_desc->letter_providers.size())
		{
			std::cout << "ClanLib Font Warning: Font letters do not match!" << std::endl;
			std::cout << "ClanLib Font Warning: Letters expected:  " << len << std::endl;
			std::cout << "ClanLib Font Warning: Letters available: "
				<< (int)font_desc->letter_providers.size() << std::endl;
		}
		
		if (len > 0)
		{
			font_desc->letter_providers[0]->lock();
		}
		for(unsigned int i=0; i<len; i++)
		{
			CL_SurfaceProvider *provider = font_desc->letter_providers[i];
			cl_assert(provider != NULL);

			provider->lock();

			int width = provider->get_width();
			int height = provider->get_height();
			int no_sprs = provider->get_num_frames();
			int bytes_per_pixel = (provider->get_depth()+7)/8;
			int transcol = provider->uses_src_colorkey() ? (int)provider->get_src_colorkey() : -1;
	//		int format = provider->get_pixel_format();
			int red_mask = provider->get_red_mask();
			int green_mask = provider->get_green_mask();
			int blue_mask = provider->get_blue_mask();
			int alpha_mask = provider->get_alpha_mask();
			int translate_x = provider->get_translate_x();
			int translate_y = provider->get_translate_y();
			int pitch = provider->get_pitch();

			CL_Palette *pal = provider->get_palette();
			unsigned char *data = (unsigned char *) provider->get_data();
			cl_assert(data != NULL);

			output->write_int32(width);
			output->write_int32(height);
			output->write_int32(no_sprs);
			output->write_int32(transcol);
			
			char palette_bool = (pal != NULL) ? 1 : 0;
			output->write_char8(palette_bool);
			
			if (palette_bool) output->write(pal->palette, 256*3);

			char indexed_bool = (provider->is_indexed()) ? 1 : 0;
			output->write_char8(indexed_bool);
			
	//		output->write_int32(format);
			output->write_int32(red_mask);
			output->write_int32(green_mask);
			output->write_int32(blue_mask);
			output->write_int32(alpha_mask);
			output->write_int32(provider->get_depth());

			if (transcol == -1)
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

					output->write(
						data+translate_x*bytes_per_pixel+img_y*pitch,
						bytes_per_pixel*width);
				}
			}

			provider->unlock();
		}
		if (len > 0)
		{
			font_desc->letter_providers[0]->unlock();
		}
	}
	catch (...)
	{
		delete output;
		throw;
	}
	delete output;
}

void CL_ResourceData_Font::on_unload()
{
	delete font;      font = 0;
	delete font_desc; font_desc = 0;
	std::list<CL_SurfaceProvider*>::iterator it;
	for(it = list_delete.begin(); it != list_delete.end(); it++)
		delete *it;
	list_delete.clear();
}
