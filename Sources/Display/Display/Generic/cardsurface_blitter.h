/*
	$Id: cardsurface_blitter.h,v 1.3 2001/12/14 02:39:58 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_cardsurface_blitter
#define header_cardsurface_blitter

#include "cardsurface.h"
#include "blitters.h"
#include <Display/Display/Generic/blit_opaque.h>

class CL_DisplayCard_Generic;

class CL_CardSurface_Blitter : public CL_CardSurface
{
public:
	CL_CardSurface_Blitter(
		CL_DisplayCard_Generic *target,
		CL_SurfaceProvider *provider,
		bool dynamic = false);
	virtual ~CL_CardSurface_Blitter();

	virtual bool reload();

	virtual bool can_convert_video();
	virtual bool can_convert_system();

	virtual bool convert_video();
	virtual bool convert_system();

	virtual void put_screen(
		int x,
		int y,
		int spr_no,
		CL_Target *target);
		
	virtual void put_screen(
		int x,
		int y,
		float scale_x,
		float scale_y,
		int spr_no,
		CL_Target *target);

	virtual void put_screen(
		int x,
		int y,
		int size_x,
		int size_y,
		int spr_no,
		CL_Target *target);
	
	virtual bool is_video();

protected:
	virtual CL_Target *get_target();
	virtual CL_ClipRect get_clip_rect();

	int get_width() { return m_width; }
	int get_height() { return m_height; }
	int get_no_sprs() { return m_no_sprs; }
	
	bool is_dynamic() { return m_dynamic; }

private:
	void create_sw_blitters();

	CL_Blitters m_blitters;
	CL_DisplayCard_Generic *m_target;

//	CL_Blit_Opaque *blt_dyn;
	
	int m_width;
	int m_height;
	int m_no_sprs;
	bool m_dynamic;
};

#endif
