/*
	$Id: displaycard_svgalib.h,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_displaycard_svgalib
#define header_displaycard_svgalib

#include "Display/Display/Generic/displaycard_generic.h"
#include "Display/Display/Generic/target_system.h"

class CL_DisplayCard_Svgalib : public CL_DisplayCard_Generic
{
public:
	CL_DisplayCard_Svgalib(int card_no);
	virtual ~CL_DisplayCard_Svgalib();

	// CL_DisplayCard functions:
	// -------------------------

	virtual void flip_display(bool sync=false);
	virtual void put_display(const class CL_Rect &rect);

	virtual void set_palette(CL_Palette *palette);
	virtual CL_Palette *get_palette();

	virtual void set_videomode(int width, int height, int bpp, bool fullscreen, bool allow_resize, bool video_memory);
	virtual bool is_initialized();

	virtual std::string get_name() { return "Svgalib Display"; }
	virtual int get_total_memory() { return -1; }
	virtual const std::list<CL_VidMode*> &get_videomodes();

	// CL_DisplayCard_Generic functions:
	// ---------------------------------
	
	virtual CL_Target *get_target() { return m_target; }
	virtual CL_Target *get_frontbuffer() { return m_target; }

private:
	bool m_initialized;
	CL_Palette *m_palette;
	CL_Target_System *m_target;
};

#endif
