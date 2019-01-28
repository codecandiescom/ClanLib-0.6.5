/*
	$Id: image_generic.h,v 1.3 2001/12/27 23:30:08 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_image_generic
#define header_image_generic

#include "API/GUI/image.h"

class CL_Image_Generic
{
// Construction:
public:
	CL_Image_Generic(CL_Image *self);
	~CL_Image_Generic() {};

// Attributes:
public:
	CL_Surface *surface;
	CL_Image::Mode mode;

// Implementation:
private:
	CL_Image *image;
};  

#endif
