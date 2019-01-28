
#ifndef header_mainframe
#define header_mainframe

#include <ClanLib/GUI/component.h>
#include "toolbar.h"
#include "treebar.h"
#include "propertybar.h"
#include "editview.h"
#include "document.h"

class MainFrame : public CL_Component
{
//!Construction:
public:
	MainFrame(CL_Component *parent);

	~MainFrame();

//!Attributes:
public:
	Document document;

	ToolBar *toolbar;

	TreeBar *treebar;

	PropertyBar *propertybar;

	EditView *editview;

//!Operations:
public:

//!Signals:
public:
	CL_Signal_v1< std::list<ComponentNode *> & > sig_selection_changed;

//!Implementation:
private:
	void on_resize(int old_width, int old_height);

	CL_SlotContainer slots;
};

#endif
