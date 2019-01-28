/*
	$Id: surfaceprovider.h,v 1.9 2002/06/27 20:30:43 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanDisplay="Display 2D"
//! header=display.h

#ifndef header_surfaceprovider
#define header_surfaceprovider

#include <string>
#include "target.h"
class CL_ResourceManager;

//: The image source interface.
//- <p>This is the interface used by a CL_Surface surface to
//- load an image. For more information about the relationship between a
//- surface and its provider, please the CL_Surface class description.</p>
//-
//- <p>The surface provider interface extends the 
//- CL_Target renderer target interface with some blitting
//- related functions. Currently, this is translation of the image and
//- colorkey transparency.</p>
//-
//- <p>ClanLib includes a large range of surface providers that can load an
//- image from various sources: PCX, TGA, PNG and so on.</p>
//-
//- <p>If you want to be able to load your (newly created) surface provider
//- as a resource, you have to implement the CL_ResourceSource_Surface as
//- well.</p>
class CL_SurfaceProvider : public CL_Target
{
public:
//! Construction:
	//: This function loads a surface provider from a resource file (usually a datafile).
	static CL_SurfaceProvider *load(
		const std::string &resource_id,
		CL_ResourceManager *manager);

	//: Surface Provider Destructor
	virtual ~CL_SurfaceProvider() {;}

//! Attributes:
	//: <p>Returns the offset of the surface_data returned 
	//: by get_data(). This translation will 
	//: be applied to every putscreen using a given 
	//: surfaceproviders data as input.</p>
	//- Returns - The x-offset of the surface data.
	virtual int get_translate_x() const { return 0; }

	//: <p>Returns the offset of the surface_data returned 
	//: by get_data(). This translation will 
	//: be applied to every putscreen using a given 
	//: surfaceproviders data as input.</p>
	//- Returns - The y-offset of the surface data.
	virtual int get_translate_y() const { return 0; }

	//: Returns true if the surface provider uses a source color key.
	virtual bool uses_src_colorkey() const =0;

	//: Returns the source color key.
	virtual unsigned int get_src_colorkey() const =0;

	//: Get Pixel
	virtual void get_pixel(int x, int y, float *r, float *g, float *b, float *a);

	//: Get Pixel
	virtual int get_pixel(int x, int y) {
		return CL_Target::get_pixel (x, y);
	}

};

#endif
