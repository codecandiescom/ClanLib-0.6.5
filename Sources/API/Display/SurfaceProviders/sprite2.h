/*
	$Id: sprite2.h,v 1.5 2001/09/22 15:52:12 plasmoid Exp $

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

#ifndef header_sprite2
#define header_sprite2

#include "generic_surfaceprovider.h"
#include "../../Core/IOData/inputsource.h"
#include "../../Core/IOData/inputsource_provider.h"
#include "../Display/pixelformat.h"

//: Sprite 2 Provider Class
//- <p>Sprite surface provider especially designed for the font class.
//- Most likely not very usable other places.</p>
class CL_Sprite2Provider : public CL_SurfaceProvider_Generic
{
protected:
	std::string surface_id;
	void *surface_data;
	int width, height, no_sprs, transcol;
	CL_Palette *palette;
	EPixelFormat pixelformat;
	unsigned int red_mask, green_mask, blue_mask, alpha_mask;
	unsigned int bpp;
	bool m_is_indexed;

	void load_data(CL_InputSource *datafile);

public:
//! Construction:
	//: Sprite 2 Provider Constructor
	CL_Sprite2Provider(CL_InputSource *source);

	//: Sprite 2 Provider Constructor
	CL_Sprite2Provider(
		CL_SurfaceProvider *src,
		int x, int y, int width, int height,
		int *tcols, int tcols_num);

	//: Sprite 2 Provider Destructor
	virtual ~CL_Sprite2Provider();

//! Attributes:
	//: Returns the width of the sprite2.
	virtual unsigned int get_width() const;

	//: Returns the height of the sprite2.
	virtual unsigned int get_height() const;

	//: Returns the pitch of the sprite2.
	virtual unsigned int get_pitch() const;
	
	//: Returns the number of subsprite2s in this sprite2.
	virtual unsigned int get_num_frames() const;
	
	//: <p>Returns the palette used by the surface. NULL if the system palette is 
	//: used.</p>
	virtual CL_Palette *get_palette() const;

	//: Uses src colourkey
	virtual bool uses_src_colorkey() const;

	//: Returns the transparency color used, or -1 if none.
	virtual unsigned int get_src_colorkey() const;

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

	//: Get depth
	virtual unsigned int get_depth() const;
	
	//: <p>Returns a pointer to the sprite2s image data. Can only be called between 
	//: lock() and unlock().</p>
	virtual void *get_data() const;

//! Operations:
	//: <p>Locks the surface provider. This causes the sprite2 to retrieve its 
	//: surface data from the datafile.</p>
	virtual void perform_lock();
	
	//: <p>Unlocks the surface provider. The sprite2 releases its surface data when
	//: it is called.</p>
	virtual void perform_unlock();
};

#endif
