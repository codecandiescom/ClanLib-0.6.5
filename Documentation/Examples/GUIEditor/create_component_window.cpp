#include "create_component_window.h"

CreateComponentWindow::CreateComponentWindow(
									std::string type,
									CL_Component *parent, 
									ComponentStore *store,
									CL_StyleManager *style)
:
	CL_Window(
		CL_Rect(180, 100,  600, 300),
		"Create " + type + " component",
		parent,
		style),
	store(store)
{
	this->type=type;

	label_name=new CL_Label(CL_Rect(5,5,80,25),"Name:",get_client_area());
	component_name=new CL_InputBox(CL_Rect(90,5,200,25),get_client_area());

	but_create=new CL_Button(CL_Rect(210,5,300,25),"Create",get_client_area());
	slot_create=but_create->sig_clicked().connect(this, &CreateComponentWindow::on_create);

	but_cancel=new CL_Button(CL_Rect(310,5,400,25),"Cancel",get_client_area());
	slot_cancel=but_cancel->sig_clicked().connect(this, &CreateComponentWindow::on_cancel);

	fill_optionvalues_list();
	set_client_size(420, 50 + option_values.size() * 20);

	create_optionvalue_edit_list();
}

CreateComponentWindow::~CreateComponentWindow()
{

}


void CreateComponentWindow::fill_optionvalues_list()
{
	option_values.clear();

	CL_ComponentManager::component_type_map_t::iterator i;

	i=CL_ComponentManager::component_types.find(type);

	if (i!=CL_ComponentManager::component_types.end())
	{
		CL_ComponentType *comp_type=(*i).second;

		CL_ComponentType::option_map_t::iterator j;
		
		for (j=comp_type->options.begin();
			 j!=comp_type->options.end();
			 j++)
		{
			 option_values[(*j).first].type = (*j).second.type;
		}
	}
	
}

void CreateComponentWindow::create_optionvalue_edit_list()
{
	option_map_t::iterator it(option_values.begin());
	option_map_t::iterator it_end(option_values.end());

	// ok, we need to dynamicly create the list of labels and inputboxes so we can present them later on a on_paint
	for (int pos=0;it!=it_end;++it, ++pos)
	{
		new CL_Label(CL_Rect(5,35+pos*20,80,55+pos*20),(*it).first, get_client_area());
		switch ((*it).second.type)
		{
		case CL_ComponentType::SOptionType::STRING:
			{
				CL_InputBox *input = new CL_InputBox(CL_Rect(85,35+pos*20, 225, 55+pos*20), "", get_client_area());
				(*it).second.component = input;
			}
			break;
		case CL_ComponentType::SOptionType::NUMBER:
			{
				CL_InputBox *input = new CL_InputBox(CL_Rect(85,35+pos*20, 225, 55+pos*20), "0", get_client_area());
				input->set_text("0");
				(*it).second.component = input;
			}
			break;
		case CL_ComponentType::SOptionType::BOOL:
			{
				CL_CheckBox *box = new CL_CheckBox(CL_Point(85,35+pos*20), "", get_client_area());
				box->set_checked(false);
				(*it).second.component = box;
			}
			break;
		}
	} 
}

void CreateComponentWindow::on_create()
{
	ComponentStore::component_data_t c;
	c.name = component_name->get_text();

	// add component options
	CL_ComponentOptions options;
	option_map_t::iterator it(option_values.begin());
	option_map_t::iterator it_end(option_values.end());
	for (;it!=it_end;++it)
	{
		switch ((*it).second.type)
		{
		case CL_ComponentType::SOptionType::STRING:
		case CL_ComponentType::SOptionType::NUMBER:
			options.add_option((*it).first, ((CL_InputBox*) (*it).second.component)->get_text());
			break;
		case CL_ComponentType::SOptionType::BOOL:
			options.add_option((*it).first, ((CL_CheckBox*) (*it).second.component)->is_checked()?"1":"0");
			break;
		}
	}

	c.component = CL_ComponentManager::create_component(type, NULL, get_style_manager());
	c.component->sig_set_options().call(options);

	// add created component to component store
	store->push_back(c);
}

void CreateComponentWindow::on_cancel()
{
	quit();
}