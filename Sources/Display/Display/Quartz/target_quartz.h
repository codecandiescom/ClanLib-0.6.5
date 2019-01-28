/*
	$Id: target_quartz.h,v 1.2 2001/12/20 01:11:35 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef target_quartz_header
#define target_quartz_header

class CL_Target_Quartz : public CL_Target
{
public:
	CL_Target_Quartz(CGDirectDisplayID context);
	virtual ~CL_Target_Quartz();

	virtual void lock();
	virtual void unlock();

	virtual bool is_video() const;
	virtual void *get_data() const;

	virtual unsigned int get_num_frames() const;
	virtual unsigned int get_width() const;
	virtual unsigned int get_height() const;
	virtual unsigned int get_pitch() const;
	
	virtual unsigned int get_depth() const;
	virtual unsigned int get_red_mask() const;
	virtual unsigned int get_green_mask() const;
	virtual unsigned int get_blue_mask() const;
	virtual unsigned int get_alpha_mask() const;

	virtual CL_Palette *get_palette() const;
	virtual void set_palette(CL_Palette *pal);

	virtual bool is_indexed() const;

private:
	unsigned int red_mask;
	unsigned int green_mask;
	unsigned int blue_mask;
	unsigned int alpha_mask;

	unsigned int bpp;
	unsigned int pitch;
	unsigned int height;
	unsigned int width;

	bool is_locked;
	signed int ref_count;

	CGDirectDisplayID display;
};

#endif

 