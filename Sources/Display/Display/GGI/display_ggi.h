/*
	$Id: display_ggi.h,v 1.2 2001/03/17 23:53:24 vaidhy Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_display_ggi
#define header_display_ggi

#ifdef USE_GGI

#include "Display/Display/Generic/displaycard_generic.h"

#include "target_ggi.h"

#include <ggi/ggi.h>

class CL_GGI_DisplayCard : public CL_DisplayCard_Generic
{
public:
	CL_GGI_DisplayCard(int card_no);
	virtual ~CL_GGI_DisplayCard();
	
	// CL_DisplayCard functions:
	// -------------------------

	virtual void flip_display(bool sync=false);
	virtual void put_display(const class CL_Rect &rect);
	virtual void set_palette(CL_Palette *palette);
	virtual CL_Palette *get_palette();
	virtual void set_videomode(int width, int height, int bpp, bool fullscreen, bool allow_resize, bool video_memory);
	virtual bool is_initialized();
        
	virtual std::string get_name() { return "GGI Display"; }
	virtual int get_total_memory() { return -1; }
	virtual const std::list<CL_VidMode*> &get_videomodes();

	virtual void clear_display(float red=0, float green=0, float blue=0, float alpha=1);
	virtual void fill_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a);
	virtual void draw_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a);
	virtual void draw_line(int x1, int y1, int x2, int y2, float r, float g, float b, float a);
	
	// CL_DisplayCard_Generic functions:
	// ---------------------------------

	virtual CL_Target *get_target() { return m_target; }
	virtual CL_Target *get_frontbuffer() { if (m_target2) return m_target2; else return m_target; }

	ggi_visual_t vis;
	ggi_mode mode;

protected:
#ifdef GGI_BLIT
	virtual CL_Blitters create_hw_blitters(CL_SurfaceProvider *provider);
#endif GGI_BLIT
		
private:
	bool m_initialized;
	CL_Palette *m_palette;
	CL_Target_GGI *m_target;
	CL_Target_GGI *m_target2;
};

#endif /* USE_GGI */

#endif
