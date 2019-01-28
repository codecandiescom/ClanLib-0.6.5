/*
	$Id: target_fbdev.h,v 1.2 2001/03/10 17:26:28 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_target_fbdev
#define header_target_fbdev

#ifdef USE_FBDEV

#include <linux/fb.h>

#include "API/Display/Display/target.h"

class CL_Target_FBDev : public CL_Target
{
public:
	CL_Target_FBDev( bool doublebuffer, char *fb, unsigned int width, unsigned int height, fb_var_screeninfo *var, volatile unsigned char *mmioaddr );
	virtual ~CL_Target_FBDev();

	virtual void lock();
	virtual void unlock();

	virtual void *get_data() const;

	virtual unsigned int get_width() const;
	virtual unsigned int get_height() const;
	virtual unsigned int get_pitch() const;
	
	virtual bool is_indexed() const;
	virtual unsigned int get_depth() const;
	virtual unsigned int get_red_mask() const;
	virtual unsigned int get_green_mask() const;
	virtual unsigned int get_blue_mask() const;
	virtual unsigned int get_alpha_mask() const;
	virtual unsigned int get_num_frames() const { return 1; }

	virtual CL_Palette *get_palette() const;
	
	virtual void put_image();
	
	virtual bool is_video();
        
protected:
	volatile unsigned char *m_mmioaddr;
	fb_var_screeninfo* m_var;
	bool m_doublebuffer;
	int buffer;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_skip;
	char* m_fb;
	char* image_data;
	unsigned int image_size;
};

#endif /*USE_FBDEV*/

#endif
