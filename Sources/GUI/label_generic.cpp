/*
	$Id: label_generic.cpp,v 1.15 2001/12/27 23:43:17 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "label_generic.h"
#include "API/GUI/component_options.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Label_Generic::CL_Label_Generic(CL_Label *self, const std::string &_text)
: alignment(0), label(self)
{
	text = _text;

	slot_set_options = label->sig_set_options().connect(
		this, &CL_Label_Generic::on_set_options);
}

void CL_Label_Generic::on_set_options(const CL_ComponentOptions &options)
{
	if(options.exists("text"))
		text = options.get_value("text");
}
