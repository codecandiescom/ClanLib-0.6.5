/*
	$Id: label_generic.h,v 1.8 2001/12/27 23:43:17 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_label_generic
#define header_label_generic

#include "API/GUI/label.h"

class CL_Label_Generic
{
// Construction:
public:
	CL_Label_Generic(CL_Label *self, const std::string &text);
	~CL_Label_Generic() {};

// Attributes:
public:
	std::string text;
	int alignment;

// Implementation:
private:
	CL_Label *label;

	void on_set_options(const CL_ComponentOptions &options);
	CL_Slot slot_set_options;
};  

#endif
