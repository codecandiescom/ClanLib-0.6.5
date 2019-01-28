
#ifndef header_document
#define header_document

#ifdef WIN32
#pragma warning(disable : 4786)
#endif

#include <list>
#include <string>
#include <ClanLib/signals.h>
#include <ClanLib/GUI/gui_file_parser.h>
class ComponentNode;

class Document
{
//!Construction:
public:
	//: Construct empty document.
	Document();

	//: Destructor.
	~Document();

//!Attributes:
public:
	// List of component nodes in document.
	std::list<ComponentNode *> nodes;

//!Operations:
public:
	//: New document. Clears the entire document for elements.
	void clear();

	//: Load document tree from a .gui definition file.
	void load(const std::string &gui_file);

	//: Save document tree to a .gui definition file.
	void save(const std::string &gui_file);

	//: Add component to document.
	ComponentNode *add_node(const std::string &type);

	//: Remove component from document.
	void remove_node(ComponentNode *node);

//!Signals:
public:
	//: Signal emitted when a node was added to the document.
	CL_Signal_v1<ComponentNode *> sig_node_added;

	//: Signal emitted when a node was removed from the document.
	CL_Signal_v1<ComponentNode *> sig_node_removed;

	//: Signal emitted when a node's component options was changed.
	CL_Signal_v1<ComponentNode *> sig_node_options_changed;

	//: Signal emitted when a node's type was changed.
	CL_Signal_v1<ComponentNode *> sig_node_type_changed;

//!Implementation:
private:
	void add_node_child(ComponentNode *parent, CL_GUIFileParser::ComponentInfo *info);
};

#endif
