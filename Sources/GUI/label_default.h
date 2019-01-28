/*
	$Id: label_default.h,v 1.19 2002/01/04 15:20:43 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_label_default
#define header_label_default

#include "API/GUI/label.h"
#include "API/GUI/component_style.h"
#include "API/GUI/stylemanager_default.h"

class CL_Label_Default : public CL_ComponentStyle
{
public:
	CL_Label_Default(
		CL_Label *label,
		CL_StyleManager_Default *style);

	virtual ~CL_Label_Default();

private:
	CL_Slot slot_paint;
	CL_Slot slot_get_preferred_size;

	void on_paint();
	void on_get_preferred_size(CL_Point &size);

	CL_Label *label;
	
	CL_ResourceManager *resources;
	class CL_Font *font;
};

#endif
