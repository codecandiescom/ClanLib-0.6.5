/*
	$Id: scrollbar_default.h,v 1.24 2002/02/01 14:44:06 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_scrollbar_default
#define header_scrollbar_default

#include "API/Core/System/timer.h"
#include "API/GUI/scrollbar.h"
#include "API/GUI/stylemanager_default.h"
#include "API/GUI/component_style.h"

class CL_ResourceManager;
class CL_Surface;

class CL_ScrollBar_Default : public CL_ComponentStyle
{
public:
	CL_ScrollBar_Default(
		CL_ScrollBar *scrollbar,
		CL_StyleManager_Default *style);
	
	virtual ~CL_ScrollBar_Default();

private:
	void start_scroll(int delta);
	void stop_scroll();

	void on_paint();
	void on_timer_scroll();
	void on_resize(int old_width, int old_height);

	CL_SlotContainer slots;

	CL_Timer timer_scroll;
	int scroll_delta;

	CL_ScrollBar *scrollbar;
	
	CL_Button *button_increase;
	CL_Button *button_decrease;

	CL_StyleManager_Default *style;
	CL_ResourceManager *resources;
	CL_Surface *sur_increase_normal;
	CL_Surface *sur_increase_toggled;
	CL_Surface *sur_decrease_normal;
	CL_Surface *sur_decrease_toggled;
};

#endif

