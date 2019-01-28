/*
	$Id: treeitem_default.h,v 1.1 2002/03/01 19:36:55 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_treeitem_default
#define header_treeitem_default

#include "API/GUI/treeitem.h"
#include "API/GUI/component_style.h"

class CL_Font;
class CL_StyleManager_Default;

class CL_TreeItem_Default : public CL_ComponentStyle
{
public:
	CL_TreeItem_Default(
		CL_TreeItem *item,
		CL_StyleManager_Default *style);

	virtual ~CL_TreeItem_Default();

	void on_paint();
	void on_get_preferred_size(CL_Point &size);

private:
	CL_Slot slot_paint;
	CL_Slot slot_get_preferred_size;

	CL_TreeItem *item;

	CL_Font *font;

	CL_StyleManager_Default *style;
	CL_ResourceManager *resources;
};

#endif
