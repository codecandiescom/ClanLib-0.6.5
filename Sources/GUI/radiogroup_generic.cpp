/*
	$Id: radiogroup_generic.cpp,v 1.6 2002/03/01 19:36:55 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "../API/GUI/radiobutton.h"
#include "radiogroup_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Attributes:

CL_RadioButton *CL_RadioGroup_Generic::get_toggled() const
{
	std::vector<CL_RadioButton *>::const_iterator it;
	for(it = buttons.begin(); it != buttons.end(); ++it)
		if((*it)->is_checked())
			return (*it);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_RadioGroup_Generic::add(CL_RadioButton *button, bool delete_component)
{
	// TODO: Use delete_component
	
	buttons.push_back(button);

	slots.connect(button->sig_clicked(), this, &CL_RadioGroup_Generic::on_button_clicked, button);
}

void CL_RadioGroup_Generic::remove(CL_RadioButton *button)
{
	// TODO: Remove clicked-slot from slots as well
	
	std::vector<CL_RadioButton *>::iterator it;
	for(it = buttons.begin(); it != buttons.end(); ++it)
	{
		if((*it) == button)
		{
			buttons.erase(it);
			return;
		}	
	}
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_RadioGroup_Generic::on_button_clicked(CL_RadioButton *button)
{
	std::vector<CL_RadioButton *>::iterator it;
	for(it = buttons.begin(); it != buttons.end(); ++it)
	{
		if((*it) != button)
			(*it)->set_checked(false);
		else
		{
			if((*it)->is_checked())
				sig_selection_changed(*it);
			else
				(*it)->set_checked(true);
		}
	}
}
