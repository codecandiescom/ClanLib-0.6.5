/*
	$Id: jpgd_provider.h,v 1.8 2001/09/22 15:52:18 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanJPEG="System"
//! header=jpeg.h

#ifndef header_jpgd_provider
#define header_jpgd_provider

#include "../Display/Display/surfaceprovider.h"
#include <string>

class CL_InputSourceProvider;
class CL_Palette;

//: JPGD Provider Class
class CL_JPGDProvider : public CL_SurfaceProvider
{
public:
//! Construction:
	//: JPGD Provider Constructor
	CL_JPGDProvider(
		const std::string &filename,
		CL_InputSourceProvider *provider = 0);

	//: JPGD Provider Destructor
	~CL_JPGDProvider();
		
//! Operations:
	//: Lock
	virtual void lock();
	
	//: Unlock
	virtual void unlock();

//! Attributes:
	//: Get data
	virtual void *get_data() const;

	//: Get num frames
	virtual unsigned int get_num_frames() const;

	//: Get width
	virtual unsigned int get_width() const;

	//: Get height
	virtual unsigned int get_height() const;

	//: Get pitch
	virtual unsigned int get_pitch() const;

	//: Get bytes per pixel
	virtual unsigned int get_bytes_per_pixel() const;

	//: Is indexed
	virtual bool is_indexed() const;

	//: Get red mask
	virtual unsigned int get_red_mask() const;

	//: Get green mask
	virtual unsigned int get_green_mask() const;

	//: Get blue mask
	virtual unsigned int get_blue_mask() const;

	//: Get alpha mask
	virtual unsigned int get_alpha_mask() const;

	//: Get palette
	virtual CL_Palette *get_palette() const;

	//: Uses src colorkey
	virtual bool uses_src_colorkey() const;

	//: Get src colorkey
	virtual unsigned int get_src_colorkey() const;

//! Implementation:
private:
	std::string filename;
	CL_InputSourceProvider *provider;
	unsigned int width, height, components;
	unsigned int *data;
	int ref_count;
};

#endif
