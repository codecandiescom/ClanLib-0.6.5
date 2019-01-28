/*
	$Id: provider_jpeg.h,v 1.15 2002/01/22 10:29:32 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	written by Ingo Ruhnke
	in order to work with
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanJPEG="Surface Providers"
//! header=jpeg.h

#ifndef header_provider_jpeg
#define header_provider_jpeg

#include "../Display/Display/surfaceprovider.h"

class CL_Surface;
class CL_String;
class CL_InputSourceProvider;
class CL_JPEGProvider_Generic;

//: Surface provider that can load JPEG (.jpg) files.
class CL_JPEGProvider : public CL_SurfaceProvider
{
//! Construction:
public:
	//: Loads the JPEG file 'handle' from the inputsource provider 'provider. 
	//: Creates a CL_Surface using the JPEG image and returns it.
	static CL_Surface *create(
		const CL_String &handle, 
		CL_InputSourceProvider *provider=NULL, 
		bool transparent=true,
		bool ignore_alphachannel=false);

	//: Constructs a surface provider that can read PNG files.
	//- name - Name of the PNG file to load.
	//- provider - Input source provider that delivers the JPEG file.
	//- transparent - True if a transparency color should be used.
	//- ignore_alphachannel - True if the alpha component should be ignored.
	//- trans_red - Red component of the transparency color.
	//- trans_green - Green component of the transparency color.
	//- trans_blue - blue component of the transparency color.
	CL_JPEGProvider(
		CL_String name, 
		CL_InputSourceProvider *provider = NULL,
		bool transparent=true,
		bool ignore_alphachannel=false);

	//: JPEG Provider Destructor
	virtual ~CL_JPEGProvider();

//! Attributes:
	//: Returns the pitch of the image (bytes per line).
	virtual unsigned int get_pitch() const;

	//: Returns the width of the image.
	virtual unsigned int get_width() const;

	//: Returns the height of the image.
	virtual unsigned int get_height() const;
	
	//: Returns the number of subsprites in the image.
	virtual unsigned int get_num_frames() const;

	//: Returns the pixelformat used by the image.
	virtual EPixelFormat get_pixel_format() const;

	//: Returns the palette used by the image. NULL if system palette.
	virtual CL_Palette *get_palette() const;

	//: Returns the transparency color used.
	virtual unsigned int get_src_colorkey() const;

	//: Returns whether a source colorkey is used.
	virtual bool uses_src_colorkey() const;

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

	//: Returns the image data. Provider must be locked before pointer is valid.
	virtual void *get_data() const;

//! Operations:
	//: Locks the surface provider.
	virtual void lock();

	//: Unlocks the surface provider.
	virtual void unlock();

//! Implementation:
private:
	CL_JPEGProvider_Generic *impl;
};

//: Setup JPEG Class
class CL_SetupJPEG
{
public:
//! Operations:
	//: Init
	static void init(bool register_resources_only = false);

	//: DeInit
	static void deinit();
};

#endif
