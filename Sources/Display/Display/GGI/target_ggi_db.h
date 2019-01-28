/*
	$Id: target_ggi_db.h,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_target_ggi_db
#define header_target_ggi_db

#ifdef USE_GGI

#include <ggi/ggi.h>

#include "target_ggi.h"

class CL_Target_GGI_DB : public CL_Target_GGI
{
public:
	CL_Target_GGI_DB(ggi_visual_t vis, int frame);
		
	virtual ~CL_Target_GGI_DB();

	virtual void lock();
	virtual void unlock();

	virtual void *get_data() const;
	virtual void to_back();
	virtual void to_front();

	virtual unsigned int get_width() const;
	virtual unsigned int get_height() const;
	virtual unsigned int get_pitch() const;
	
	virtual unsigned int get_depth() const;
	virtual unsigned int get_red_mask() const;
	virtual unsigned int get_green_mask() const;
	virtual unsigned int get_blue_mask() const;
	virtual unsigned int get_alpha_mask() const;

	virtual bool is_indexed() const { return false; }

	virtual CL_Palette *get_palette() const;
	virtual unsigned int get_num_frames() const { return 1; }
	
	virtual bool is_video() { return true; }

protected:
	const ggi_directbuffer*	m_db;
	int			m_frame;
	void*			m_fb;
};

#endif /*USE_GGI*/

#endif
