/*
	$Id: component_manager_generic.cpp,v 1.37 2001/12/31 01:31:42 starch Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/Resources/resourcetype_raw.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"
#include "API/GUI/component_style.h"
#include "component_manager_generic.h"
#include "component_tokenizer.h"
#include "componentfile_preprocess.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentManager_Generic construction:

CL_ComponentManager_Generic::CL_ComponentManager_Generic(
	const std::string &res_id,
	CL_ResourceManager *resources,
	CL_StyleManager *style,
	CL_Component *parent)
: input(0), filename(filename), style_manager(style), resources(resources), parent(parent), ref_count(0)
{
	input = new CL_InputSource_Raw(res_id, resources);
	parse();
}

CL_ComponentManager_Generic::CL_ComponentManager_Generic(
	const std::string &filename,
	bool is_datafile,
	CL_StyleManager *style,
	CL_Component *parent)
: input(0), filename(filename), style_manager(style), parent(parent), ref_count(0)
{
	CL_InputSourceProvider *provider = 0;
	if (is_datafile) provider = CL_InputSourceProvider::create_datafile_provider(filename);
	else provider = CL_InputSourceProvider::create_file_provider(".");

	input = provider->open_source(filename);
	delete provider;
	
	parse();
}

CL_ComponentManager_Generic::~CL_ComponentManager_Generic()
{
	std::vector<CL_Component *> components;
	get_root_component_list(components);
	
	std::vector<CL_Component *>::iterator it;
	for (it = components.begin(); it != components.end(); it++)
		delete *it;

	delete input;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentManager_Generic attributes:

CL_Component *CL_ComponentManager_Generic::get_component(const std::string &component_id) const
{
	component_map_t::const_iterator it = component_map.find(component_id);
	if (it != component_map.end()) 
	{
		return (*it).second.component;
	}
	throw CL_Error("Unable to locate component");
}

void CL_ComponentManager_Generic::get_root_component_list(std::vector<CL_Component *> &components)
{
	component_map_t::iterator it;
	for (it = component_map.begin(); it != component_map.end();++it)
	{
		if ((*it).second.component->get_parent() == NULL)
		{
			components.push_back((*it).second.component);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentManager_Generic operations:

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentManager_Generic implementation:

void CL_ComponentManager_Generic::init_style_manager(const std::string &style_id)
{
/*
	if (style_id == NULL)
	{
		style_manager = CL_StyleManager::default_style_manager;
	}
	else
	{
		std::map<std::string, CL_StyleManager *>::iterator style_it = CL_StyleManager::style_managers.find(style_id);
		if (style_it == CL_StyleManager::style_managers.end()) throw CL_Error("Specified style manager does not exist");
		style_manager = (*style_it).second;
	}
*/
}

void CL_ComponentManager_Generic::create_components(const CL_GUIFileParser::component_sort_map_t &creation_sort_map)
{
	CL_GUIFileParser::component_sort_map_t::const_iterator it;
	for (it=creation_sort_map.begin();it!=creation_sort_map.end();it++)
	{
		CL_GUIFileParser::ComponentInfo *info = (*it).second;

		if (info->component == NULL)
		{
			info->component = style_manager->create_component(
				info->type,
				parent);

			info->component->sig_set_options()(info->options);
		}

		if (!info->children.empty())
		{
			if (!info->component_type->is_container()) 
			{
				throw CL_Error("Internal component manager error");
			}

			std::list<CL_GUIFileParser::ComponentInfo *>::const_iterator c_it;
			for (c_it=info->children.begin();c_it!=info->children.end();c_it++)
			{
				CL_GUIFileParser::ComponentInfo *child_info = *c_it;
				if (child_info->component == NULL)
				{
					child_info->component = style_manager->create_component(
						child_info->type,
						info->component);

					child_info->component->sig_set_options()(child_info->options);
				}

				info->component->add_child(child_info->component, true);
			}
			info->children.clear();
		}
	}
}

void CL_ComponentManager_Generic::parse()
{
	CL_GUIFileParser parser(filename, input);
	create_components(parser.get_sorted_components());
	component_map.swap(parser.get_components());
}


