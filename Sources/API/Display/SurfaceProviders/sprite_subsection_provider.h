/*
	$Id: sprite_subsection_provider.h,v 1.5 2001/09/22 15:52:12 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

	File purpose:
		Subsection surface provider header file

*/

//! clanDisplay="Surface Providers"
//! header=display.h

#ifndef header_sprite_subsection_provider
#define header_sprite_subsection_provider

#include "generic_surfaceprovider.h"

//: The Sprite Sub Section Provider Class
class CL_SpriteSubsectionProvider : public CL_SurfaceProvider_Generic
{
	CL_SurfaceProvider *parent_provider;
	bool locked;
	int start_x, start_y;
	int width, height;
public:
//! Construction:
	//: Create
	static CL_Surface *create(CL_SurfaceProvider *parent_provider, int start_x, int start_y, int width, int height);

	//: The Sprite Sub Section Provider Constructor
	CL_SpriteSubsectionProvider(CL_SurfaceProvider *parent_provider, int start_x, int start_y, int width, int height);

	//: The Sprite Sub Section Provider Destructor
	virtual ~CL_SpriteSubsectionProvider();

//! Attributes:
	//: Get pitch
	virtual unsigned int get_pitch() const;

	//: Get translate x
	virtual int get_translate_x() const;

	//: Get translate y
	virtual int get_translate_y() const;

	//: Get width
	virtual unsigned int get_width() const;

	//: Get height
	virtual unsigned int get_height() const;

	//: Get num frames
	virtual unsigned int get_num_frames() const;

	//: *Get palette
	virtual CL_Palette *get_palette() const;

	//: Is indexed
	virtual bool is_indexed() const;

	//: Uses src colorkey
	virtual bool uses_src_colorkey() const;

	//: Get src colorkey
	virtual unsigned int get_src_colorkey() const;

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

	//: Get data
	virtual void *get_data() const;

//! Operations:
	//: Perform lock
	virtual void perform_lock();

	//: Perform unlock
	virtual void perform_unlock();

};

#endif
