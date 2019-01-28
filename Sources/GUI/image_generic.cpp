/*
	$Id: image_generic.cpp,v 1.4 2001/12/27 23:30:08 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "image_generic.h"

CL_Image_Generic::CL_Image_Generic(CL_Image *self)
: surface(NULL), mode(CL_Image::center), image(self)
{
}
