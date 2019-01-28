/*
	$Id: resourcetype_module.cpp,v 1.6 2001/12/16 19:18:09 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Module resource type

*/

#include "Core/precomp.h"
#include "resourcetype_module.h"
#include "module_reader.h"
#include <API/Core/System/cl_assert.h>
#include <API/Core/Resources/resource.h>

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Module construction:

CL_ResourceData_Module::CL_ResourceData_Module(CL_Resource &resource)
	: CL_ResourceData(resource)
{
	module=NULL;

	slot_load_file = resource.sig_load_file().connect(
		this, &CL_ResourceData_Module::on_load_file);

	slot_load_datafile = resource.sig_load_datafile().connect(
		this, &CL_ResourceData_Module::on_load_datafile);

	slot_save_datafile = resource.sig_save_datafile().connect(
		this, &CL_ResourceData_Module::on_save_datafile);

	slot_unload = resource.sig_unload().connect(
		this, &CL_ResourceData_Module::on_unload);

	resource.attach_data("module", this);
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Module implementation:

void CL_ResourceData_Module::on_load_file()
{
	CL_ResourceManager manager = get_resource().get_manager();
	std::string location = get_resource().get_location();

	CL_InputSource *input = manager.get_resource_provider()->open_source(location.c_str());

	try
	{
		MREADER *reader=new_clanlib_reader((void *) input);
		cl_assert ( reader != NULL );

		module = Player_LoadGeneric(reader,CLANLIB_READER_CHANNELS,0);

		if (module == NULL)
		{
			throw CL_Error(MikMod_strerror(MikMod_errno));
		}
		delete_clanlib_reader(reader);
	}
	catch (...)
	{
		delete input;
		throw;
	}

	delete input;
}

void CL_ResourceData_Module::on_load_datafile(CL_InputSourceProvider *provider)
{
	CL_InputSource *input = provider->open_source(get_resource().get_name().c_str());

	try
	{
		MREADER *reader=new_clanlib_reader((void *) input);
		cl_assert ( reader != NULL );

		module = Player_LoadGeneric(reader,CLANLIB_READER_CHANNELS,0);

		if (module == NULL)
		{
			throw CL_Error(MikMod_strerror(MikMod_errno));
		}
		delete_clanlib_reader(reader);
	}
	catch (...)
	{
		delete input;
		throw;
	}

	delete input;
}

void CL_ResourceData_Module::on_save_datafile(CL_OutputSourceProvider *provider)
{
	CL_ResourceManager manager = get_resource().get_manager();
	std::string location = get_resource().get_location();

	CL_InputSource *input = manager.get_resource_provider()->open_source(location.c_str());
	CL_OutputSource *output = provider->open_source(get_resource().get_name().c_str());

	try
	{
		/*
		 * We do not use the module stored in memory,
		 * instead we re-read the file from the disk
		 * and copy it "as is" in the datafile.
		 * We *need* to do this since libMikMod does not
		 * know how to save a file... Therefore we store
		 * the genuine .mod or .xm file in the .dat file,
		 * and will call the MikMod reader again when
		 * we read the module from the datafile.
		 */
		int length = input->size();
		unsigned char *buffer = new unsigned char[length];
		cl_assert (buffer != NULL);
		input->read(buffer, length);
		output->write(buffer, length);
	}
	catch (...)
	{
		delete output;
		delete input;
		throw;
	}

	delete output;
	delete input;
}

void CL_ResourceData_Module::on_unload()
{
	if (module)
	{
		Player_Free(module);
		module=NULL;
	}
}
