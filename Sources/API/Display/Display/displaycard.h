/*
	$Id: displaycard.h,v 1.8 2001/09/22 15:52:11 plasmoid Exp $

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

#ifndef header_displaycard
#define header_displaycard

#include <list>
#include <string>

class CL_VidMode;
class CL_Palette;
class CL_Target;
class CL_Rect;

#include "cliprect.h"
#include "../../signals.h"

class CL_InputSourceProvider;
//: The display card class.
//- <p>This class represents a display card in ClanLib. It is mainly used to
//- set the videomode, flip the display and do simple graphics operations
//- such as filled rectangles or drawing lines.</p>
//-
//- <p>If you're always only using one display card at the time, you may want
//- to consider using the static functions in CL_Display instead. They do
//- exactly the same, but operates on a selected display card instead
//- (default the primary card). This saves you from passing around a pointer
//- to the displaycard.</p>
class CL_DisplayCard
{
public:
//! Construction:
	//: Display Card Destructor
	virtual ~CL_DisplayCard() { ; }

//! Operations (Display Functions):
	//: <p>Flips the front and backbuffer. Everything is normally drawn to the 
	//: backbuffer, and flip_display() needs to be called before it can be seen 
	//: onto the screen.</p>
	//- sync - VSync on/off
	virtual void flip_display(bool sync=false)=0;

	//: <p>Copies the specified area of the backbuffer to the front buffer. This is
	//: in particular useful if you only want to update a limited region, and not
	//: do a full-blown flipping.</p>
	virtual void put_display(const class CL_Rect &rect)=0;

	//: <p>Copies the contents of the frontbuffer to all other buffers (usually just
	//: the backbuffer). This ensures that all buffers contain the same image.</p>
	virtual void sync_buffers()=0;

	//: Clears backbuffer with the specified color.
	//- red - red component of the color.
	//- green - green component of the color.
	//- blue - blue component of the color.
	//- alpha - alpha (transparency) component of the color.
	virtual void clear_display(float red=0, float green=0, float blue=0, float alpha=1)=0;

	//: Set system palette on this card.
	//: 
	//: <p>If the display card is in a non-palettelized mode, this will be the palette 
	//: used when surfaceproviders doesn't specify a palette themself.</p>
	//- palette - Palette to use as new system palette.
	virtual void set_palette(CL_Palette *palette)=0;
	
	//: Return the current system palette on this card.
	//- Returns - The system palette.
	virtual CL_Palette *get_palette()=0;

	//: Change the display card's video mode.
	//- mode - videomode to be changed to.
	virtual void set_videomode(CL_VidMode *mode);

	//: Change the display card's video mode.
	//- width - width in pixels of the new video mode.
	//- height - height in pixels of the new video mode.
	//- bpp - Bits per pixel. The depth of the new video mode. (8, 16, 24, 32)
	//- video_memory - Use video memory if possible. System memory may be faster if alpha blending is used a lot.
	virtual void set_videomode(
		int width,
		int height,
		int bpp,
		bool fullscreen = true,
		bool allow_resize = false,
		bool video_memory = true)=0;

//! Attributes:
	//: Get list of videomodes available on this card.
	//- Returns - Returns the list of videomodes.
	virtual const std::list<CL_VidMode*> &get_videomodes()=0;

	//: Get name of the card.
	//- Returns - Returns the card name.
	virtual std::string get_name()=0;
	
	//: Get videomemory available.
	//- Returns - Videomemory available, or -1 if unknown.
	virtual int get_total_memory()=0;
	
	//: Get card number.
	//- Returns - Card number.
	virtual int get_card_no()=0;

	//: Returns true if the display card is in a video mode.
	//- Returns - true if initialized.
	virtual bool is_initialized()=0;

	//: Return either NULL or the Target that is blitted to (aka Framebuffer)
	//- Returns - NULL if Framebuffer access is not possible, CL_Target if it is
	virtual CL_Target* get_target()=0;

	//: Returns the width of the current video mode.
	//- Returns - Width of video mode.
	virtual int get_width()=0;

	//: Returns the height of the current video mode.
	//- Returns - Height of video mode.
	virtual int get_height()=0;

	//: Returns the depth of the current video mode.
	//- Returns - Depth of video mode.
	virtual int get_bpp()=0;

	//: Pushes the current clipping rectangle onto the cliprect stack.
	virtual void push_clip_rect()=0;

	//: <p>Pushes the current clipping rectangle onto the cliprect stack. It then clips 
	//: the passed rectangle 'rect' with the current one, and uses the result as the 
	//: new clipping rect.</p>
	//- rect - The new clipping rectangle to be with the old one and then used.
	virtual void push_clip_rect(const CL_ClipRect &rect)=0;
 
	//: Returns the current clipping rectangle.
	//- Returns - The current clipping rectangle.
	virtual CL_ClipRect get_clip_rect()=0;
 
	//: <p>Sets the current clipping rectangle. This is an absolute set, so it doesn't
	//: get clipped with the previous one.</p>
	virtual void set_clip_rect(const CL_ClipRect &rect)=0;

	//: Pop the clipping rectangle last pushed onto the stack.
	virtual void pop_clip_rect()=0;

	//: Pushes the current translation rectangle onto the cliprect stack.
	virtual void push_translate_offset()=0;

	//: <p>Push translation offset onto translation stack. This offset will
	//: affect any subsequent display operations on the displaycard, by
	//: translating the position of the display operation with the offset.
	//: The offset will be offset by any previous offsets pushed onto the stack,
	//: eg. it inherits the previous offset.</p>
	virtual void push_translate_offset(int x, int y)=0;

	//: Returns the current effective x-axis translation offset.
	virtual int  get_translate_offset_x() =0;

	//: Returns the current effective y-axis translation offset.
	virtual int  get_translate_offset_y() =0;

	//: <p>Sets the translation offset as a new absolute translation offset.
	//: The new offset will disregard any previous offset's, but will not
	//: empty the translation stack. The new translation offset will affect
	//: any subsequent display operations on the displaycard, by
	//: translating the position of the display operation with the offset</p>
	virtual void set_translate_offset(int x, int y)=0;

	//: <p>Pops the last pushed translation offset from the translation offset
	//: stack. If the stack is empty, nothing will happen, and if the last
	//: translation offset is popped, the translation offset will be set to 0,0</p>
	virtual void pop_translate_offset()=0;

//! Operations (Draw Functions):
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
	virtual void fill_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a)=0;

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
	virtual void draw_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a)=0;

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
	virtual void draw_line(int x1, int y1, int x2, int y2, float r, float g, float b, float a)=0;
	
//! Signals:
	//: Returns the resize signal for the display card.
	//: You can use this signal to listen for window resize events.
	//: The parameters passed by the signal are the new width and height of the window.
	virtual CL_Signal_v2<int, int> &sig_resized()=0;

	//: Returns the paint signal for the display card.
	//: Use this signal to listen for invalidated screen areas that need to be repainted.
	//: The parameter passed by the signal is the area that need a repaint.
	virtual CL_Signal_v1<const CL_Rect &> &sig_paint()=0;
};

#endif
