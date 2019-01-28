
#ifndef header_propertybar
#define header_propertybar

#include <ClanLib/GUI/component.h>
#include <ClanLib/GUI/listbox.h>
#include <ClanLib/GUI/label.h>
#include <ClanLib/GUI/inputbox.h>
//#include <ClanLib/GUI/combobox.h>

class MainFrame;
class Document;
class ComponentNode;

class PropertyBar : public CL_Component
{
//!Construction:
public:
	PropertyBar(MainFrame *mainframe);

	~PropertyBar();

//!Attributes:
public:
	Document &get_document();

	CL_ListBox listbox;

	CL_Label name_label;

	CL_InputBox value_inputbox;

//	CL_ComboBox value_combobox;

//!Operations:
public:

//!Implementation:
private:
	void on_resize(int old_width, int old_height);

	void on_highlighted(int item);

	void on_node_options_changed(ComponentNode *node);

	void on_selection_changed(std::list<ComponentNode *> &selection); 

	CL_SlotContainer slots;

	MainFrame *mainframe;
};

#endif
