/*
	$Id: provider_png.h,v 1.18 2002/01/22 10:29:32 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	PNG-SurfaceProvider Class
	written by Karsten-Olaf Laux

	in order to works with
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

	File purpose:
		PNG (.png) surface provider.
*/

//! clanPNG="Surface Providers"
//! header=png.h

#ifndef header_PNGprovider
#define header_PNGprovider

#ifdef WIN32
#pragma warning( disable : 4786)
#endif

#include "../Display/Display/pixelformat.h"
#include "../Display/SurfaceProviders/generic_surfaceprovider.h"
#include "../Core/System/clanstring.h"
#include "../Core/System/cl_assert.h"
#include "../Core/IOData/inputsource.h"
#include "../Core/IOData/inputsource_provider.h"

#include <png.h>

//: Surface provider that can load PNG (.png) files.
class CL_PNGProvider : public CL_SurfaceProvider_Generic
{
public:
//! Construction:
	//: Loads the PNG file 'handle' from the inputsource provider 'provider. 
	//: Creates a CL_Surface using the PNG image and returns it.
	static CL_Surface* create(CL_String handle, 
				  CL_InputSourceProvider* provider=NULL, 
				  bool transparent=true,
				  bool ignore_alphachannel=false);

	//: Constructs a surface provider that can read PNG files.
	//- name - Name of the PNG file to load.
	//- provider - Input source provider that delivers the PNG file.
	//- transparent - True if a transparency color should be used.
	//- ignore_alphachannel - True if the alpha component should be ignored.
	//- trans_red - Red component of the transparency color.
	//- trans_green - Green component of the transparency color.
	//- trans_blue - blue component of the transparency color.
	CL_PNGProvider(CL_String name, 
			 CL_InputSourceProvider *provider = NULL,
			 bool transparent=true,
			 bool ignore_alphachannel=false);

	// PNG Provider Destructor
	virtual ~CL_PNGProvider();

//! Attributes:
	//: returns Pointer to CL_InputSource
	//: (used by libpng-callback CL_PNGProvider::pngread_file() )
	CL_InputSource* get_input_source() {return input_source; };

	//: Returns the pitch of the image (bytes per line).
	virtual unsigned int get_pitch() const { return pitch; }

	//: Returns the width of the image.
	virtual unsigned int get_width() const { return width; }

	//: Returns the height of the image.
	virtual unsigned int get_height() const { return height; }
	
	//: Returns the number of subsprites in the image.
	virtual unsigned int get_num_frames() const { return 1; }

	//: Returns the pixelformat used by the image.
	virtual EPixelFormat get_pixel_format() const { return pixel_format; }

	//: Returns the palette used by the image. NULL if system palette.
	virtual CL_Palette *get_palette() const { return palette; }

	//: Returns the transparency color used.
	virtual unsigned int get_src_colorkey() const { return trans_col; }

	//: Returns whether a source colorkey is used.
	virtual bool uses_src_colorkey() const { return m_uses_src_colorkey; }

	//: Returns whether the target uses an indexed color mode or not.
	virtual bool is_indexed() const;
	
	//: Returns the red color mask used by the target.
	virtual unsigned int get_red_mask() const;

	//: Returns the green color mask by the target.
	virtual unsigned int get_green_mask() const;

	//: Returns the blue color mask by the target.
	virtual unsigned int get_blue_mask() const;

	//: Returns the alpha mask by the target.
	virtual unsigned int get_alpha_mask() const;

	//: Returns the bit depth of the surface provider
	virtual unsigned int get_depth() const;

	//: Returns the image data. Provider must be locked before pointer is valid.
	virtual void* get_data() const;

//! Operations:
	//: Locks the surface provider.
	virtual void perform_lock();

	//: Unlocks the surface provider.
	virtual void perform_unlock();
	
	//: Callback used by libpng to retrieve the filedata. 
	//: (calls get_input_source()->read_uchar8())
	static void pngread_file(png_structp png_ptr,
				png_bytep data, 
				png_size_t length)
	{
		// since this method is static, we need to know who we are ...
		CL_PNGProvider *instance =  (CL_PNGProvider *)png_get_io_ptr(png_ptr);
		// no error-checking here ....
		unsigned int read_length = instance->get_input_source ()->read(data, length);
		cl_assert (read_length == length);
	}

//! Implementation:
private:
	CL_String filename;
	int locked;

	bool m_uses_src_colorkey;
	
	unsigned char *image;
	int pitch;
	int no_sprs;
	int width;
	int height;

	EPixelFormat pixel_format;

	bool transparent;
	bool ignore_alphachannel;

	CL_Palette* palette;
	bool indexed;
	int trans_col;
	int color_type;
	unsigned char trans_redcol, trans_greencol, trans_bluecol;

	void read_data();
	void read_data_rgb();
	void read_data_rgba();
	void read_data_grayscale();
	void read_data_grayscale_alpha();
	void read_data_palette();

	CL_InputSourceProvider *provider;
	CL_InputSource *input_source;

	//PNGlib stuff:
	png_structp png_ptr;
	png_infop info_ptr;
	png_infop end_info;

};

//: Setup PNG Class
class CL_SetupPNG
{
public:
//! Operations:
	//: Init
	static void init(bool register_resources_only = false);

	//: DeInit
	static void deinit();
};

#endif
