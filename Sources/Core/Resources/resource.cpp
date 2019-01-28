/*
	$Id: resource.cpp,v 1.17 2002/02/28 22:12:57 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/Resources/resourceoptions.h"
#include "API/Core/Resources/resourcedata.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/IOData/inputsource_provider.h"
#include "API/Core/IOData/inputsource_provider_file.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/System/cl_assert.h"
#include "Core/Resources/resource_manager_file.h"
#include <ctype.h>
#include <algorithm>

/////////////////////////////////////////////////////////////////////////////
// CL_Resource_Generic:

class CL_Resource_Generic
{
public:
	CL_Signal_v0 sig_load_file;
	CL_Signal_v1<CL_InputSourceProvider *> sig_load_datafile;
	CL_Signal_v1<CL_OutputSourceProvider *> sig_save_datafile;
	CL_Signal_v0 sig_unload;
	std::map<std::string, CL_ResourceData*> datas;
	std::string type, name, location;
	CL_ResourceOptions options;
	int ref_count, load_ref;
	CL_ResourceManager_File *manager;

	CL_Resource_Generic()
	: ref_count(0), load_ref(0), manager(NULL)
	{
	}

	~CL_Resource_Generic()
	{
		std::map<std::string, CL_ResourceData*>::iterator it(datas.begin());
		std::map<std::string, CL_ResourceData*>::iterator it_end(datas.end());
		for (; it != it_end; ++it)
			delete it->second;
	}

	void add_ref()
	{
		ref_count++;
	}

	void release_ref()
	{
		ref_count--;
		if (ref_count == 0) delete this;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CL_Resource construction:

CL_Resource::CL_Resource(
	const std::string &type,
	const std::string &name,
	const std::string &location,
	const CL_ResourceOptions &options,
	const CL_ResourceManager &manager)
: impl(NULL)
{
	std::string lower_name = name;
	std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), tolower);

	impl = new CL_Resource_Generic;
	impl->add_ref();
	impl->type = type;
	impl->name = lower_name;
	impl->location = location;
	impl->options = options;
	impl->manager = manager.impl;
}

CL_Resource::CL_Resource(const CL_Resource &copy)
: impl(NULL)
{
	impl = copy.impl;
	if (impl) impl->add_ref();
}

CL_Resource::CL_Resource()
: impl(NULL)
{
	impl = new CL_Resource_Generic;
	impl->add_ref();
}

CL_Resource::CL_Resource(class CL_Resource_Generic *impl)
: impl(impl)
{
	if (impl) impl->add_ref();
}

CL_Resource::~CL_Resource()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Resource attributes:

const std::string &CL_Resource::get_type() const
{
	return impl->type;
}

const std::string &CL_Resource::get_name() const
{
	return impl->name;
}

const std::string &CL_Resource::get_location() const
{
	return impl->location;
}

std::string CL_Resource::get_full_location() const
{
	CL_InputSourceProvider *provider = (CL_InputSourceProvider *) impl->manager->get_resource_provider();
	return provider->get_path(get_location().c_str());
}

CL_ResourceOptions &CL_Resource::get_options()
{
	return impl->options;
}

CL_ResourceManager CL_Resource::get_manager()
{
	return CL_ResourceManager(impl->manager);
}

CL_ResourceData *CL_Resource::get_data(const std::string &name)
{
	return impl->datas[name];
}

int CL_Resource::get_reference_count() const
{
	return impl->load_ref;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Resource signals:

CL_Signal_v0 &CL_Resource::sig_load_file()
{
	return impl->sig_load_file;
}

CL_Signal_v1<CL_InputSourceProvider *> &CL_Resource::sig_load_datafile()
{
	return impl->sig_load_datafile;
}

CL_Signal_v1<CL_OutputSourceProvider *> &CL_Resource::sig_save_datafile()
{
	return impl->sig_save_datafile;
}

CL_Signal_v0 &CL_Resource::sig_unload()
{
	return impl->sig_unload;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Resource operations:

void CL_Resource::operator = (const CL_Resource &copy)
{
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();
}

void CL_Resource::attach_data(const std::string &name, CL_ResourceData *data)
{
	impl->datas[name] = data;
}

void CL_Resource::detach_data(CL_ResourceData *data)
{
	std::map<std::string, CL_ResourceData *>::iterator it;
	for (it = impl->datas.begin(); it != impl->datas.end(); it++)
	{
		if (it->second == data)
		{
			impl->datas.erase(it);
			break;
		}
	}
}

void CL_Resource::unload()
{
	if (!impl) return;
	if (impl->manager == 0) return;

	impl->load_ref--;
	cl_assert(impl->load_ref >= 0);
	if (impl->load_ref == 0)
	{
		impl->sig_unload();
	}
}

void CL_Resource::load()
{
	if (!impl) return;
	if (impl->manager == 0) return;

	if (impl->manager->from_source) load_file();
	else
	{
//		CL_InputSource *inputsource = impl->manager->get_resource_provider()->open_source(get_name().c_str());
		load_datafile(impl->manager->get_resource_provider());
//		delete inputsource;
	}
}

void CL_Resource::load_file()
{
	if (!impl) return;
	impl->load_ref++;
	if (impl->load_ref == 1) impl->sig_load_file();
}

void CL_Resource::load_datafile(CL_InputSourceProvider *input)
{
	if (!impl) return;
	impl->load_ref++;
	if (impl->load_ref == 1)
	{
		impl->sig_load_datafile(input);
	}
}

void CL_Resource::save_datafile(CL_OutputSourceProvider *output)
{
	if (!impl) return;
	impl->sig_save_datafile(output);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Resource implementation:
