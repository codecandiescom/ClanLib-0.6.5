
#pragma warning(disable:4786)

#include "document.h"
#include "component_node.h"
#include <ClanLib/Core/IOData/inputsource_provider.h>

/////////////////////////////////////////////////////////////////////////////
// Document construction:

Document::Document()
{
	load("test.gui");
}

Document::~Document()
{
}

/////////////////////////////////////////////////////////////////////////////
// Document attributes:


/////////////////////////////////////////////////////////////////////////////
// Document operations:

void Document::clear()
{
}

void Document::load(const std::string &gui_file)
{
	CL_GUIFileParser parser(
		gui_file, 
		CL_InputSourceProvider::create_file_provider(".")->open_source(gui_file));

	CL_GUIFileParser::component_sort_map_t::iterator it;
	it = parser.get_sorted_components().find(0);
	while (it != parser.get_sorted_components().end())
	{
		// Only enumerate first level components:
		if (it->first != 0) break;

		CL_GUIFileParser::ComponentInfo *info = it->second;
		ComponentNode *node = new ComponentNode(info->name, info->type, info->options, this);
		nodes.push_back(node);

		std::list<CL_GUIFileParser::ComponentInfo *>::iterator it_child;
		for (it_child = info->children.begin(); it_child != info->children.end(); it_child++)
		{
			add_node_child(node, *it_child);
		}

		it++;
	}
}

void Document::save(const std::string &gui_file)
{
}

ComponentNode *Document::add_node(const std::string &type)
{
	CL_ComponentOptions options;
	ComponentNode *node = new ComponentNode("", type, options, this);
	nodes.push_back(node);
	sig_node_added(node);
	return node;
}

void Document::remove_node(ComponentNode *node)
{
	nodes.remove(node);
	sig_node_removed(node);
	delete node;
}

/////////////////////////////////////////////////////////////////////////////
// Document signals:


/////////////////////////////////////////////////////////////////////////////
// Document implementation:

void Document::add_node_child(ComponentNode *parent, CL_GUIFileParser::ComponentInfo *info)
{
	ComponentNode *node = new ComponentNode(info->name, info->type, info->options, parent);
	parent->nodes.push_back(node);

	std::list<CL_GUIFileParser::ComponentInfo *>::iterator it_child;
	for (it_child = info->children.begin(); it_child != info->children.end(); it_child++)
	{
		add_node_child(node, *it_child);
	}
}
