/*
	$Id: frame_default.h,v 1.18 2002/01/16 18:58:13 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_frame_default
#define header_frame_default

#include "API/GUI/frame.h"
#include "API/GUI/stylemanager_default.h"
#include "API/GUI/component_style.h"

class CL_Surface;

class CL_Frame_Default : public CL_ComponentStyle
{
public:
	CL_Frame_Default(
		CL_Frame *frame, 
		CL_StyleManager_Default *style);

	virtual ~CL_Frame_Default();

private:
	CL_Slot slot_set_options;
	CL_Slot slot_paint;

	void on_set_options(const CL_ComponentOptions &options);
	void on_paint();

	CL_Frame *frame;

	enum Mode
	{
		center,
		stretch,
		tile
	} mode;
	
	CL_StyleManager_Default *style;
	CL_ResourceManager *resources;
	CL_Surface *sur_surface;
};

#endif
