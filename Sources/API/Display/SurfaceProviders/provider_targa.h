/*
	$Id: provider_targa.h,v 1.6 2002/04/06 19:50:45 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

	File purpose:
		Targa (.tga) surface provider.
*/

//! clanDisplay="Surface Providers"
//! header=display.h

#ifndef header_targaprovider
#define header_targaprovider

#include "generic_surfaceprovider.h"
#include "../../Core/IOData/inputsource.h"
#include "../../Core/IOData/inputsource_provider.h"

//: Surface provider that can load targa (.tga) files.
class CL_TargaProvider : public CL_SurfaceProvider_Generic
{
	std::string filename;
	bool locked;
	unsigned char *file;
	int filesize;

	unsigned char *image;

	unsigned short map_length;
	unsigned char *color_map;

	unsigned char datatype;

	short pitch, height;
	int no_sprs, trans_col;
	int bounding_left, bounding_top, bounding_right, bounding_bottom;
	int map_direction_x, map_direction_y;

	unsigned char bpp;

	bool transparent, ignore_alphachannel, use_alphapixels;
	unsigned char trans_redcol, trans_greencol, trans_bluecol;

	int pos;

	// returns true if non-transparent pixel read
	inline bool read_rgb_16(
		unsigned char *a, 
		unsigned char *b, 
		unsigned char *g, 
		unsigned char *r);
	inline bool read_rgb_24(
		unsigned char *a, 
		unsigned char *b, 
		unsigned char *g, 
		unsigned char *r);
	inline bool read_rgb_32(
		unsigned char *a, 
		unsigned char *b, 
		unsigned char *g, 
		unsigned char *r);
	inline bool read_rgb(
		unsigned char *a, 
		unsigned char *b, 
		unsigned char *g, 
		unsigned char *r);

	void read_from_colormap(unsigned char *a,
							unsigned char *b,
							unsigned char *g,
							unsigned char *r);
							

	void read_data();
	void read_header(bool read_colormap);

	void read_colormapped();
	void read_uncompressed_rgb();
	void read_runlength_encoded_colormapped_rgb();
	void read_runlength_encoded_rgb();

	CL_InputSourceProvider *provider;
	CL_InputSource *input_source;
public:
//! Construction:
	//: Loads the targa file 'handle' from the inputsource provider 'provider. 
	//: Creates a CL_Surface using the targa image and returns it.
	//: If you just want to load a file, use 'handle' for filename, and set provider to NULL
	static CL_Surface *create(
		std::string handle,
		CL_InputSourceProvider *provider, 
		bool transparent=false,
		bool ignore_alphachannel=false,
		unsigned char trans_red=0, 
		unsigned char trans_green=0, 
		unsigned char trans_blue=0);

	//: Constructs a surface provider that can read targa files.
	//- name - Name of the targa file to load.
	//- provider - Input source provider that delivers the targa file.
	//- transparent - True if a transparency color should be used.
	//- ignore_alphachannel - True if the alpha component should be ignored.
	//- trans_red - Red component of the transparency color.
	//- trans_green - Green component of the transparency color.
	//- trans_blue - blue component of the transparency color.
	CL_TargaProvider(
		std::string name,
		CL_InputSourceProvider *provider,
		bool transparent=false,
		bool ignore_alphachannel=false,
		unsigned char trans_red=0, 
		unsigned char trans_green=0, 
		unsigned char trans_blue=0);

	//: Target Provider Destructor
	virtual ~CL_TargaProvider();

//! Attributes:
	//: Returns the pitch of the image (bytes per line).
	virtual unsigned int get_pitch() const { return pitch*4; }

	//: Returns how many pixels to translate the image (X axis).
	virtual int get_translate_x() const { return bounding_left; }

	//: Returns how many pixels to translate the image (Y axis).
	virtual int get_translate_y() const { return bounding_top; }

	//: Returns the width of the image.
	virtual unsigned int get_width() const { return bounding_right-bounding_left+1; }

	//: Returns the height of the image.
	virtual unsigned int get_height() const { return bounding_bottom-bounding_top+1; }
	
	//: Returns the number of subsprites in the image.
	virtual unsigned int get_num_frames() const { return no_sprs; }

	//: Returns the pixelformat used by the image.
	virtual unsigned int get_depth() const { return 32; }

	//: Get red mask
	virtual unsigned int get_red_mask() const   { return 0xff000000; }

	//: Get green mask
	virtual unsigned int get_green_mask() const { return 0x00ff0000; }

	//: Get blue mask
	virtual unsigned int get_blue_mask() const  { return 0x0000ff00; }

	//: Get alpha mask
	virtual unsigned int get_alpha_mask() const { if ((!transparent) && (ignore_alphachannel)) return 0x0; else return 0x000000ff; }

	//: Is indexed
	virtual bool is_indexed() const { return false; }

	//: Returns the palette used by the image. NULL if system palette.
	virtual CL_Palette *get_palette() const { return NULL; }

	//: Uses src colour key
	virtual bool uses_src_colorkey() const { return trans_col != -1; }

	//: Returns the transparency color used, -1 if none.
	virtual unsigned int get_src_colorkey() const { return trans_col; }

	//: Returns the image data. Provider must be locked before pointer is valid.
	virtual void *get_data() const;

//! Operations:
	//: Locks the surface provider.
	virtual void perform_lock();

	//: Unlocks the surface provider.
	virtual void perform_unlock();

};

#endif
