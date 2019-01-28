
#ifdef WIN32
#pragma warning(disable : 4355)
#pragma warning(disable : 4786)
#endif

#include "toolbar.h"
#include "mainframe.h"

/////////////////////////////////////////////////////////////////////////////
// ToolBar construction:

ToolBar::ToolBar(MainFrame *mainframe)
: CL_Component(mainframe), mainframe(mainframe), new_document(this),
	load_document(this), save_document(this), new_component_mode(this),
	select_mode(this)
{
	new_document.set_position(CL_Rect(0, 0, 36, 36));
	load_document.set_position(CL_Rect(40, 0, 40+36, 36));
	save_document.set_position(CL_Rect(80, 0, 80+36, 36));
	new_component_mode.set_position(CL_Rect(120, 0, 120+36, 36));
	select_mode.set_position(CL_Rect(160, 0, 160+36, 36));

	// todo: draw some icons or something. Setting text strings instead:
	new_document.set_text("New");
	load_document.set_text("Load");
	save_document.set_text("Save");
	new_component_mode.set_text("+");
	select_mode.set_text("*");

	Document &doc = get_document();
	slots.connect(new_document.sig_clicked(), this, &ToolBar::on_new_document);
	slots.connect(load_document.sig_clicked(), this, &ToolBar::on_load_document);
	slots.connect(save_document.sig_clicked(), this, &ToolBar::on_save_document);
	slots.connect(new_component_mode.sig_clicked(), this, &ToolBar::on_new_component_mode);
	slots.connect(select_mode.sig_clicked(), this, &ToolBar::on_select_mode);
}

ToolBar::~ToolBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// ToolBar attributes:

Document &ToolBar::get_document()
{
	return mainframe->document;
}

/////////////////////////////////////////////////////////////////////////////
// ToolBar operations:


/////////////////////////////////////////////////////////////////////////////
// ToolBar implementation:

void ToolBar::on_new_document()
{
	get_document().clear();
}

void ToolBar::on_load_document()
{
	get_document().load("test.gui");
}

void ToolBar::on_save_document()
{
	get_document().save("test.gui");
}

void ToolBar::on_new_component_mode()
{
	mainframe->editview->set_mode(EditView::mode_new);
}

void ToolBar::on_select_mode()
{
	mainframe->editview->set_mode(EditView::mode_select);
}
