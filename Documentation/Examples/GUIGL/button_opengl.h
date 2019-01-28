/*
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_button_style_opengl
#define header_button_style_opengl

#include <ClanLib/GUI/component_style.h>

class CL_Button;
class CL_StyleManager_OpenGL;
class CL_ResourceManager;
class CL_Font;

class CL_Button_OpenGL : public CL_ComponentStyle
{
public:
	CL_Button_OpenGL(
		CL_Button *button,
		CL_StyleManager_OpenGL *style);

	virtual ~CL_Button_OpenGL();

private:
	CL_Slot slot_paint;
	void on_paint();

	CL_Slot slot_mouse_enter;
	void on_mouse_enter();

	CL_Slot slot_mouse_leave;
	void on_mouse_leave();

	CL_Button *button;
	
	bool highlighted;

	CL_StyleManager_OpenGL *style;
	CL_ResourceManager *resources;
	CL_Font *font;
};

#endif
