#include "component_type_window.h"
#include "create_component_window.h"

ComponentTypeWindow::ComponentTypeWindow(CL_Component *parent, CL_StyleManager *style)
:
	CL_Window(
		CL_Rect(100, 20,  420, 200),
		"Component Types",
		parent,
		style)
{
		type_list=new CL_ListBox(CL_Rect(5,35,160,150),get_client_area());
		options_list=new CL_ListBox(CL_Rect(165,35,300,150),get_client_area());

		add_component=new CL_Button(CL_Rect(5,5,160,30),"Create Component with type:",get_client_area());
		options_label=new CL_Label(CL_Rect(165,10,300,30),"Component Options:",get_client_area());

		slot_add=add_component->sig_clicked().connect(this, &ComponentTypeWindow::on_add);
		slot_select=type_list->sig_activated().connect(this,&ComponentTypeWindow::on_select);

		fill_type_list();
		type_list->set_current_item(0);
}


ComponentTypeWindow::~ComponentTypeWindow()
{
}

void ComponentTypeWindow::fill_type_list()
{
	// we clear the current type list, and fill it with all the information
	// from the CL_ComponentManager (it keeps a nice list with all the component types)
	type_list->clear();

	std::map<std::string,CL_ComponentType *>::iterator i;

	for (i=CL_ComponentManager::component_types.begin();
		 i!=CL_ComponentManager::component_types.end();
		 i++)
	{
		type_list->insert_item((*i).first);
	}
	
}

void ComponentTypeWindow::on_add()
{
	// whoooo... the user  is trying to add a component of a certain type,
	// next step is to show him a dialog with the available component options he can set

	std::string type = type_list->get_current_text();

	if (type=="") 
		// you were trying to add something but you didn't tell me what
		return;

	CreateComponentWindow createWindow(type, NULL, &m_store, get_style_manager());
	createWindow.run(get_gui_manager());
}

void ComponentTypeWindow::fill_options_list(std::string &type)
{
	options_list->clear();

	std::map<std::string,CL_ComponentType *>::iterator i;

	i=CL_ComponentManager::component_types.find(type);

	if (i!=CL_ComponentManager::component_types.end())
	{
		CL_ComponentType *comp_type=(*i).second;

		CL_ComponentType::option_map_t::iterator j;
		
		for (j=comp_type->options.begin();
			 j!=comp_type->options.end();
			 j++)
		{
			options_list->insert_item((*j).first);
		}
	}
}


void ComponentTypeWindow::on_select(int i)
{
	std::string type=type_list->get_text(i);

	fill_options_list(type);
}
