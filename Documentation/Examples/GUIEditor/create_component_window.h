#ifndef CREATE_COMPONENT_WINDOW_INCLUDED
#define CREATE_COMPONENT_WINDOW_INCLUDED

#include <ClanLib/gui.h>
#include <string>
#include <vector>
#include "component_store.h"

class CreateComponentWindow: public CL_Window
{
// Construction
public:
	CreateComponentWindow(std::string type,CL_Component *parent, ComponentStore *store, CL_StyleManager *style = NULL);
	~CreateComponentWindow();

	CL_Slot slot_create, slot_cancel;

// Implementation
private:
	void fill_optionvalues_list();
	void create_optionvalue_edit_list();
	
	void on_create();
	void on_cancel();

	CL_Component	*list;		// this is our custom listbox

	CL_ScrollBar	*scrollbar;	// list-scrollbar

	struct SOption
	{
		CL_ComponentType::SOptionType::EDatatype type;
		CL_Component *component;
	};
	typedef	std::map<std::string, SOption> option_map_t;
	option_map_t option_values;

	std::vector<CL_Label *>label_options;
	std::vector<CL_InputBox *>input_options;

	std::string type;			// component type name

	ComponentStore *store;
	CL_Button	*but_create;
	CL_Button	*but_cancel;
	CL_Label	*label_name;
	CL_InputBox *component_name;
};

#endif

