
#ifndef header_treebar
#define header_treebar

#include <map>
#include <ClanLib/GUI/component.h>
#include <ClanLib/GUI/treeview.h>

class MainFrame;
class Document;
class ComponentNode;
class CL_TreeView_Node;

class TreeBar : public CL_Component
{
//!Construction:
public:
	TreeBar(MainFrame *mainframe);

	~TreeBar();

//!Attributes:
public:
	Document &get_document();

	CL_TreeView tree;

//!Operations:
public:

//!Implementation:
private:
	std::map<ComponentNode *, CL_TreeNode *> tree_nodes;

	void on_resize(int old_width, int old_height);

	void on_tree_selection_changed(const CL_TreeNode &node);

	void on_node_added(ComponentNode *node);

	void on_node_removed(ComponentNode *node);

	void on_node_options_changed(ComponentNode *node);

	void on_node_type_changed(ComponentNode *node);

	void on_selection_changed(std::list<ComponentNode *> &selection);

	CL_SlotContainer slots;

	MainFrame *mainframe;
};

#endif
