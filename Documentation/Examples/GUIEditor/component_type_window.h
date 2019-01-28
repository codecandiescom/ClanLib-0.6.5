#ifndef COMPONENT_TYPE_WINDOW_INCLUDED
#define COMPONENT_TYPE_WINDOW_INCLUDED

#include <ClanLib/gui.h>
#include <string>
#include "component_store.h"

class ComponentTypeWindow: CL_Window
{
//Creation
public:
	ComponentTypeWindow(CL_Component *parent, CL_StyleManager *style = NULL);
	~ComponentTypeWindow();

// Signals
	CL_Slot	slot_add;
	CL_Slot slot_select;

	void on_add();

// Implementation
private:
	void fill_type_list();
	void fill_options_list(std::string &type);
	void on_select(int i);

	ComponentStore m_store;
	CL_ListBox	*options_list;	// component options' options
	CL_Button  *add_component;
	CL_ListBox *type_list;
	CL_Label	*options_label;
};

#endif
