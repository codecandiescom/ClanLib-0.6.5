
#ifdef WIN32
#pragma warning(disable : 4355)
#pragma warning(disable : 4786)
#endif

#include "propertybar.h"
#include "mainframe.h"

/////////////////////////////////////////////////////////////////////////////
// PropertyBar construction:

PropertyBar::PropertyBar(MainFrame *mainframe)
: CL_Component(mainframe), mainframe(mainframe), listbox(this), name_label(this),
	value_inputbox(this) //, value_combobox(this)
{
	// Only show either inputbox or combobox, but never both at the same time.
//	value_combobox.show(false);

	Document &doc = get_document();
	slots.connect(doc.sig_node_options_changed, this, &PropertyBar::on_node_options_changed);
	slots.connect(sig_resize(), this, &PropertyBar::on_resize);
	slots.connect(listbox.sig_highlighted(), this, &PropertyBar::on_highlighted);
	slots.connect(mainframe->sig_selection_changed, this, &PropertyBar::on_selection_changed);

	on_resize(get_width(), get_height());
}

PropertyBar::~PropertyBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// PropertyBar attributes:

Document &PropertyBar::get_document()
{
	return mainframe->document;
}

/////////////////////////////////////////////////////////////////////////////
// PropertyBar operations:


/////////////////////////////////////////////////////////////////////////////
// PropertyBar implementation:

void PropertyBar::on_resize(int old_width, int old_height)
{
	int width = get_width();
	int height = get_height();

	listbox.set_position(CL_Rect(4, 4, width-4, height-40));
	name_label.set_position(CL_Rect(4, height-30, 90, height-4));
	value_inputbox.set_position(CL_Rect(100, height-30, width-4, height-4));
//	value_combobox.set_position(CL_Rect(100, height-30, width-4, height-4));
}

void PropertyBar::on_highlighted(int item)
{
}

void PropertyBar::on_node_options_changed(ComponentNode *node)
{
}

void PropertyBar::on_selection_changed(std::list<ComponentNode *> &selection)
{
}
