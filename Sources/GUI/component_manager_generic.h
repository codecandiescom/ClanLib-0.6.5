/*
	$Id: component_manager_generic.h,v 1.26 2002/01/08 10:17:25 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_component_manager_generic
#define header_component_manager_generic

#include <string>
#include <stack>
#include <vector>
#include <stdio.h>

#include "API/GUI/component_manager.h"
#include "API/GUI/stylemanager.h"
#include "API/GUI/component.h"
#include "API/GUI/component_options.h"
#include "API/GUI/gui_file_parser.h"

class CL_ComponentManager_Generic
{
//!Construction:
public:
	CL_ComponentManager_Generic(
		const std::string &filename,
		CL_ResourceManager *resources,
		CL_StyleManager *style, CL_Component *parent);

	CL_ComponentManager_Generic(
		const std::string &filename,
		bool is_datafile,
		CL_StyleManager *style, CL_Component *parent);
		
	~CL_ComponentManager_Generic();

//!Attributes:
public:
	CL_Component *get_component(const std::string &component_id) const;
	CL_StyleManager *get_style_manager() const { return style_manager; }

	void get_root_component_list(std::vector<CL_Component *> &components);

//!Operations:
public:
	int add_ref() { return ++ref_count; }

	int release_ref()
	{
		ref_count--;
		if (ref_count == 0) { delete this; return 0; }
		return ref_count;
	}

//!Implementation:
private:
	void init_style_manager(const std::string &style_id);
	
	void parse();
	
	void create_components(const CL_GUIFileParser::component_sort_map_t &creation_sort_map);

	CL_InputSource *input;

	std::string filename;
	
	CL_StyleManager *style_manager;
	
	CL_ResourceManager *resources;
	
	CL_Component *parent;

	typedef std::map<std::string, CL_GUIFileParser::ComponentInfo> component_map_t;
	component_map_t component_map;

	int ref_count;
};

#endif
