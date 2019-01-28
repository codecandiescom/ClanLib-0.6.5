/*
	$Id: provider_pcx.h,v 1.7 2001/10/16 11:17:17 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

	File purpose:
		PCX surface provider.
*/

//! clanDisplay="Surface Providers"
//! header=display.h

#ifndef header_pcxprovider
#define header_pcxprovider

#include "generic_surfaceprovider.h"
#include "../Display/pixelformat.h"

class CL_InputSourceProvider;

//: Surface provider that can load PCX files.
//- <p>The PCX decoder has been modified to load most PCX files
//- (instead of just the ones created by Deluxe Paint IIe).
//- It now support 1,2,4,8, or 24 bit files... and has been tested
//- in all but a few situations.</p>
//-
//- <p>Technically, the implementation cheats in that it doesnt
//- store images with less than 8 bits per pixel in their 
//- native format.  It simply converts them to PAL8
//- pixelformat instead of creating other pixelformats.</p>
//-
//- <p>Also note that the PCX decoder creates more bytes per 
//- line than the image is supposed to contain.  We rely on the clipping
//- capabilities of ClanLib to take care of that.  Also note that some
//- default palettes were defined in the read_header() method.  These 
//- definitions might be better placed in palette.h.  Since the scope of 
//- the PCX_Provider rewrite was limited to the provider itself, no files
//- outside of pcx_provider.h and pcx_provider.cpp were modified.</p>
//-
//- <p>The PCX decoding code was moved to read_data() and read_header()
//- to make it easier to follow the code (called from perform_unlock()).</p>
class CL_PCXProvider : public CL_SurfaceProvider_Generic
{
public:
//! Construction:
	//: Loads the pcx file 'handle' from the inputsource provider 'provider. 
	//: Creates a CL_Surface using the pcx image and returns it.
	//: If you just want to load a file, use 'handle' for filename, and set provider to NULL
	static CL_Surface *create(
		std::string handle,
		CL_InputSourceProvider *provider,
		bool transparent=false,
		unsigned char trans_col=0);
	
	//: Constructs a surface provider that can read pcx files.
	//- name - Name of the pcx file to load.
	//- provider - Input source provider that delivers the pcx file.
	//- transparent - True if a color in image should be transparent.
	//- trans_col - Transparency color used if 'transparent' is true. Defaults to color 0.
	CL_PCXProvider(
		std::string name,
		CL_InputSourceProvider *provider,
		bool transparent=false,
		unsigned char trans_col=0);

	//: PCX Provider destructor
	virtual ~CL_PCXProvider();

//! Attributes:
	//: Returns the pitch of the image (bytes per line).
	virtual unsigned int get_pitch() const { return pitch; }

	//: Returns how many pixels to translate the image (X axis).
	virtual int get_translate_x() const { return bounding_left; }

	//: Returns how many pixels to translate the image (Y axis).
	virtual int get_translate_y() const { return bounding_top; }

	//: Returns the width of the image.
	virtual unsigned int get_width() const { return bounding_right-bounding_left; }

	//: Returns the height of the image.
	virtual unsigned int get_height() const { return bounding_bottom-bounding_top; }

	//: Returns the number of subsprites in the image.
	virtual unsigned int get_num_frames() const { return 1; }

	//: Get red mask
	virtual unsigned int get_red_mask() const;

	//: Get green mask
	virtual unsigned int get_green_mask() const;

	//: Get blue mask
	virtual unsigned int get_blue_mask() const;

	//: Get alpha mask
	virtual unsigned int get_alpha_mask() const;

	//: Get depth
	virtual unsigned int get_depth() const;

	//: Returns the palette used by the image. NULL if system palette.
	virtual CL_Palette *get_palette() const { return palette; }

	//: Is indexed
	virtual bool is_indexed() const { return true; }

	//: Uses src colour key	
	virtual bool uses_src_colorkey() const { return trans_col != -1; }

	//: Returns the transparency color used, -1 if none.
	virtual unsigned int get_src_colorkey() const { return trans_col; }

	//: Returns the image data. Provider must be locked before pointer is valid.
	virtual void *get_data() const { return image+bounding_left+bounding_top*pitch; }

//! Operations:
	//: Locks the surface provider.
	virtual void perform_lock();

	//: Unlocks the surface provider.
	virtual void perform_unlock();

	//: Loads header data into class variables.  Called by read_data().
	virtual void read_header(CL_InputSource *_datafile);

	//: Decodes image data
	virtual void read_data();
	
//! Implementation:
private:
	//PCX specific variables
	int num_planes;			// Number of color planes in file data
	int dest_num_planes;		// Number of color planes in final image format
	int size_data;			// Size of data in file
	unsigned char pcx_version;	// PCX Version

	
	unsigned char bits_per_pixel_per_plane;		// Bit per pixel per plane
	
	int decode_pitch;		// Bytes to decode per scanline (different than pitch)
	
	int bytes_to_allocate;		// Bytes to allocate for decoding process.
					// This is not necessarily the same as the image size.
	
	//Surface Provider variables
	CL_InputSourceProvider *provider;
	int pitch;			// Bytes per scanline in decoded image
	int height;
	int bounding_left, bounding_top, bounding_right, bounding_bottom;
	bool transparent;
	int trans_col;
	std::string name;
	EPixelFormat pixelformat;

	CL_Palette *palette;
	unsigned char *image;
};

#endif
