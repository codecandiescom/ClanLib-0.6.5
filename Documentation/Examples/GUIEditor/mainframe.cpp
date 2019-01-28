
#ifdef WIN32
#pragma warning(disable : 4355)
#pragma warning(disable : 4786)
#endif

#include "mainframe.h"

/////////////////////////////////////////////////////////////////////////////
// MainFrame construction:

MainFrame::MainFrame(CL_Component *parent)
: CL_Component(parent)
{
	toolbar = new ToolBar(this);
	treebar = new TreeBar(this);
	propertybar = new PropertyBar(this);
	editview = new EditView(this);

	slots.connect(sig_resize(), this, &MainFrame::on_resize);
	on_resize(get_width(), get_height());
}

MainFrame::~MainFrame()
{
}

/////////////////////////////////////////////////////////////////////////////
// MainFrame attributes:


/////////////////////////////////////////////////////////////////////////////
// MainFrame operations:


/////////////////////////////////////////////////////////////////////////////
// MainFrame implementation:

void MainFrame::on_resize(int old_width, int old_height)
{
	int width = get_width();
	int height = get_height();

	toolbar->set_position(CL_Rect(0, 0, width, 40));
	treebar->set_position(CL_Rect(0, 40, 250, height/2));
	propertybar->set_position(CL_Rect(0, height/2, 250, height));
	editview->set_position(CL_Rect(250, 40, width, height));
}
