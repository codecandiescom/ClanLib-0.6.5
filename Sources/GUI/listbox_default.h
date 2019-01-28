/*
	$Id: listbox_default.h,v 1.22 2001/12/27 23:54:01 mbn Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_listbox_default
#define header_listbox_default

#include "API/GUI/listbox.h"
#include "API/GUI/stylemanager_default.h"
#include "API/GUI/component_style.h"

class CL_Font;

class CL_ListBox_Default : public CL_ComponentStyle
{
public:
	CL_ListBox_Default(
		CL_ListBox *listbox,
		CL_StyleManager_Default *style);
	
	virtual ~CL_ListBox_Default();

private:
	CL_Slot slot_paint_background;
	CL_Slot slot_paint_listbox;
	void on_paint_background();
	void on_paint_listbox();

	CL_ListBox *listbox;

	CL_StyleManager_Default *style;
	CL_ResourceManager *resources;
	CL_Font *font;
};

#endif
