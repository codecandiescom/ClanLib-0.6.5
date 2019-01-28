
#ifndef header_toolbar
#define header_toolbar

#include <ClanLib/GUI/component.h>
#include <ClanLib/GUI/button.h>

class MainFrame;
class Document;
class ComponentNode;

// God damn. Someone please make a toolbar component in clanlib!
class ToolBar : public CL_Component
{
//!Construction:
public:
	ToolBar(MainFrame *mainframe);

	~ToolBar();

//!Attributes:
public:
	Document &get_document();

	CL_Button new_document;

	CL_Button load_document;

	CL_Button save_document;

	CL_Button new_component_mode;

	CL_Button select_mode;

//!Operations:
public:

//!Implementation:
private:
	void on_new_document();

	void on_load_document();

	void on_save_document();

	void on_new_component_mode();

	void on_select_mode();

	void on_node_options_changed(ComponentNode *node);

	CL_SlotContainer slots;

	MainFrame *mainframe;
};

#endif
