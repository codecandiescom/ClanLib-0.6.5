/*
	$Id: target_ggi.h,v 1.2 2001/03/17 23:53:24 vaidhy Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_target_ggi
#define header_target_ggi

#ifdef USE_GGI

#include <ggi/ggi.h>

#include "API/Display/Display/target.h"
#include "API/Display/Display/palette.h"

class CL_Target_GGI : public CL_Target
{
public:
	CL_Target_GGI(ggi_visual_t vis);
		
	virtual ~CL_Target_GGI();
/*
	virtual void lock()=0;
	virtual void unlock()=0;
*/
//	virtual void *get_data()=0;
	virtual void to_back()=0;
	virtual void to_front()=0;
/*
	virtual unsigned int get_width()=0;
	virtual unsigned int get_height()=0;
	virtual unsigned int get_pitch()=0;
	
	virtual unsigned int get_depth()=0;
	virtual unsigned int get_red_mask()=0;
	virtual unsigned int get_green_mask()=0;
	virtual unsigned int get_blue_mask()=0;
	virtual unsigned int get_alpha_mask()=0;

	virtual CL_Palette *get_palette()=0;
*/	
protected:
	ggi_visual_t		m_vis;
        ggi_mode                m_mode;
        
        CL_Palette		m_palette;
};

#endif /*USE_GGI*/

#endif
