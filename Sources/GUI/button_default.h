/*
	$Id: button_default.h,v 1.29 2002/01/16 18:47:50 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_button_default
#define header_button_default

#include "API/GUI/button.h"
#include "API/GUI/component_style.h"

class CL_Font;
class CL_Surface;
class CL_StyleManager_Default;

class CL_Button_Default : public CL_ComponentStyle
{
public:
	CL_Button_Default(
		CL_Button *button,
		CL_StyleManager_Default *style);

	virtual ~CL_Button_Default();

private:
	void on_set_options(const CL_ComponentOptions &options);
	CL_Slot slot_set_options;

	void on_paint();
	CL_Slot slot_paint;

	void on_get_preferred_size(CL_Point &size);
	CL_Slot slot_get_preferred_size;

	CL_Button *button;
	
	CL_StyleManager_Default *style;
	CL_ResourceManager *resources;
	CL_Font *font;
	CL_Font *font_disabled;
};

#endif
