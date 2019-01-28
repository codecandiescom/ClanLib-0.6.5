/*
	$Id: display_quartz.h,v 1.2 2001/12/20 01:11:35 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_display_quartz
#define header_display_quartz


#include "Display/Display/Generic/displaycard_generic.h"
#include "target_quartz.h"

#include <list>

class CL_DisplayCard_Quartz : public CL_DisplayCard_Generic
{
public:
	CL_DisplayCard_Quartz();
	virtual ~CL_DisplayCard_Quartz();

	virtual void flip_display(bool sync=false);
	virtual void put_display(const class CL_Rect &rect);
	virtual void clear_display(float red=0, float green=0, float blue=0, float alpha=1);
	virtual void sync_buffers();

	virtual void set_palette(CL_Palette *palette);
	virtual CL_Palette *get_palette();

	virtual void set_videomode(int width, int height,
		int bpp,
		bool fullscreen = true,
		bool allow_resize = false,
		bool video_memory = true);

	virtual const std::list<CL_VidMode*> &get_videomodes();
	virtual std::string get_name();
	virtual int get_total_memory();
	virtual int get_card_no();
	virtual bool is_initialized();
	CL_Target* get_target();

	virtual CL_Target *get_frontbuffer();

	class CL_FlipDisplayCallback_Generic
	{
	public:
		virtual void pre_flip();
		virtual void post_flip();
	};

protected:
	void create_mode_list();

private:
	std::list<CL_VideoMode*> mode_list;

	bool inited;

	CFDictionaryRef current_mode;
	CFDictionaryRef prime_mode;
};

#endif
