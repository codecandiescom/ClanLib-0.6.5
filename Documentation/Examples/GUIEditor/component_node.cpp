
#pragma warning(disable:4786)

#include "component_node.h"
#include "document.h"
#include <ClanLib/GUI/component_manager.h>

/////////////////////////////////////////////////////////////////////////////
// ComponentNode construction:

ComponentNode::ComponentNode(const std::string &name, const std::string &type, const CL_ComponentOptions &options, ComponentNode *parent)
: name(name), type(type), parent(parent), document(parent->document), options(options)
{
}

ComponentNode::ComponentNode(const std::string &name, const std::string &type, const CL_ComponentOptions &options, Document *document)
: name(name), type(type), parent(0), document(document), options(options)
{
}

ComponentNode::~ComponentNode()
{
}

/////////////////////////////////////////////////////////////////////////////
// ComponentNode attributes:

const CL_ComponentOptions &ComponentNode::get_options() const
{
	return options;
}

const std::string &ComponentNode::get_type() const
{
	return type;
}

CL_ComponentType *ComponentNode::find_component_type()
{
	CL_ComponentManager::component_type_map_t::iterator it;

	it = CL_ComponentManager::component_types.find(type);
	if (it == CL_ComponentManager::component_types.end()) return 0;

	return it->second;
}

Document *ComponentNode::get_document()
{
	return document;
}

/////////////////////////////////////////////////////////////////////////////
// ComponentNode operations:

void ComponentNode::set_options(const CL_ComponentOptions &new_options)
{
	options = new_options;
	document->sig_node_options_changed(this);
}

void ComponentNode::set_type(const std::string &new_type)
{
	type = new_type;
	document->sig_node_type_changed(this);
}

ComponentNode *ComponentNode::add_node(const std::string &type)
{
	CL_ComponentOptions options;
	ComponentNode *node = new ComponentNode("", type, options, this);
	nodes.push_back(node);
	document->sig_node_added(node);
	return node;
}

void ComponentNode::remove_node(ComponentNode *node)
{
	nodes.remove(node);
	document->sig_node_removed(node);
	delete node;
}

/////////////////////////////////////////////////////////////////////////////
// ComponentNode implementation:
