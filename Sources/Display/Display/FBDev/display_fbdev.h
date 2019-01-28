/*
	$Id: display_fbdev.h,v 1.2 2001/03/10 17:26:28 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_display_fbdev
#define header_display_fbdev

#ifdef USE_FBDEV

#include "Display/Display/Generic/displaycard_generic.h"

#include "target_fbdev.h"

#include <linux/fb.h>

class CL_FBDev_DisplayCard : public CL_DisplayCard_Generic
{
public:
	CL_FBDev_DisplayCard(int card_no);
	virtual ~CL_FBDev_DisplayCard();
	
	// CL_DisplayCard functions:
	// -------------------------

	virtual void flip_display(bool sync=false);
	virtual void put_display(const class CL_Rect &rect);
	virtual void set_palette(CL_Palette *palette);
	virtual CL_Palette *get_palette();
	virtual void set_videomode(int width, int height, int bpp, bool fullscreen, bool allow_resize, bool video_memory);
	virtual bool is_initialized();
        
	virtual std::string get_name() { return "FBDev Display"; }
	virtual const std::list<CL_VidMode*> &get_videomodes();

	// CL_DisplayCard_Generic functions:
	// ---------------------------------

	virtual void push_clip_rect() { m_clip_stack.push_front(m_cur_clip); }

	virtual void push_clip_rect(const CL_ClipRect &clip)
	{
		push_clip_rect();
		m_cur_clip = m_cur_clip.clip(clip);
		set_card_clip();
	}
	virtual void set_clip_rect(const CL_ClipRect &clip)
	{
		m_cur_clip = clip;
		set_card_clip();
	}
	virtual void pop_clip_rect()
	{
		m_cur_clip = m_clip_stack.front();
		m_clip_stack.pop_front();
		set_card_clip();
	}

	virtual CL_Target *get_target() { return m_target; }
	virtual CL_Target *get_frontbuffer() { return m_target; }
	
	virtual void fill_rect(int, int, int, int, float, float, float, float);
	virtual void draw_rect(int, int, int, int, float, float, float, float);

	virtual int get_total_memory() { return fb_fix.smem_len; }
	

	// own functions
	unsigned char* get_offscreen_mem();
	int yoffset;

	unsigned int get_virt_width();
	
	bool accel_alpha_ret() { return m_accel_alpha_rect; }
	bool accel_scale_blit() { return m_accel_scale_blit; }


protected:
        virtual CL_Blitters create_hw_blitters(CL_SurfaceProvider *provider);

private:
	void set_card_clip();

	volatile unsigned char *m_mmioaddr;
	bool doublebuffer;
	bool m_accel_alpha_rect;
	bool m_accel_scale_blit;
	fb_fix_screeninfo fb_fix;
	fb_var_screeninfo fb_var_orig;
	fb_var_screeninfo fb_var;
	unsigned char* fb_mem;
	int fd;

	
	bool m_initialized;
	CL_Palette *m_palette;
	CL_Target_FBDev *m_target;

	std::string fb_try_accel;
};

#endif /* USE_FBDEV */

#endif
