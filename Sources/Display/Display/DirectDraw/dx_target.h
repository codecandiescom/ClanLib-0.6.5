/*
	$Id: dx_target.h,v 1.2 2001/09/02 17:33:37 starch Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_dx_target
#define header_dx_target

#include <ddraw.h>
#include "API/Display/Display/palette.h"
#include "API/Display/Display/target.h"

class CL_Target_DX : public CL_Target
{
public:
	CL_Target_DX(LPDIRECTDRAWSURFACE surface);
	virtual ~CL_Target_DX();

	virtual void lock();
	virtual void unlock();

	virtual bool is_video() const { return m_surface_is_video; }
	virtual void *get_data() const { return m_data; }

	virtual unsigned int get_num_frames() const { return 1; }
	virtual unsigned int get_width() const { return m_width; }
	virtual unsigned int get_height() const { return m_height; }
	virtual unsigned int get_pitch() const { return m_pitch; }
	
	virtual unsigned int get_depth() const { return m_depth; }
	virtual unsigned int get_red_mask() const { return m_red_mask; }
	virtual unsigned int get_green_mask() const { return m_green_mask; }
	virtual unsigned int get_blue_mask() const { return m_blue_mask; }
	virtual unsigned int get_alpha_mask() const { return m_alpha_mask; }

	virtual CL_Palette *get_palette() const;
	virtual void set_palette(CL_Palette *pal);

	virtual bool is_indexed() const { return get_depth()==8; }

	LPDIRECTDRAWSURFACE get_surface() { return m_surface; }

protected:
	void set_surface(LPDIRECTDRAWSURFACE surface);
	void get_palette_from_surface();

	LPDIRECTDRAWSURFACE m_surface;
	bool m_surface_is_video;
	void *m_data;
	unsigned int m_width, m_height, m_pitch, m_depth;
	unsigned int m_red_mask, m_green_mask, m_blue_mask, m_alpha_mask;
	CL_Palette m_palette;
	unsigned int m_ref_count;
};

#endif
