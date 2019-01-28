#ifndef header_listbox_style_opengl
#define header_listbox_style_opengl

#include <ClanLib/gui.h>

class CL_Font;
class CL_ListBox;
class CL_ComponentOptions;
class CL_StyleManager_OpenGL;
class CL_ResourceManager;

class CL_ListBox_OpenGL : public CL_ComponentStyle
{
public:
	CL_ListBox_OpenGL(
		CL_ListBox *listbox,
		CL_StyleManager_OpenGL *style);
	
	virtual ~CL_ListBox_OpenGL();

private:
	CL_Slot slot_paint_background;
	CL_Slot slot_paint_listbox;
	void on_paint_background();
	void on_paint_listbox();

	CL_ListBox *listbox;

	CL_StyleManager_OpenGL *style;
	CL_ResourceManager *resources;
	CL_Font *font;
};

#endif
