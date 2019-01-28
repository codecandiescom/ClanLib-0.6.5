/*
	$Id: surface.h,v 1.11 2002/06/13 23:44:02 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanDisplay="Display 2D"
//! header=display.h

#ifndef header_surface
#define header_surface

#include "../../Core/Resources/resource.h"

class CL_DisplayCard;
class CL_SurfaceProvider;
class CL_Target;
class CL_ResourceManager;
class CL_Surface_Generic;

//: Image class in ClanLib.
//- <p>This class is used to draw 2D images onto the backbuffer or to a
//- CL_Target renderer target.</p>
//-
//- <p>In ClanLib, a surface is understood
//- to be an image represented in a form, where it is most suitable for as
//- fast rendering as possible. Unlike many other libraries, it is not
//- possible to get direct access to the surface data.</p>
//-
//- <p>A surface uses a CL_SurfaceProvider surface provider to
//- load its data. It is passed at creation time, and you can either roll
//- your own provider, or use one already available in ClanLib.</p>
//-
//- <p>It is also possible to create a surface from a resource source. This
//- will allow you to load an image using a resource ID instead of directly
//- specifying the image in your source code.</p>
//-
//- Dynamic surfaces
//-
//- <p>If you have an image which changes very frequently (eg. each
//- frame), you want to use a dynamic surface instead. The difference is that
//- the surface won't cache the image, and it won't convert it to another
//- image format more suitable for the display target. Note that this is only
//- faster if you change your image very frequently - otherwise a normal
//- surface is the best solution.</p>
//-
//- <p>When constructing your own images, you will most likely find the 
//- CL_Canvas canvas surface provider interesting. This class
//- will make it easier to create a custom drawing area where you can draw
//- your image.</p>
//-
//- <p>All surfaceproviders is a CL_Target renderer target.
//- However, most of the normal surface providers are "read only", meaning
//- that their image data will only remain intact during a lock session. This
//- is because the providers consume less memory when the image is only
//- available, when someone needs it. Only the CL_Canvas canvas
//- provider will not destroy the image.</p>
//-
//- <p>A surface can blit (draw itself) itself to the backbuffer, or to a
//- renderer target. This means that you can use the surface to draw one
//- surface provider to another.</p>
class CL_Surface
{
//! Static_init:
public:
	//: This function loads a surface from a resource file (usually a datafile).
	static CL_Surface *load(
		const std::string &resource_id,
		CL_ResourceManager *manager);

	//: This function creates a CL_Surface instance being blitted by a caching blitter.
	//- provider - Image source.
	//- delete_provider - If true, the surface will delete the provider when deleted itself.
	//- Returns - The surface created.
	static CL_Surface *create(
		CL_SurfaceProvider *provider,
		bool delete_provider=false);

	//: This function creates a CL_Surface instance being blitted by the dynamic (non caching) blitter.
	//: You don't need to call reload with dynamic surfaces as they do that by themselves. 
	//- provider - Image source.
	//- delete_provider - If true, the surface will delete the provider when deleted itself.
	//- Returns - The surface created.
	static CL_Surface *create_dynamic(
		CL_SurfaceProvider *provider,
		bool delete_provider=false);

//! Construction:
public:
	//: Empty constructor, you need to assign a surface to the
	//: CL_Surface object first before it becomes useable
	CL_Surface();

	//: This function creates a CL_Surface.
	//: If dynamic is true, the surface will be a dynamic blitter. If false, it will be a caching blitter.
	//- provider - Image source.
	//- delete_provider - If true, the surface will delete the provider when deleted itself.
	//- dynamic - If true, a dynamic blitter will be chosen. If false, a caching blitter will be used.
	//- Returns - The surface created.
	CL_Surface(
		CL_SurfaceProvider *provider,
		bool delete_provider = false,
		bool dynamic = false);

	//: Loads a surface from a resource file.
	CL_Surface(
		const std::string &resource_id,
		CL_ResourceManager *manager);

	//: Loads a surface from an image file.
	CL_Surface(const std::string &filename, bool dynamic = false);

	//: Make a copy of an other surface, but share the image data.
	CL_Surface(const CL_Surface &surface);

	//: Surface Destructor
	virtual ~CL_Surface();

//! Operations:
public:
	//: You can use this operator to check if the surface is
	//: valid, to do that you write something like this: if (surf) {...}
	operator bool () const;

	//: Surface operator
	const CL_Surface& operator=(const CL_Surface& surf);

	//: Forces surface to reload surface data from provider.
	void reload();

	//: Returns the surface provider used by this surface.
	//- Returns - Surface provider used by this surface.
	CL_SurfaceProvider *get_provider() const;

	//: Draws the surface onto the backbuffer.
	//- x - x coordinate to draw surface.
	//- y - y coordinate to draw surface.
	//- spr_no - subsprite number to use.
	//- card - Display card to be drawn onto. If NULL it will use the currently selected video card by CL_Display.
	void put_screen(
		int x,
		int y,
		int spr_no=0,
		CL_DisplayCard *card=NULL);

	//: Draws the surface onto the backbuffer - scaling it to a specified size.
	//- x - x coordinate to draw surface.
	//- y - y coordinate to draw surface.
	//- scale_x - scale x multiplyer.
	//- scale_y - scale y multiplyer.
	//- spr_no - subsprite number to use.
	//- card - Display card to be drawn onto. If NULL it will use the currently selected video card by CL_Display.
	void put_screen(
		int x,
		int y,
		float scale_x,
		float scale_y,
		int spr_no=0,
		CL_DisplayCard *card=NULL);

	//: Draws the surface onto the backbuffer - scaling it to a specified size.
	//- x - x coordinate to draw surface.
	//- y - y coordinate to draw surface.
	//- size_x - width of the image when scaled.
	//- size_y - height of the image when scaled.
	//- spr_no - subsprite number to use.
	//- card - Display card to be drawn onto. If NULL it will use the currently selected video card by CL_Display.
	void put_screen(
		int x,
		int y,
		int size_x,
		int size_y,
		int spr_no=0,
		CL_DisplayCard *card=NULL);

	//: Draws the surface to a target.
	//- x - x coordinate to draw surface.
	//- y - y coordinate to draw surface.
	//- spr_no - subsprite number to use.
	//- target - target.
	void put_target(
		int x,
		int y,
		int spr_no,
		CL_Target *target);

	//: Returns the width of the surface
	unsigned int get_width() const;

	//: Returns the height of the surface
	unsigned int get_height() const;

	//: Returns the number of frames/subsprites in the surface
	unsigned int get_num_frames() const;

	//: returns true if in videomemory 
	//: on the specified card (null = current dispcard).
	bool is_video(CL_DisplayCard *card = NULL) const;
	
	//: returns true if loaded in either video- or system-memory
	bool is_loaded(CL_DisplayCard *card = NULL) const;

	//: returns true if successfully loaded into videomemory, or already there
	bool convert_video(CL_DisplayCard *card = NULL);

	//: convert surface to system memory - never fails! (or serious heap usage!)
	bool convert_system(CL_DisplayCard *card = NULL);

	//: completely flushes surface (removes from video/system-memory)
	void flush(CL_DisplayCard *card = NULL);

	//: Return the number of references to this surface
	int get_reference_count ();

//! Implementation:
public:
	//: Surface Constructor
	CL_Surface(class CL_Surface_Generic *impl);

	//: Surface Implementation
	CL_Surface_Generic *impl;

	//: Surface resource, if any.
	CL_Resource resource;
};

#endif
