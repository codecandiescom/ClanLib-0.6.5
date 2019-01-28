/*
	$Id: provider_dynamic.h,v 1.5 2001/09/22 15:52:12 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

*/

//! clanDisplay="Surface Providers"
//! header=display.h

#ifndef header_dynamic_provider
#define header_dynamic_provider

#include "generic_surfaceprovider.h"

//: Dynamic Provider Class
//- Empty space dynamic surface provider header file
class CL_DynamicProvider : public CL_SurfaceProvider_Generic
{
public:

//! Construction:
	//: Create
	static CL_Surface *create(int width, int height, int no_sprs, EPixelFormat type = PAL8, int transcol = -1)
	{
		return CL_Surface::create(new CL_DynamicProvider(width, height, no_sprs, type, transcol), true);
	}

	//: Create
	static CL_Surface *create(int width, int height, int no_sprs, int red_mask, int green_mask, int blue_mask, 
				  int alpha_mask, int transcol = -1)
	{
		return CL_Surface::create(new CL_DynamicProvider(width, height, no_sprs, red_mask, green_mask, 
					  blue_mask, alpha_mask, transcol), true);
	}

	//: Dynamic Provider Constructor
	CL_DynamicProvider(int width, int height, int no_sprs, EPixelFormat type = PAL8, int trans_col = -1)
	{
		this->width = width;
		this->height = height;
		this->no_sprs = no_sprs;
		this->trans_col = trans_col;
		data = NULL;
		pixelformat = type;
		bpp = get_bytes_pr_pixel();
		data = new unsigned char[width * height * no_sprs * bpp]; 
	}  

	//: Dynamic Provider Constructor
	CL_DynamicProvider(int width, int height, int no_sprs, int red_mask, int green_mask, int blue_mask, 
			   int alpha_mask, int transcol = -1)
	{
		// has to be blitted by the dynamic blitter at the moment
		this->width = width;
		this->height = height;
		this->no_sprs = no_sprs;
		this->trans_col = trans_col;
		this->red_mask = red_mask;
		this->green_mask = green_mask;
		this->blue_mask = blue_mask;
		this->alpha_mask = alpha_mask;
		
		data = NULL;
		pixelformat = CUSTOM;
		bpp = get_bytes_pr_pixel();

		data = new unsigned char[width * height * no_sprs * bpp]; 
	}  

	//: <p>Deletes the DynamicProvider and deletes the data
	//: which was allocated in the constructor</p>
	virtual ~CL_DynamicProvider() { delete[] data; }

//! Attributes:
	//: Get width
	virtual int get_width() const { return width; }

	//: Get height
	virtual int get_height() const { return height; }

	//: Get no sprs
	virtual int get_no_sprs() const { return no_sprs; }

	//: Returns the pixelformat of the DynamicProvider as it was set in the constructor
	virtual EPixelFormat get_pixel_format() const { return pixelformat; }

	//: Returns NULL, because PAL8 isn't supported
	virtual CL_Palette *get_palette() const { return NULL; }

	//: Returns the transparent color, which was set in the constructor
	virtual int get_transcol() const { return trans_col; }

	//: <p>Returns the pointer to the surface data, where you
	//: can copy your data to. It should be
	//: only called between lock() and unlock(). </p>
	virtual void *get_data() const { cl_assert(data!=NULL); return data; }

//! Operations:
	//: Perform Lock
	virtual void perform_lock() { }

	//: Perform UnLock
	virtual void perform_unlock() { }

//! Implementation:
private:

	int width, height, no_sprs, trans_col;
	int red_mask, green_mask, blue_mask, alpha_mask;
	EPixelFormat pixelformat;
	unsigned char *data;
	int bpp;
};

#endif
