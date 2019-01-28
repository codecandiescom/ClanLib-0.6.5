/*
	$Id: provider_bmp.h,v 1.5 2001/09/22 15:52:12 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

	File purpose:
		BMP surface provider.
*/

//! clanDisplay="Surface Providers"
//! header=display.h

#ifndef header_bmpprovider
#define header_bmpprovider

#define BITMAP_ID     0x4D42      // universal id for a bitmap

/* Compression encodings for BMP files */

#ifndef WIN32
#define BI_RGB		0
#define BI_RLE8		1
#define BI_RLE4		2
#define BI_BITFIELDS	3
#endif

#include "generic_surfaceprovider.h"
#include "../Display/pixelformat.h"
#include <fstream>

class CL_BMPProvider : public CL_SurfaceProvider_Generic
//: Surface provider that can load BMP files.
//- This code currently supports Win32 DIBs in uncompressed 8 and 24 bpp.
{
public:
	
//! Construction:
	//: Loads the bmp file 'handle' from the inputsource provider 'provider. 
	//: Creates a CL_Surface using the bmp image and returns it.
	static CL_Surface *create(
		std::string handle,
		CL_InputSourceProvider *provider,
		bool transparent=false,
		short trans_col=-1);

	//: Constructs a surface provider that can read bmp files.
	//- name - Name of the bmp file to load.
	//- provider - Input source provider that delivers the bmp file.
	//- transparent - True if a color in image should be transparent.
	//- trans_col - Transparency color used if 'transparent' is true. Defaults to color 0.
	CL_BMPProvider(
		std::string name,
		CL_InputSourceProvider *provider,
		bool transparent=false,
		short trans_col=-1);

	//: BMP Provider Destructor
	virtual ~CL_BMPProvider();

//! Attributes:
	//: Returns the pitch of the image (bytes per line).
	virtual unsigned int get_pitch() const;

	//: Returns the width of the image.
	virtual unsigned int get_width() const { return biWidth; }

	//: Returns the height of the image.
	virtual unsigned int get_height() const { return biHeight; }

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

	//: Is Indexed
	virtual bool is_indexed() const { return biBitCount == 8;}

	//: Uses src colourkey	
	virtual bool uses_src_colorkey() const { return trans_col != -1; }

	//: Returns the transparency color used, -1 if none.
	virtual unsigned int get_src_colorkey() const { return trans_col; }

	//: Returns the image data. Provider must be locked before pointer is valid.
	virtual void *get_data() const { return image; }

//! Operations:
	//: Locks the surface provider.
	virtual void perform_lock();

	//: Unlocks the surface provider.
	virtual void perform_unlock();

//! Implementation:
private:
	CL_InputSourceProvider *provider;
	bool transparent;
	int trans_col;
	std::string name;

	/* The Win32 BMP file header (14 bytes) */
	char   magic[2];
	unsigned int bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int bfOffBits;

	/* The Win32 BITMAPINFOHEADER struct (40 bytes) */
	unsigned int biSize;
	int biWidth;
	int biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	unsigned int biClrUsed;
	unsigned int biClrImportant;


	CL_Palette *palette;
	unsigned char *image;
};

#endif
