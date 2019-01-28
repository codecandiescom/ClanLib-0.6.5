/*
	$Id: gui_file_parser.h,v 1.5 2002/02/28 21:36:50 japj Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_gui_file_parser
#define header_gui_file_parser

//! clanGUI="Framework"
//! header=gui.h

#include <string>
#include <list>
#include <map>
#include "component_options.h"

class CL_ComponentType;
class CL_Component;
class CL_InputSource;

//: GUI File Parser Class
class CL_GUIFileParser
{
//! Construction:	
public:
	//: File Parser Constructor
	CL_GUIFileParser(const std::string &filename, CL_InputSource *input);

	struct ComponentInfo
	{
	public:
		ComponentInfo()
		{
			type = "";
			component_type = NULL;
			component = NULL;
		}

		std::string name, type;

		CL_ComponentType *component_type;
		
		CL_ComponentOptions options;
		
		CL_Component *component;
		
		std::list<ComponentInfo *> children;
	};

	typedef std::map<std::string, ComponentInfo> component_map_t;
	typedef std::multimap<int, ComponentInfo *> component_sort_map_t;
	typedef std::pair<std::string, std::string> variable_pair_t;
	typedef std::list<variable_pair_t> globals_t;

//! Attributes:
	component_map_t &get_components() { return m_component_map; }
	component_sort_map_t &get_sorted_components() { return m_component_sort_map; }
	globals_t &globals() { return m_globals; }
	
private:
//! Implementation:
	void parse();
	std::string preprocess_templates();
	std::string concatenate_stringlist(std::list<std::string> &expression);
	std::string evaluate_expression(std::list<std::string> &expression);
	CL_ComponentType *is_component_type(const std::string &tok);

	std::string filename;
	CL_InputSource *input;

	component_map_t m_component_map;
	component_sort_map_t m_component_sort_map;
	std::list<variable_pair_t> m_globals;
};

#endif
