/*
	$Id: provider_jpeg_generic.h,v 1.3 2001/09/08 19:12:52 japj Exp $
	
	------------------------------------------------------------------------
	JPEG-SurfaceProvider Class
	written by Ingo Ruhnke

	in order to work with
	ClanLib, the platform independent game SDK.

	This file is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_provider_jpeg_generic
#define header_provider_jpeg_generic

#ifdef WIN32
#define XMD_H
#endif

#include <stdio.h>
extern "C"
{
	#include <jpeglib.h>
}

#include "API/core.h" // todo: include the correct stuff.
#include "API/Display/Display/pixelformat.h"
#include "API/Core/System/clanstring.h"

class CL_Surface;
class CL_InputSourceProvider;
class CL_Palette;

class CL_JPEGProvider_Generic
//: Surface provider that can load JPEG (.jpg) files.
{
public:
	CL_JPEGProvider_Generic(
		CL_String name, 
		CL_InputSourceProvider *provider = NULL,
		bool transparent=true,
		bool ignore_alphachannel=false);
	//: Constructs a surface provider that can read PNG files.
	//!param: name - Name of the PNG file to load.
	//!param: provider - Input source provider that delivers the JPEG file.
	//!param: transparent - True if a transparency color should be used.
	//!param: ignore_alphachannel - True if the alpha component should be ignored.
	//!param: trans_red - Red component of the transparency color.
	//!param: trans_green - Green component of the transparency color.
	//!param: trans_blue - blue component of the transparency color.

	~CL_JPEGProvider_Generic();

	unsigned int get_pitch() const { return pitch; }
	//: Returns the pitch of the image (bytes per line).

	unsigned int get_width() const { return width; }
	//: Returns the width of the image.

	unsigned int get_height() const { return height; }
	//: Returns the height of the image.
	
	unsigned int get_num_frames() const { return 1; }
	//: Returns the number of subsprites in the image.

	EPixelFormat get_pixel_format() const { return RGB888; }
	//: Returns the pixelformat used by the image.

	CL_Palette *get_palette() const { return NULL; }
	//: Returns the palette used by the image. NULL if system palette.

	unsigned int get_src_colorkey() const { return 0; }
	//: Returns the transparency color used.

	bool uses_src_colorkey() const { return false; }
	//: Returns whether a source colorkey is used.

	bool is_indexed() const { return false; }
	//: Returns whether the target uses an indexed color mode or not.
	
	unsigned int get_red_mask() const;
	//: Returns the red color mask used by the target.

	unsigned int get_green_mask() const;
	//: Returns the green color mask by the target.

	unsigned int get_blue_mask() const;
	//: Returns the blue color mask by the target.

	unsigned int get_alpha_mask() const;
	//: Returns the alpha mask by the target.

	void *get_data() const;
	//: Returns the image data. Provider must be locked before pointer is valid.

	void perform_lock();
	//: Locks the surface provider.

	void perform_unlock();
	//: Unlocks the surface provider.

private:
	CL_String filename;
	int locked;
	
	unsigned char *image;

	int pitch;
	int width, height;

	int bpp;

	bool transparent, ignore_alphachannel, use_alphapixels;

	unsigned char trans_redcol, trans_greencol, trans_bluecol;

	int pos;

	typedef struct
	{
		struct jpeg_source_mgr   pub;           /* public fields */
		CL_JPEGProvider_Generic* jpeg_provider; /* source stream */
		JOCTET *                 buffer;        /* start of buffer */
		boolean                  start_of_file; /* have we gotten any data yet? */
	} InputSource_source_mgr;
	
	typedef InputSource_source_mgr* InputSource_src_ptr;

	// Functions for managing the input handling
	static void jpeg_InputSource_src (j_decompress_ptr cinfo, CL_JPEGProvider_Generic* prov); 
	static void    init_source (j_decompress_ptr cinfo);
	static boolean fill_input_buffer (j_decompress_ptr cinfo);
	static void    skip_input_data (j_decompress_ptr cinfo, long num_bytes);
	static void    term_source (j_decompress_ptr cinfo);

	CL_InputSourceProvider* input_provider;
	CL_InputSource* input_source;
};

#endif
