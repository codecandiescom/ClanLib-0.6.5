/*
	$Id: sprite.h,v 1.6 2001/12/11 20:44:19 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

	File purpose:
		Simple sprite support.

*/

//! clanDisplay="Surface Providers"
//! header=display.h

#ifndef header_sprite
#define header_sprite

#include "generic_surfaceprovider.h"
#include "../../Core/IOData/inputsource.h"
#include "../../Core/IOData/inputsource_provider.h"

//: ClanLib datafile sprite.
//- Used to load surfaces stored in ClanLib's datafile sprite format.
class CL_SpriteProvider : public CL_SurfaceProvider_Generic
{
protected:
	std::string surface_id;
	CL_InputSourceProvider *provider;
	void *surface_data;
	int width, height, no_sprs, transcol;
	CL_Palette *palette;
	unsigned int red_mask, green_mask, blue_mask, alpha_mask;
	unsigned int bpp;
	bool m_is_indexed;

	void load_data();

public:
//! Construction:
	//: Loads the sprite 'surface_id' from the inputsource provider 'datafile'. 
	//: Creates a surface using this sprite (surfaceprovider) and returns it.
	//: This function is a easier way of typing:
	//: CL_Surface::create(new CL_Sprite(surface_id, datafile), true);
	static CL_Surface *create(const std::string &surface_id, CL_InputSourceProvider *datafile);

	//: <p>Constructs a surface provider that represents the sprite 'surface_id' 
	//: from the inputsource provider 'datafile'.</p>
	CL_SpriteProvider(const std::string &surface_id, CL_InputSourceProvider *datafile);

	//: Sprite Provider Destructor
	virtual ~CL_SpriteProvider();

//! Attributes:
	//: Returns the width of the sprite.
	virtual unsigned int get_width() const;

	//: Returns the height of the sprite.
	virtual unsigned int get_height() const;

	//: Returns the pitch of the sprite.
	virtual unsigned int get_pitch() const;
	
	//: Returns the number of subsprites in this sprite.
	virtual unsigned int get_num_frames() const;
	
	//: <p>Returns the palette used by the surface. NULL if the system palette is 
	//: used.</p>
	virtual CL_Palette *get_palette() const;

	//: Uses src colourkey
	virtual bool uses_src_colorkey() const;

	//: Returns the transparency color used, or -1 if none.
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

	//: Is indexed
	virtual bool is_indexed() const;
	
	//: <p>Returns a pointer to the sprites image data. Can only be called between 
	//: lock() and unlock().</p>
	virtual void *get_data() const;

//! Operations:
	//: <p>Locks the surface provider. This causes the sprite to retrieve its 
	//: surface data from the datafile.</p>
	virtual void perform_lock();
	
	//: <p>Unlocks the surface provider. The sprite releases its surface data when
	//: it is called.</p>
	virtual void perform_unlock();
};

#endif
