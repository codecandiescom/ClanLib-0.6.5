/*
	$Id: target_system.h,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_target_system
#define header_target_system

#include "API/Core/Display/target.h"

class CL_Target_System : public CL_Target
{
public:
	CL_Target_System(
		int width,
		int height,
		int bytes_per_pixel,
		int red_mask,
		int green_mask,
		int blue_mask,
		int alpha_mask)
	{
		m_width = width;
		m_height = height;
		m_bytes_per_pixel = bytes_per_pixel;
		m_red_mask = red_mask;
		m_green_mask = green_mask;
		m_blue_mask = blue_mask;
		m_alpha_mask = alpha_mask;
		
		m_buffer = new unsigned char[
			width*height*bytes_per_pixel];
	}
	
	virtual ~CL_Target_System()
	{
		delete[] m_buffer;
	}

	virtual void lock() { return; }
	virtual void unlock() { return; }

	virtual void *get_data() const { return m_buffer; }

	virtual unsigned int get_width() const { return m_width; }
	virtual unsigned int get_height() const { return m_height; }
	virtual unsigned int get_pitch() const { return m_width*m_bytes_per_pixel; }

	virtual unsigned int get_depth() const { return m_bytes_per_pixel*8; }
	virtual unsigned int get_red_mask() const { return m_red_mask; }
	virtual unsigned int get_green_mask() const { return m_green_mask; }
	virtual unsigned int get_blue_mask() const { return m_blue_mask; }
	virtual unsigned int get_alpha_mask() const { return m_alpha_mask; }
	virtual unsigned int get_num_frames() const { return 1; }
	virtual bool is_indexed() const { return false; }

	virtual CL_Palette *get_palette() const { return NULL; }
	
private:
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_bytes_per_pixel;
	unsigned int m_red_mask;
	unsigned int m_green_mask;
	unsigned int m_blue_mask;
	unsigned int m_alpha_mask;
	unsigned char *m_buffer;
};

#endif
