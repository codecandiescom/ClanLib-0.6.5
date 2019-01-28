/*
	$Id: resourcetype_ttf.cpp,v 1.16 2002/01/15 16:18:48 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#include "Core/precomp.h"
#include "../Display/Font/resourcetype_ttf.h"
#include <API/Core/IOData/inputsource.h>
#include <API/Display/Font/font.h>
#include <API/Core/System/cl_assert.h>
#include <API/Core/Resources/resourceoptions.h>
#include <API/Core/Resources/resource.h>
#include "../Display/Font/resource_font_generic.h"

#include "TTF/font_ttf.h"


CL_ResourceType_TTF::CL_ResourceType_TTF() : CL_ResourceType("TTF")
{
}

void CL_ResourceType_TTF::connect_data(CL_Resource &resource)
{
	resource.attach_data("font",create_from_location(resource));
}



CL_ResourceData *CL_ResourceType_TTF::create_from_location(CL_Resource &resource)
{
	bool is_font_type = false;
	if(resource.get_options().exists("type"))
        {
		if (resource.get_options().get_option("type").get_value() != "TTF") return NULL;
		  is_font_type = true;
 	}

	if (is_font_type)
	{
		return new CL_Font_Resource_TTF(resource);
	}

	return NULL;
}


CL_Font_Resource_TTF::CL_Font_Resource_TTF(CL_Resource &resource)
 	: CL_ResourceData_Font(resource)
{
 //	this->options = options;
 //	this->parent = parent;
 //	from_datafile = false;
 //	load_count = 0;

 	slot_load_file = resource.sig_load_file().connect(
		this, &CL_Font_Resource_TTF::on_load_file);

 	slot_load_datafile = resource.sig_load_datafile().connect(
		this, &CL_Font_Resource_TTF::on_load_datafile);

	slot_save_datafile = resource.sig_save_datafile().connect(
		this, &CL_Font_Resource_TTF::on_save_datafile);

 	slot_unload = resource.sig_unload().connect(
		this, &CL_Font_Resource_TTF::on_unload);

	resource.attach_data("font", this);

	font = NULL;
 	font_desc = NULL;
}
 /*
CL_Font_Resource_TTF::~CL_Font_Resource_TTF()
{
	delete font; font = 0;
  delete ttf; ttf = 0;
}  */

void CL_Font_Resource_TTF::on_load_file()
{
	CL_Resource resource = get_resource();
	ttf = new CL_Font_TTF(resource.get_full_location(),NULL);

	font = new CL_Font(ttf);
	font_desc = NULL;
}

void CL_Font_Resource_TTF::on_load_datafile(CL_InputSourceProvider *input_provider)
{
	CL_Resource resource = get_resource();
	ttf = new CL_Font_TTF(resource.get_location(),input_provider);
	font = new CL_Font(ttf);
	font_desc = NULL;
}

void CL_Font_Resource_TTF::on_save_datafile(class CL_OutputSourceProvider *output_provider)
{
	cl_assert(false);
}

void CL_Font_Resource_TTF::on_unload()
{
	delete font; font = 0;
}



