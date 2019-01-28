/*
	$Id: clanwindowscreen.h,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/


#ifndef header_clanwindowscreen
#define header_clanwindowscreen

#include <WindowScreen.h>
#include <SupportDefs.h>

typedef int32 (*rect8_hook)(int32,int32,int32,int32,uint8);
typedef int32 (*rect16_hook)(int32,int32,int32,int32,uint16);
typedef int32 (*rect32_hook)(int32,int32,int32,int32,uint32);
typedef int32 (*sync_hook)();

class ClanWindowScreen : public BWindowScreen
{
public:
	ClanWindowScreen(status_t*);
	void set_videomode(int width, int height, int bpp, bool fullscreen, bool allow_resize, bool video_memory);
	void flip_display();
	void put_display(const class CL_Rect &rect);
	uint8* get_data();
	int get_frames() { return frames; }
	bool get_connected() { return connected; }
	int get_width() { return width; }
	int get_height() { return height; }
	int get_depth() { return bpp; }
	int get_pitch() { return line_length; }
	bool is_connected() { return connected; }
	bool fill_rect_accelerated();

	rect8_hook fill_rect8;
	rect16_hook fill_rect16;
	rect32_hook fill_rect32;
	sync_hook sync;

	int write_frame;

protected:
	virtual void MessageReceived(BMessage *message);
	virtual void ScreenConnected(bool);
	uint32 find_space();
	void init_accelerations();
	
	uint8* save_buffer;
	uint8* frame_buffer;
	ulong line_length;
	int frames;
	int width, height, bpp;
	bool fullscreen, allow_resize;
	bool connected;
	bool acc_fill_rect;
};

#endif
