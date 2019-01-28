#ifndef header_checkbox_style_opengl
#define header_checkbox_style_opengl

#include <ClanLib/gui.h>
#include <ClanLib/core.h>

class CL_CheckBox;
class CL_StyleManager_OpenGL;
class CL_ResourceManager;

class CL_CheckBox_OpenGL : public CL_ComponentStyle
{
public:
	CL_CheckBox_OpenGL(
		CL_CheckBox *button,
		CL_StyleManager_OpenGL *style);

	virtual ~CL_CheckBox_OpenGL();

private:
	CL_Slot slot_paint;
	void on_paint(CL_SlotParent_v0 &super);

	CL_Slot slot_get_preferred_size;
	void on_get_preferred_size(CL_Point &size);

	CL_CheckBox *checkbox;
	
	CL_StyleManager_OpenGL *style;
	CL_ResourceManager *resources;

	CL_Font *font;

	CL_Surface *surface_left;
	CL_Surface *surface_right;
	CL_Surface *surface_ticker;
};

#endif
