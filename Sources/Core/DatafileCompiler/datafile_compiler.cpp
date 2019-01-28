/*
	$Id: datafile_compiler.cpp,v 1.17 2002/02/03 20:56:05 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h" // visual c++: precompiled header file MUST be mentioned FIRST.

#include <iostream>
#include <string>
#include <stdio.h>

#ifdef BORLAND
#pragma hdrstop
//This will speed up the Borland compile when using PCH
#endif

#include <Core/Resources/resource_manager_file.h>
#include <API/Core/System/clanstring.h>
#include <API/Core/IOData/inputsource.h>
#include <API/Core/IOData/inputsource_provider.h>
#include <API/Core/IOData/inputsource_provider_file.h>
#include "API/Core/Resources/datafile_compiler.h"
#include "datafile_writer.h"

void CL_DatafileCompiler::write(
	const std::string &scr_file,
	const std::string &dat_file)
{
	std::cout << std::endl << "ClanLib Datafile Compiler" << std::endl << std::endl;

  	CL_String input_file(scr_file);
	CL_String output_file(dat_file);
			
	std::cout << "Input file: " << input_file.get_string() << ", output file: " << output_file.get_string() << std::endl;
	CL_ResourceManager *manager = new CL_ResourceManager(input_file, false);

	OutputSourceProvider_Datafile output(output_file.get_string());

	// Write the config-file to the datafile
	{
		CL_OutputSource *config_osource = output.open_source(default_scriptfile_id);

		CL_InputSourceProvider *file_provider = CL_InputSourceProvider::create_file_provider("");
		CL_InputSource *config_file = file_provider->open_source(input_file);

		int total_size = config_file->size();
		char *config_file_data = new char[total_size];
		config_file->read(config_file_data, total_size);
		config_osource->write(config_file_data, total_size);

		delete[] config_file_data;
		delete file_provider;
		delete config_file;
		delete config_osource;
	}

	std::list<std::string> *resource_list = manager->get_all_resources();
	for (
		std::list<std::string>::iterator it = resource_list->begin();
		it != resource_list->end();
		it++)
	{
		CL_Resource res = manager->get_resource(*it);

		if (res.get_options().exists("uncompressed")) output.set_compression(false);
		else output.set_compression();

		std::cout << "Writing resource '" << it->c_str() << "'" << std::endl;
		res.load();
		res.save_datafile(&output);
		res.unload();
	}
	delete resource_list;
	delete manager;
}
