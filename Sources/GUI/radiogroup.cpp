/*
	$Id: radiogroup.cpp,v 1.3 2002/03/01 19:36:55 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "../API/GUI/radiogroup.h"
#include "radiogroup_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_RadioGroup::CL_RadioGroup()
{
	impl = new CL_RadioGroup_Generic();
}

CL_RadioGroup::~CL_RadioGroup()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CL_RadioGroup::get_count() const
{
	return impl->buttons.size();
}
	
CL_RadioButton *CL_RadioGroup::get_toggled() const
{
	return impl->get_toggled();
}

const std::vector<CL_RadioButton *> &CL_RadioGroup::get_buttons() const
{
	return impl->buttons;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_RadioGroup::add(CL_RadioButton *button, bool delete_component)
{
	impl->add(button, delete_component);
}
	
void CL_RadioGroup::remove(CL_RadioButton *button)
{
	impl->remove(button);
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v1<CL_RadioButton *> &CL_RadioGroup::sig_selection_changed()
{
	return impl->sig_selection_changed;
}
