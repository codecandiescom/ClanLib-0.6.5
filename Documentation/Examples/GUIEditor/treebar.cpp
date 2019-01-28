
#ifdef WIN32
#pragma warning(disable : 4355)
#pragma warning(disable : 4786)
#endif

#include "treebar.h"
#include "mainframe.h"
#include "component_node.h"

/////////////////////////////////////////////////////////////////////////////
// TreeBar construction:

TreeBar::TreeBar(MainFrame *mainframe)
: CL_Component(mainframe), mainframe(mainframe), tree(this)
{
	Document &doc = get_document();

	std::list<ComponentNode *>::iterator it;
	for (it = doc.nodes.begin(); it != doc.nodes.end(); it++)
	{
		on_node_added(*it);
	}

	slots.connect(sig_resize(), this, &TreeBar::on_resize);
	slots.connect(tree.sig_selection_changed(), this, &TreeBar::on_tree_selection_changed);
	slots.connect(doc.sig_node_added, this, &TreeBar::on_node_added);
	slots.connect(doc.sig_node_removed, this, &TreeBar::on_node_removed);
	slots.connect(doc.sig_node_options_changed, this, &TreeBar::on_node_options_changed);
	slots.connect(doc.sig_node_type_changed, this, &TreeBar::on_node_type_changed);
	slots.connect(mainframe->sig_selection_changed, this, &TreeBar::on_selection_changed);

	on_resize(get_width(), get_height());
}

TreeBar::~TreeBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// TreeBar attributes:

Document &TreeBar::get_document()
{
	return mainframe->document;
}

/////////////////////////////////////////////////////////////////////////////
// TreeBar operations:


/////////////////////////////////////////////////////////////////////////////
// TreeBar implementation:

void TreeBar::on_tree_selection_changed(const CL_TreeNode &node)
{
}

void TreeBar::on_resize(int old_width, int old_height)
{
	int width = get_width();
	int height = get_height();

	tree.set_position(CL_Rect(4, 4, width-4, height-4));
}

void TreeBar::on_node_added(ComponentNode *node)
{
	CL_TreeNode *parent = &tree;
	if (node->get_parent() != 0)
	{
		std::map<ComponentNode *, CL_TreeNode *>::iterator it_comp;
		it_comp = tree_nodes.find(node->get_parent());
		if (it_comp != tree_nodes.end()) parent = it_comp->second;
	}

	std::string name = node->get_type();
	if (!node->get_name().empty())
		name = name + " [" + node->get_name() + "]";

	CL_TreeNode *tree_node = parent->insert_item(name);
	tree_nodes[node] = tree_node;

	std::list<ComponentNode *>::iterator it;
	for (it = node->nodes.begin(); it != node->nodes.end(); it++)
	{
		on_node_added(*it);
	}
}

void TreeBar::on_node_removed(ComponentNode *node)
{
}

void TreeBar::on_node_options_changed(ComponentNode *node)
{
}

void TreeBar::on_node_type_changed(ComponentNode *node)
{
}

void TreeBar::on_selection_changed(std::list<ComponentNode *> &selection)
{
	tree.clear_selection();

	std::list<ComponentNode *>::iterator it;
	for (it = selection.begin(); it != selection.end(); it++)
	{
		ComponentNode *node = *it;
		tree_nodes[node]->set_selected(true);
	}
}
