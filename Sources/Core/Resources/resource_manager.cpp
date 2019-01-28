/*
	$Id: resource_manager.cpp,v 1.9 2001/12/15 01:30:07 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <iostream>
#include "API/Core/Resources/resource_manager.h"
#include "resource_manager_file.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceManager construction:

CL_ResourceManager::CL_ResourceManager(
	const std::string &config_file,
	CL_InputSourceProvider *provider,
	bool read_directly_from_source,
	bool delete_inputsource_provider)
: impl(NULL)
{
	impl = new CL_ResourceManager_File(
		config_file,
		provider,
		read_directly_from_source,
		delete_inputsource_provider);
}

CL_ResourceManager::CL_ResourceManager(
	const std::string &config_file,
	const bool is_datafile)
: impl(NULL)
{
	impl = new CL_ResourceManager_File(
		config_file,
		is_datafile);
}

CL_ResourceManager::CL_ResourceManager(
	const std::string &config_file,
	const bool is_datafile,
	CL_ResourceManager &additional_resources)
: impl(NULL)
{
	impl = new CL_ResourceManager_File(
		config_file,
		is_datafile);
	
	add_resources(additional_resources);
}

CL_ResourceManager::CL_ResourceManager(const CL_ResourceManager &copy)
: impl(NULL)
{
	impl = copy.impl;
	impl->add_ref();
}

CL_ResourceManager::CL_ResourceManager()
: impl(NULL)
{
	impl = new CL_ResourceManager_File;
}

CL_ResourceManager::CL_ResourceManager(class CL_ResourceManager_File *impl)
: impl(impl)
{
	if (impl != NULL) impl->add_ref();
}

CL_ResourceManager::~CL_ResourceManager()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceManager attributes:

CL_Resource &CL_ResourceManager::get_resource(const std::string &res_id)
{
	return impl->get_resource(res_id);
}

std::list<std::string> *CL_ResourceManager::get_all_resources()
{
	return impl->get_all_resources();
}

std::list<std::string> *CL_ResourceManager::get_resources_of_type(const std::string &type_id)
{
	return impl->get_resources_of_type(type_id);
}

CL_InputSourceProvider *CL_ResourceManager::get_resource_provider() const
{
	return impl->get_resource_provider();
}

bool CL_ResourceManager::is_from_source()
{
	return impl->from_source;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceManager operations:

void CL_ResourceManager::operator = (const CL_ResourceManager &copy)
{
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();
}

void CL_ResourceManager::add_resources(const CL_ResourceManager &additional_resources)
{
	impl->add_resources(additional_resources.impl);
}

void CL_ResourceManager::remove_resources(const CL_ResourceManager &additional_resources)
{
	impl->remove_resources(additional_resources.impl);
}

void CL_ResourceManager::load_all()
{
	impl->load_all();
}

void CL_ResourceManager::unload_all()
{
	impl->unload_all();
}

void CL_ResourceManager::load_section(const std::string &section_name)
{
	impl->load_section(section_name);
}

void CL_ResourceManager::unload_section(const std::string &section_name)
{
	impl->unload_section(section_name);
}
