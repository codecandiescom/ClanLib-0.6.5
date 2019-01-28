/*
	$Id: display.h,v 1.9 2002/01/09 10:49:00 grumbel Exp $

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

#ifndef header_display
#define header_display

#include <vector>
class CL_Palette;
class CL_DisplayCard;
class CL_VidMode;
class CL_Target;
class CL_Surface;
class CL_Rect;

#include "cliprect.h"
#include "../../signals.h"

//: Main Display class.
//- <p>CL_Display is the "main" class when dealing with 2D graphics. It
//- contains simple drawing operations, backbuffer clipping and access to the
//- display cards available on the system.</p>
//-
//- <p>A large amount of the functions in the class have the same purpose as
//- their name equalient in CL_DisplayCard. The difference is that those
//- placed here operate on a selected display card, while those in
//- CL_DisplayCard class require an instance to the card.</p>
//-
//- <p>All backbuffer related drawing operations in other classes (CL_Surface
//- for instance) work on the currently selected display card, unless you
//- explict pass a pointer pointing to a display card.</p>
//-
//- <p>The main purpose of this system is to avoid passing around a pointer
//- to the display card in those games, where it only access one single
//- card anyway.</p>
//-
//- <p>When using OpenGL the GL context will always point to the selected
//- card. So if you need to use OpenGL commands to draw onto another card,
//- you have to select it here.</p>
class CL_Display
{
public:
//! Construction:
	// Display Destructor
	virtual ~CL_Display() { ; }

//! Operations (Display Functions):
	//: Flips the front and backbuffer.
	//: <p>Everything is normally drawn to the backbuffer, and
	//: flip_display() needs to be called before it can be seen 
	//: onto the screen.</p>
	//- sync - VSync on/off
	static void flip_display(bool sync=false);

	//: Copies the specified area of the backbuffer to the front buffer.
	//: <p>This is in particular useful if you only want to update a limited
	//: region, and not - do a full-blown flipping.</p>
	static void put_display(const CL_Rect &rect);

	//: <p>Copies the contents of the frontbuffer to all other buffers (usually just
	//: the backbuffer).</p>
	//: This ensures that all buffers contain the same image.
	static void sync_buffers();

	//: Clears backbuffer with the specified color.
	//- red - red component of the color.
	//- green - green component of the color.
	//- blue - blue component of the color.
	//- alpha - alpha (transparency) component of the color.
	static void clear_display(float red=0, float green=0, float blue=0, float alpha=1);

	//: Set system palette on this card.
	//: <p>If the display card is in a non-palettelized mode, this will be the palette 
	//: used when surfaceproviders doesn't specify a palette themself.</p>
	//- palette - Palette to use as new system palette.
	static void set_palette(CL_Palette *palette);

	//: Return the current system palette on this card.
	//- Returns - The system palette.
	static CL_Palette *get_palette();

	//: Selects the display card used by the other memberfunctions of this class.
	//- card - new selected display card.
	static void select_card(CL_DisplayCard *card);

	//: Change the display card's video mode.
	//- mode - videomode to be changed to.
	//- See also: CL_VidMode - Video mode description class.
	static void set_videomode(CL_VidMode *mode);

	//: Change the display card's video mode.
	//- width - width in pixels of the new video mode.
	//- height - height in pixels of the new video mode.
	//- bpp - Bits per pixel. The depth of the new video mode. (8, 16, 24, 32)
	//- video_memory - Use video memory if possible. System memory may be faster if alpha blending is used a lot.
	static void set_videomode(
		int width,
		int height,
		int bpp,
		bool fullscreen = false,
		bool allow_resize = false,
		bool video_memory = true);

//! Attributes:
	//: Returns the currently selected display card.
	//- Returns - Current display card.
	static CL_DisplayCard *get_current_card();

	//: Returns either NULL or the framebuffer
	//- Returns - NULL or the framebuffer
	static CL_Target *get_target();

	//: Returns the width of the current video mode.
	//- Returns - Width of current video mode.
	static int get_width();

	//: Returns the height of the current video mode.
	//- Returns - Height of current video mode.
	static int get_height();

	//: Returns the depth of the current video mode.
	//- Returns - Depth of current video mode.
	static int get_bpp();

	//: The list of display cards available to the application.
	static std::vector<CL_DisplayCard*> cards;
	
	//: Pushes the current clipping rectangle onto the cliprect stack.
	static void push_clip_rect();

	//: <p>Pushes the current clipping rectangle onto the cliprect stack. It then clips 
	//: the passed rectangle 'rect' with the current one, and uses the result as the 
	//: new clipping rect.</p>
	//- rect - The new clipping rectangle to be with the old one and then used.
	static void push_clip_rect(const CL_ClipRect &rect);

	//: Returns the current clipping rectangle.
	//- Returns - The current clipping rectangle.
	static CL_ClipRect get_clip_rect();

	//: Sets the current clipping rectangle.
	//: This is an absolute set, so it doesn't get clipped with the previous one.
	static void set_clip_rect(const CL_ClipRect &rect);

	//: Pop the clipping rectangle last pushed onto the stack.
	static void pop_clip_rect();

	//: Pushes the current translation rectangle onto the cliprect stack.
	static void push_translate_offset();

	//: <p>Push translation offset onto translation stack. This offset will
	//: affect any subsequent display operations on the current displaycard, by
	//: translating the position of the display operation with the offset.
	//: The offset will be offset by any previous offsets pushed onto the stack,
	//: eg. it inherits the previous offset.</p>
	static void push_translate_offset(int x, int y);

	//: Returns the current effective x-axis translation offset.
	static int  get_translate_offset_x();

	//: Returns the current effective y-axis translation offset.
	static int  get_translate_offset_y();

	//: <p>Sets the translation offset as a new absolute translation offset.
	//: The new offset will disregard any previous offset's, but will not
	//: empty the translation stack. The new translation offset will affect
	//: any subsequent display operations on the current displaycard, by
	//: translating the position of the display operation with the offset</p>
	static void set_translate_offset(int x, int y);

	//: <p>Pops the last pushed translation offset from the translation offset
	//: stack. If the stack is empty, nothing will happen, and if the last
	//: translation offset is popped, the translation offset will be set to 0,0</p>
	static void pop_translate_offset();

//! Operations (Draw functions):
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
	static void draw_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a=1.0f);

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
	static void fill_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a=1.0f);

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
	static void draw_line(int x1, int y1, int x2, int y2, float r, float g, float b, float a=1.0f);
	
	//: Draw a filled rectangle from ('x1', 'y1') to ('x2', 'y2') using the surface 'fill_surface' as tiled background
	//- x1 - Leftmost x-coordinate.
	//- y1 - Upper y-coordinate.
	//- x2 - Rightmost x-coordinate.
	//- y2 - Lower y-coordinate.
	//- fill_surface - surface used to fill the area (tiled)
	//- focus_x - destination x offset used to offset (0, 0) in fill_surface (controls tiling position)
	//- focus_y - destination y offset used to offset (0, 0) in fill_surface (controls tiling position)
	static void fill_rect(int x1, int y1, int x2, int y2, CL_Surface *fill_surface, int focus_x=0, int focus_y=0);

//! Signals:
	//: Returns the resize signal for the currently selected display card.
	//: You can use this signal to listen for window resize events.
	//: The parameters passed by the signal are the new width and height of the window.
	static CL_Signal_v2<int, int> &sig_resized();

	//: Returns the paint signal for the currently selected display card.
	//: Use this signal to listen for invalidated screen areas that need to be repainted.
	//: The parameter passed by the signal is the area that need a repaint.
	static CL_Signal_v1<const CL_Rect &> &sig_paint();
};

#endif
