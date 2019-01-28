/*
	$Id: radiogroup_generic.h,v 1.4 2002/03/01 19:36:55 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_radiogroup_generic
#define header_radiogroup_generic

#include "../API/signals.h"

class CL_RadioButton;

class CL_RadioGroup_Generic
{
public:
	CL_RadioGroup_Generic() {};

	void add(CL_RadioButton *button, bool delete_component);
	void remove(CL_RadioButton *button);

	CL_RadioButton *get_toggled() const;
	
	CL_Signal_v1<CL_RadioButton *> sig_selection_changed;

	std::vector<CL_RadioButton *> buttons;

private:
	void on_button_clicked(CL_RadioButton *button);

	CL_SlotContainer slots;
};

#endif
