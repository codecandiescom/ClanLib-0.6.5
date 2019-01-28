/*
	$Id: resourcetype_raw.cpp,v 1.4 2001/12/16 18:24:10 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Raw resource type

*/

#include "Core/precomp.h"
#include "API/Core/Resources/resourcetype_raw.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"
#include "API/Core/IOData/outputsource.h"
#include "API/Core/IOData/outputsource_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Raw construction:

CL_ResourceData_Raw::CL_ResourceData_Raw(CL_Resource &resource)
: CL_ResourceData(resource)
{
	slot_load_file = resource.sig_load_file().connect(this, &CL_ResourceData_Raw::on_load_file);
	slot_load_datafile = resource.sig_load_datafile().connect(this, &CL_ResourceData_Raw::on_load_datafile);
	slot_save_datafile = resource.sig_save_datafile().connect(this, &CL_ResourceData_Raw::on_save_datafile);
	slot_unload = resource.sig_unload().connect(this, &CL_ResourceData_Raw::on_unload);

	resource.attach_data("raw", this);
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Raw implementation:

void CL_ResourceData_Raw::on_load_file()
{
	CL_ResourceManager manager = get_resource().get_manager();
	std::string location = get_resource().get_location();

	CL_InputSource *input = manager.get_resource_provider()->open_source(location.c_str());

	try
	{
		int length = input->size();
		value = std::vector<unsigned char>(length);
		input->read(&value.front(), length);
	}
	catch (...)
	{
		delete input;
		throw;
	}

	delete input;
}

void CL_ResourceData_Raw::on_load_datafile(CL_InputSourceProvider *provider)
{
	CL_InputSource *input = provider->open_source(get_resource().get_name().c_str());

	int length = input->read_int32();
	value = std::vector<unsigned char>(length);
	input->read(&value.front(), length);

	delete input;
}

void CL_ResourceData_Raw::on_save_datafile(CL_OutputSourceProvider *provider)
{
	CL_OutputSource *output = provider->open_source(get_resource().get_name().c_str());

	int length = value.size();
	output->write_int32(length);
	output->write(&value.front(), length);

	delete output;
}

void CL_ResourceData_Raw::on_unload()
{
	value = std::vector<unsigned char>();
}
