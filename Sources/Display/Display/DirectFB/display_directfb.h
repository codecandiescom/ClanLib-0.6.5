/*
	$Id: display_directfb.h,v 1.2 2002/02/26 20:38:11 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_display_directfb
#define header_display_directfb

#ifdef USE_DIRECTFB

#include "Display/Display/Generic/displaycard_generic.h"

#include "target_directfb.h"

#include <directfb.h>

class CL_DirectFB_DisplayCard : public CL_DisplayCard_Generic
{
 public:
  CL_DirectFB_DisplayCard(int card_no);
  virtual ~CL_DirectFB_DisplayCard();
	
  // CL_DisplayCard_Generic functions:
  // ---------------------------------

  virtual void push_clip_rect();

  virtual void push_clip_rect (const CL_ClipRect &clip);

  virtual void set_clip_rect (const CL_ClipRect &clip);

  virtual void pop_clip_rect();

  virtual CL_Target *get_target() { return m_target; }
  virtual CL_Target *get_frontbuffer() { return m_target; }
	
  virtual int get_total_memory()
    {
      return -1;
    }

  virtual CL_Blitters create_hw_blitters(CL_SurfaceProvider *provider);

  // CL_DisplayCard functions:
  // -------------------------

  virtual void flip_display(bool sync=false);
  virtual void put_display(const class CL_Rect &rect);
  virtual void set_palette(CL_Palette *palette);
  virtual CL_Palette *get_palette();
  virtual void set_videomode(
			     int width,
			     int height,
			     int bpp,
			     bool full_screen,
			     bool allow_resize,
			     bool video_memory);
  virtual bool is_initialized();
        
  virtual std::string get_name() { return "DirectFB Display"; }
  virtual const std::list<CL_VidMode*> &get_videomodes();

  virtual void clear_display(float red=0, float green=0, float blue=0, float alpha=1);
  virtual void fill_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a);
  virtual void draw_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a);
  virtual void draw_line(int x1, int y1, int x2, int y2, float r, float g, float b, float a);

  virtual void sync_buffers();

private:
  IDirectFB *dfb;
  IDirectFBDisplayLayer *layer;
	
  bool m_initialized;
  CL_Palette *m_palette;
  CL_Target_DirectFB *m_target;
};

#endif /* USE_DIRECTFB */

#endif
