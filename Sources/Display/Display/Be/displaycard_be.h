/*
	$Id: displaycard_be.h,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_displaycard_be
#define header_displaycard_be

#include "Core/Display/Generic/displaycard_generic.h"
#include "target_be.h"

class ClanWindowScreen;

class CL_DisplayCard_Be : public CL_DisplayCard_Generic
{
public:
	CL_DisplayCard_Be(int card_no);
	virtual ~CL_DisplayCard_Be();
	
	// CL_DisplayCard functions:
	// -------------------------

	virtual void flip_display(bool sync=false);
	virtual void put_display(const class CL_Rect &rect);
	virtual void set_palette(CL_Palette *palette);
	virtual CL_Palette *get_palette();
	virtual void set_videomode(int width, int height, int bpp, bool fullscreen, bool allow_resize, bool video_memory);
	virtual bool is_initialized();
        
	virtual std::string get_name() { return "BeOS Display"; }
	virtual int get_total_memory() { return -1; }
	virtual const std::list<CL_VidMode *> &get_videomodes();

	// CL_DisplayCard_Generic functions:
	// ---------------------------------

	virtual CL_Target *get_target() { return m_target; }
	virtual CL_Target *get_frontbuffer() { return m_target; }

	virtual void fill_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a);
	virtual void draw_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a);
	virtual void clear_display(float red=0, float green=0, float blue=0, float alpha=1);

private:
	CL_Palette *m_palette;
	CL_Target_Be *m_target;
	ClanWindowScreen *clws;
};

#endif
