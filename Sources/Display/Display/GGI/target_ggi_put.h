/*
	$Id: target_ggi_put.h,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_target_ggi_put
#define header_target_ggi_put

#ifdef USE_GGI

#include <ggi/ggi.h>

#include "target_ggi.h"

class CL_Target_GGI_Put : public CL_Target_GGI
{
public:
	CL_Target_GGI_Put(ggi_visual_t vis);
		
	virtual ~CL_Target_GGI_Put();

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
	virtual unsigned int get_num_frames() const { return 1; }

	virtual bool is_indexed() const { return false; }
	virtual CL_Palette *get_palette() const;
	
	virtual bool is_video() { return false; }
        
protected:
	int			image_size;
	void*			image_data;
	const ggi_pixelformat*	m_pixelformat;
};

#endif /*USE_GGI*/

#endif
