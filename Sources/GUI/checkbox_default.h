/*
	$Id: checkbox_default.h,v 1.18 2002/03/18 13:12:25 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_checkbox_default
#define header_checkbox_default

#include "API/GUI/checkbox.h"
#include "API/GUI/stylemanager_default.h"
#include "API/GUI/component_style.h"
#include "checkbox_default.h"

class CL_Font;
class CL_Surface;

class CL_CheckBox_Default : public CL_ComponentStyle
{
public:
	CL_CheckBox_Default(
		CL_CheckBox *checkbox,
		CL_StyleManager_Default *style);
		
	virtual ~CL_CheckBox_Default();

protected:
	void on_get_preferred_size(CL_Point &size);
	CL_Slot slot_get_preferred_size;

	void on_paint(CL_SlotParent_v0 &super);
	CL_Slot slot_paint;

	CL_CheckBox *checkbox;

	CL_StyleManager_Default *style;
	CL_ResourceManager *resources;
	
	CL_Font *font;
	CL_Surface *sur_checked;
	CL_Surface *sur_unchecked;
	CL_Surface *sur_checked_disabled;
	CL_Surface *sur_unchecked_disabled;
};

#endif

