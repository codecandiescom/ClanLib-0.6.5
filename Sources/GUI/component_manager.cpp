/*
	$Id: component_manager.cpp,v 1.3 2001/12/15 21:26:30 starch Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/GUI/component_manager.h"
#include "component_manager_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentManager construction:

CL_ComponentManager *CL_ComponentManager::create(
	const std::string &resource_id,
	CL_ResourceManager *res_manager,
	CL_StyleManager *style, 
	CL_Component *parent)
{
	return new CL_ComponentManager(resource_id, res_manager, style, parent);
}

CL_ComponentManager *CL_ComponentManager::create(
	const std::string &filename,
	bool is_datafile,
	CL_StyleManager *style, 
	CL_Component *parent)
{
	return new CL_ComponentManager(filename, is_datafile, style, parent);
}

CL_ComponentManager::CL_ComponentManager(
	const std::string &resource_id,
	CL_ResourceManager *res_manager,
	CL_StyleManager *style, 
	CL_Component *parent)
: impl(0)
{
	impl = new CL_ComponentManager_Generic(resource_id, res_manager, style, parent);
	impl->add_ref();
}

CL_ComponentManager::CL_ComponentManager(
	const std::string &resource_id,
	CL_Component *parent)
: impl(0)
{
	impl = new CL_ComponentManager_Generic(
		resource_id,
		parent->get_style_manager()->get_resources(),
		parent->get_style_manager(),
		parent);
	impl->add_ref();
}

CL_ComponentManager::CL_ComponentManager(
	const std::string &filename,
	bool is_datafile,
	CL_StyleManager *style, 
	CL_Component *parent)
: impl(0)
{
	impl = new CL_ComponentManager_Generic(filename, is_datafile, style, parent);
	impl->add_ref();
}

CL_ComponentManager::CL_ComponentManager(const CL_ComponentManager &copy)
: impl(copy.impl)
{
	if (impl) impl->add_ref();
}

CL_ComponentManager::~CL_ComponentManager()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentManager attributes:

std::map<std::string, CL_ComponentType *> CL_ComponentManager::component_types;

CL_Component *CL_ComponentManager::get_component(const std::string &name) const
{
	return impl->get_component(name);
}

CL_StyleManager *CL_ComponentManager::get_style_manager() const
{
	return impl->get_style_manager();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentManager operations:

CL_ComponentManager &CL_ComponentManager::operator =(const CL_ComponentManager &copy)
{
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentManager implementation:
