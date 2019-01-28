/*
	$Id: canvas.h,v 1.9 2002/06/13 15:01:55 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

	File purpose:
		Empty space dynamic surface provider header file

*/

//! clanDisplay="Surface Providers"
//! header=display.h

#ifndef header_canvas
#define header_canvas

#include "generic_surfaceprovider.h"
#include "../../Core/System/cl_assert.h"

//: Empty surfaceprovider to be used as a rendering target.
//- <p>CL_Canvas is a surface provider useful to create images at
//- run-time.</p>
//-
//- <p>When constructed, the surface provider will contain an empty image
//- buffer of the specified dimensions. It is then the intention that the
//- game fills the buffer with data.</p>
//-
//- <p>There are three ways to do this (and you can mix them as you
//- please):</p>
//-
//- 
//- <p>Lock() the provider, and then call get_data() to get a pointer to the
//- image buffer. Party on it. And then finally unlock() the provider.</p>
//-
//- <p>Use the drawing primitives available in CL_Target. This includes line
//- drawing, fill rects and clipping.</p>
//-
//- <p>Use the put_target() function in CL_Surface (create the image from
//- other surfaces).</p>
class CL_Canvas : public CL_SurfaceProvider
{
public:
//! Construction:
	//: Creates the surface.
	//- width - the width of the surface to be created
	//- height - the height of the surface to be created
	//- no_sprs - the number of sprites the surface will have
	//- red_mask - the bit mask for red
	//- blue_mask - the bit mask for blue
	//- green_mask - the bit mask for green
	//- alpha_mask - the bit mask for the alpha 
	//- use_transcol - whether the surface should use source key transparancy
	//- transcol - the (optional) source key color
	static CL_Surface *create(
		int width,
		int height,
		int no_sprs = 1,
		int red_mask   = 0xff000000,
		int green_mask = 0x00ff0000,
		int blue_mask  = 0x0000ff00,
		int alpha_mask = 0x000000ff,
		bool use_transcol = false,
		unsigned int transcol = 0)
	{
		return CL_Surface::create(
			new CL_Canvas(
				width,
				height,
				no_sprs,
				red_mask,
				green_mask, 
				blue_mask,
				alpha_mask,
				use_transcol,
				transcol),
			true);
	}

	//: Constructs the Canvas.
	//- width - the width of the surface to be created
	//- height - the height of the surface to be created
	//- no_sprs - the number of sprites the surface will have
	//- red_mask - the bit mask for red
	//- blue_mask - the bit mask for blue
	//- green_mask - the bit mask for green
	//- alpha_mask - the bit mask for the alpha 
	//- use_transcol - whether the surface should use source key transparancy
	//- transcol - the (optional) source key color
	CL_Canvas(
		int width,
		int height,
		int no_sprs = 1,
		int red_mask   = 0xff000000,
		int green_mask = 0x00ff0000,
		int blue_mask  = 0x0000ff00, 
		int alpha_mask = 0x000000ff,
		bool use_transcol = false,
		unsigned int transcol = 0)
	{
		// has to be blitted by the dynamic blitter at the moment
		this->width = width;
		this->height = height;
		this->no_sprs = no_sprs;
		this->transcol = transcol;
		this->red_mask = red_mask;
		this->green_mask = green_mask;
		this->blue_mask = blue_mask;
		this->alpha_mask = alpha_mask;
		this->use_transcol = use_transcol;
		
		bpp = get_depth();
		
		data = new unsigned char[width * height * no_sprs * get_bytes_per_pixel()]; 
	}  

	//: Deletes the DynamicProvider and deletes the data
	//: which was allocated in the constructor
	virtual ~CL_Canvas() 
	{ 
		delete[] data; 
	}

//! Attributes:
	//: Returns the width of the surface
	virtual unsigned int get_width() const { return width; }

	//: Returns  the height of the surface
	virtual unsigned int get_height() const { return height; }

	//: Returns the number of frames (sprites) for this surface	
	virtual unsigned int get_num_frames() const { return no_sprs; }

	//: Returns the bit mask for red 
	virtual unsigned int get_red_mask() const { return red_mask; }

	//: Returns the bit mask for green
	virtual unsigned int get_green_mask() const { return green_mask; }

	//: Returns the bit mask for blue
	virtual unsigned int get_blue_mask() const { return blue_mask; }

	//: Returns the bit mask for the alpha 
	virtual unsigned int get_alpha_mask() const { return alpha_mask; }

	//: returns the pitch of the surface
	virtual unsigned int get_pitch() const { return ((bpp+7)/8)*get_width(); }

//! Operations:
	//: <p>returns true if the surface uses indexed colors
	//: which is currently not supported (it always
	//: returns false)</p>
	virtual bool is_indexed() const { return false; }

	//: CL_Canvas doesn't work with PAL8
	virtual void set_palette(CL_Palette*) { ; }

	//: CL_Canvas doesn't work with PAL8
	//- Returns - NULL
	virtual CL_Palette *get_palette() const { return NULL; }

	//: Sets the transparent color
	virtual void set_src_colorkey(unsigned int transcol) { this->transcol = transcol; }

	//: Uses Src Colourkey
	virtual bool uses_src_colorkey() const { return use_transcol; }

	//: Returns the transparent color, which was set in the constructor
	virtual unsigned int get_src_colorkey() const { return transcol; }

	//: <p>Returns the pointer to the surface data, where you
	//: can copy your data to. It should be
	//: only called between lock() and unlock(). </p>
	virtual void *get_data() const { cl_assert(data!=NULL); return data; }

	//: <p>Locks the surface so that writing to the surface
	//: may occur.</p>
	virtual void lock() { }

	//: Unlocks the surface.
	//: ( Call Unlock when all writing to the surface is done )
	virtual void unlock() { }

private:
	int width, height, no_sprs, transcol;
	int red_mask, green_mask, blue_mask, alpha_mask;
	EPixelFormat pixelformat;
	unsigned char *data;
	int bpp;
	bool use_transcol;
};

#endif
