/*
	$Id: target.h,v 1.8 2001/12/14 02:39:57 plasmoid Exp $

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

#ifndef header_target
#define header_target

#include <stack>
#include "cliprect.h"
#include "pixelformat.h"

class CL_Palette;

//: Target class in ClanLib.
//- <p>The CL_Target class represents a renderer target in ClanLib. This can
//- be a part of the framebuffer, or a piece of memory specified by the
//- application.</p>
//-
//- <p>This class contains a set of functions used to describe a piece of
//- memory, things such as the width, height, color masks and depth. This
//- information can then be used by ClanLib or the application to access the
//- given piece of memory.</p>
//-
//- <p>To access the memory itself, call get_data() after a call to lock().
//- When you are finished accessing the memory, call unlock(). Note that
//- after the unlock() call, the pointer obtained from get_data() will be
//- invalid, and new call to lock() is required before get_data() will return
//- anything valid.</p>
//-
//- <p>CL_Target contain a set of functions doing simple primitive
//- operations: draw boxes, lines and clipping. These are provided for
//- convience so you won't have to do such simple things manually in a lock
//- session.</p>
//-
//- <p>CL_Surface supports blitting to a target and all surface providers are
//- inheritated from a target. This means that you can do surface to provider
//- blitting. ClanLib has a special surface provider called CL_Canvas which
//- you should use to easilly build a new image.</p>
class CL_Target
{
public:
//! Construction:
	//: Target Constructor
	CL_Target() { m_translation_stack.push(TranslationOffset(0,0)); }

	//: Target destructor
	virtual ~CL_Target() { while(!m_translation_stack.empty()) m_translation_stack.pop(); return; }

//! Attributes:
	//: Returns true if in video memory.
	//- Returns - True if stored in video memory, false otherwise.
	virtual bool is_video() const { return false; }

	//: <p>Locks the target. This allows access to the target data using the
	//: get_data() function call.</p>
	virtual void lock()=0;

	//: <p>Unlock the target. Must be called after you're finished with modifying
	//: the target data.</p>
	virtual void unlock()=0;

	//: <p>Returns a pointer to the target data. Only valid between lock/unlock
	//: calls.</p>
	//- Returns - A pointer that points to the beginning of the target pixel data.
	virtual void *get_data() const=0;
	
	//: Returns the number of frames available on this target.
	virtual unsigned int get_num_frames() const=0;

	//: Returns the width of the target.
	virtual unsigned int get_width() const=0;

	//: Returns the height of the target.
	virtual unsigned int get_height() const=0;

	//: Returns the pitch (bytes per line) used by the target.
	virtual unsigned int get_pitch() const=0;

	//: Returns the depth used by the target.
	virtual unsigned int get_depth() const
	{
		unsigned int mask =
			get_red_mask() |
			get_green_mask() |
			get_blue_mask() |
			get_alpha_mask();

		int bits = 0;
		while (mask!=0)
		{
			mask = (mask >> 1);
			bits++;
		}
		bits = (bits+7)/8;
		return bits*8;
	}

	//: Returns the bytes per pixel of the target.
	virtual unsigned int get_bytes_per_pixel() const { return (get_depth()+7)/8; }

	//: Returns whether the target uses an indexed color mode or not.
	virtual bool is_indexed() const=0;
	
	//: Returns the red color mask used by the target.
	virtual unsigned int get_red_mask() const=0;

	//: Returns the green color mask by the target.
	virtual unsigned int get_green_mask() const=0;

	//: Returns the blue color mask by the target.
	virtual unsigned int get_blue_mask() const=0;

	//: Returns the alpha mask by the target.
	virtual unsigned int get_alpha_mask() const=0;

	//: Returns the palette used by the target.
	virtual CL_Palette *get_palette() const=0;
	
	//: Pushes the current clipping rectangle onto the cliprect stack.
	virtual void push_clip_rect();

	//: <p>Pushes the current clipping rectangle onto the cliprect stack. It then clips 
	//: the passed rectangle 'rect' with the current one, and uses the result as the 
	//: new clipping rect.</p>
	//- rect - The new clipping rectangle to be clipped with the current cliprect and then used.
	virtual void push_clip_rect(const CL_ClipRect &rect);

	//: Returns the current clipping rectangle.
	//- Returns - The current clipping rectangle.
	virtual CL_ClipRect get_clip_rect();

	//: Sets the current clipping rectangle. This is an absolute set, so it doesn't
	//: get clipped with the previous one.
	virtual void set_clip_rect(const CL_ClipRect &rect);

	//: Pop the clipping rectangle last pushed onto the stack.
	virtual void pop_clip_rect();

	//: Pushes the current translation rectangle onto the cliprect stack.
	virtual void push_translate_offset();

	//: <p>Push translation offset onto translation stack. This offset will
	//: affect any subsequent display operations on the displaycard, by
	//: translating the position of the display operation with the offset.
	//: The offset will be offset by any previous offsets pushed onto the stack,
	//: eg. it inherits the previous offset.</p>
	virtual void push_translate_offset(int x, int y);

	//: Returns the current effective x-axis translation offset.
	virtual int  get_translate_offset_x() const;

	//: Returns the current effective y-axis translation offset.
	virtual int  get_translate_offset_y() const;

	//: <p>Sets the translation offset as a new absolute translation offset.
	//: The new offset will disregard any previous offset's, but will not
	//: empty the translation stack. The new translation offset will affect
	//: any subsequent display operations on the displaycard, by
	//: translating the position of the display operation with the offset</p>
	virtual void set_translate_offset(int x, int y);

	//: <p>Pops the last pushed translation offset from the translation offset
	//: stack. If the stack is empty, nothing will happen, and if the last
	//: translation offset is popped, the translation offset will be set to 0,0</p>
	virtual void pop_translate_offset();

//! Operations:
	//: Draw Pixel
	void draw_pixel(int x1, int y1, int color);

	//: Draw Pixel
	virtual void draw_pixel(int x, int y, float r, float g, float b, float a = 1.0)
	{
		draw_pixel(x, y, CL_Color::get_color(this, r,g,b,a));
	}

	//: Get Pixel
	virtual void get_pixel(int x, int y, float *r, float *g, float *b, float *a);

	//: Get Pixel
	virtual int get_pixel(int x, int y);

	//: Flips along the Y-axis
	void flip_vertical();

	//: Flips along the X-axis
	void flip_horizontal();

	//: <p>Draw a filled rectangle from ('x1', 'y1') to ('x2', 'y2') using the color
	//: ('r', 'g', 'b', 'a').</p>
	//- x1 - Leftmost x-coordinate.
	//- y1 - Upper y-coordinate.
	//- x2 - Rightmost x-coordinate.
	//- y2 - Lower y-coordinate.
	//- r - Red component of the filled color.
	//- g - Green component of the filled color.
	//- b - Blue component of the filled color.
	//- a - Alpha component of the filled color.
	virtual void fill_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a = 1.0);

	//: <p>Draw a rectangle from ('x1', 'y1') to ('x2', 'y2') using the color
	//: ('r', 'g', 'b', 'a').</p>
	//- x1 - Leftmost x-coordinate.
	//- y1 - Upper y-coordinate.
	//- x2 - Rightmost x-coordinate.
	//- y2 - Lower y-coordinate.
	//- r - Red component of the filled color.
	//- g - Green component of the filled color.
	//- b - Blue component of the filled color.
	//- a - Alpha component of the filled color.
	virtual void draw_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a = 1.0);

	//: <p>Draw a line from ('x1', 'y1') to ('x2', 'y2') using the color
	//: ('r', 'g', 'b', 'a').</p>
	//- x1 - Leftmost x-coordinate. //FIXME
	//- y1 - Upper y-coordinate.
	//- x2 - Rightmost x-coordinate.
	//- y2 - Lower y-coordinate.
	//- r - Red component of the filled color.
	//- g - Green component of the filled color.
	//- b - Blue component of the filled color.
	//- a - Alpha component of the filled color.
	virtual void draw_line(int x1, int y1, int x2, int y2, float r, float g, float b, float a = 1.0);

private:
	std::stack<CL_ClipRect> clip_stack;
	typedef std::pair<int,int> TranslationOffset;
	std::stack<TranslationOffset> m_translation_stack;
};

#endif
