/*
	$Id: image_default.h,v 1.3 2001/12/27 23:30:08 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_image_default
#define header_image_default

#include "API/GUI/image.h"
#include "API/GUI/stylemanager_default.h"
#include "API/GUI/component_style.h"

class CL_Surface;

class CL_Image_Default : public CL_ComponentStyle
{
public:
	CL_Image_Default(
		CL_Image *image, 
		CL_StyleManager_Default *style);

	virtual ~CL_Image_Default() {}

private:
	CL_Slot slot_paint;
	void on_paint();

	CL_Image *image;

	CL_ResourceManager *resources;
};

#endif
