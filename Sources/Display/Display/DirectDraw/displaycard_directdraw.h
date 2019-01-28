/*
	$Id: displaycard_directdraw.h,v 1.4 2001/11/29 13:07:59 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_displaycard_directdraw
#define header_displaycard_directdraw

#include "displaycard_win32compatible.h"

#include "dx_target.h"

class CL_DisplayCard_DirectDraw : public CL_DisplayCard_Win32Compatible
{
public:
	CL_DisplayCard_DirectDraw(
		int card_no,
		LPGUID card_guid,
		std::string card_name);

	virtual ~CL_DisplayCard_DirectDraw();

	// CL_DisplayCard functions:
	// -------------------------

	virtual void set_palette(CL_Palette *palette);
	virtual CL_Palette *get_palette();
	virtual void flip_display(bool sync=false);
	virtual void put_display(const class CL_Rect &rect);
	virtual void set_videomode(
		int width,
		int height,
		int bpp,
		bool full_screen,
		bool allow_resize,
		bool video_memory);

	virtual void clear_display(float red=0, float green=0, float blue=0, float alpha=1);
	virtual void fill_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a);
	//virtual void draw_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a);

	virtual std::string get_name();
	virtual const std::list<CL_VidMode*> &get_videomodes();
	virtual int get_total_memory() { return -1; }

	virtual CL_Target *get_target() { return back_buffer; }

	virtual bool is_initialized() { return (get_hwnd() != NULL); }

	virtual void sync_buffers();

	// CL_DisplayCard_Generic functions:
	// ---------------------------------

	virtual CL_Target *get_frontbuffer() { return front_buffer; }
	virtual CL_Blitters create_hw_blitters(CL_SurfaceProvider *provider);

	// Used by CL_Blit_DX:
	// -------------------

	DDCAPS get_hel_caps();
	DDCAPS get_hal_caps();
	LPDIRECTDRAW get_directdraw() { return directdraw; }
	CL_Target_DX *get_back_buffer() { return back_buffer; }

private:
	virtual void init_palette();

	void deinit();

	void enumerate_displaymodes();
	void destroy_displaymodes();
	static HRESULT CALLBACK enumerate_callback(
		LPDDSURFACEDESC lpDDSurfaceDesc,
		LPVOID lpContext);

	void create_fullscreen_targets(bool video_memory);
	void create_windowed_targets(bool video_memory);
	void create_windowed_backbuffer(int width, int height);

	void on_resize(int x, int y);

private:
	LPDIRECTDRAW directdraw; 
	CL_Target_DX *back_buffer;

	class CL_FrontbufferTarget_DX : public CL_Target_DX
	{
	public:
		CL_FrontbufferTarget_DX(
			LPDIRECTDRAWSURFACE surf,
			CL_DisplayCard_DirectDraw *parent);
		virtual ~CL_FrontbufferTarget_DX() { ; }
		virtual void *get_data() const;
		virtual unsigned int get_width() const { return m_parent->get_width(); }
		virtual unsigned int get_height() const { return m_parent->get_height(); }
	private:
		CL_DisplayCard_DirectDraw *m_parent;
	};
	CL_Target_DX *front_buffer;
	bool can_flip;
	bool use_software_surfaces;

	CL_Palette pal;

	int card_no;
	LPGUID card_guid;
	std::string card_name;

	std::list<CL_VidMode*> videomodes;

	bool backbuffer_in_video_memory;
	CL_Slot slot_resize;
};

#endif
