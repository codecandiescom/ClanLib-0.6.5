
#ifndef header_component_node
#define header_component_node

#ifdef WIN32
#pragma warning(disable : 4786)
#endif

#include <list>
#include <string>
#include <ClanLib/GUI/component_options.h>
class Document;
class CL_ComponentType;

class ComponentNode
{
//!Construction:
private:
	//: Construct node with other node as parent.
	ComponentNode(const std::string &name, const std::string &type, const CL_ComponentOptions &options, ComponentNode *parent);

	//: Construct node with document as parent.
	ComponentNode(const std::string &name, const std::string &type, const CL_ComponentOptions &options, Document *parent);

	//: Destructor.
	~ComponentNode();

//!Attributes:
public:
	//: Options attached to this component.
	const CL_ComponentOptions &get_options() const;

	//: Name of the component, if any.
	const std::string &get_name() const { return name; }

	//: Component type. Eg. "button" or "window".
	const std::string &get_type() const;

	//: List of child components of this component.
	std::list<ComponentNode *> nodes;

	//: Finds and returns the component type object for this
	//: component. If not found, null is returned.
	CL_ComponentType *find_component_type();

	//: Get document the node belongs to.
	Document *get_document();

	ComponentNode *get_parent() { return parent; }

//!Operations:
public:
	//: Change the options for this component.
	void set_options(const CL_ComponentOptions &options);

	//: Change component type.
	void set_type(const std::string &type);

	//: Add child component to node.
	ComponentNode *add_node(const std::string &type);

	//: Remove child component from node.
	void remove_node(ComponentNode *node);

//!Implementation:
private:
	//: Allow construction from document.
	friend Document;

	//: Parent node, if any.
	ComponentNode *parent;

	//: Document owner.
	Document *document;

	//: Component name, if any.
	std::string name;

	//: Component type.
	std::string type;

	//: Component options.
	CL_ComponentOptions options;
};

#endif
