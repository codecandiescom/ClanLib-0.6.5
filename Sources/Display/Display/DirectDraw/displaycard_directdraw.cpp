/*
	$Id: displaycard_directdraw.cpp,v 1.12 2002/02/03 20:56:05 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "displaycard_directdraw.h"
#include "API/Display/Display/mousecursor.h"
#include "API/Display/Display/vidmode.h"
#include "blit_dx.h"
#include "API/Core/System/error.h"
#include "../Generic/colormap.h"
#include "../Generic/blitters.h"
#include "API/Core/Math/rect.h"

CL_DisplayCard_DirectDraw::CL_DisplayCard_DirectDraw(
	int card_no,
	LPGUID card_guid,
	std::string card_name)
: CL_DisplayCard_Win32Compatible(card_no)
{
	this->card_no = card_no;
	this->card_guid = card_guid;
	this->card_name = card_name;

	init_palette();

	back_buffer = NULL;
	front_buffer = NULL;
	directdraw = NULL;
	use_software_surfaces = false;
	backbuffer_in_video_memory = false;

	enumerate_displaymodes();

	slot_resize = sig_resized().connect(this, &CL_DisplayCard_DirectDraw::on_resize);
}

CL_DisplayCard_DirectDraw::~CL_DisplayCard_DirectDraw()
{
	deinit();
	destroy_displaymodes();
}

void CL_DisplayCard_DirectDraw::deinit()
{
	if (directdraw == NULL) return;
	cl_assert(get_hwnd() != NULL);

	CL_MouseCursor::hide();

	directdraw->FlipToGDISurface();

	// If fullscreen, change back to the desktop.
	if (is_fullscreen())
	{
		HRESULT err = directdraw->RestoreDisplayMode();
		cl_assert(err == DD_OK);
	}

	// Shut down directdraw object.
	// We have to release all references to the ddraw object,
	// that's why we call release_all_surfaces().

	CL_Blit_DX::release_all_surfaces();
	delete back_buffer;
	delete front_buffer;
	back_buffer = NULL;
	front_buffer = NULL;

	cl_assert(directdraw->Release()==0);
	directdraw = NULL;

	destroy_window();
}

// CL_DisplayCard functions:
// -------------------------

void CL_DisplayCard_DirectDraw::set_palette(CL_Palette *palette)
{
	// Change system palette to the specified palette.

	delete[] pal.palette;

	pal.num_colors = palette->num_colors;
	pal.palette = new unsigned char[pal.num_colors*3];

	memcpy(
		pal.palette,
		palette->palette,
		pal.num_colors*3);
}

CL_Palette *CL_DisplayCard_DirectDraw::get_palette()
{
	return &pal;
}

void CL_DisplayCard_DirectDraw::flip_display(bool sync)
{
	// Surfaces are lost if the application is minimized, or if
	// some other ddraw application runs fullscreen.
	if (front_buffer->get_surface()->IsLost())
	{
		if (front_buffer->get_surface()->Restore() == DD_OK)
		{
			CL_Blit_DX::reload_all_surfaces();
		}
	}
	signal_preflip();

	if (can_flip)
	{
		DWORD flags = DDFLIP_WAIT;
#ifdef DDFLIP_NOVSYNC
		if (!sync) flags |= DDFLIP_NOVSYNC;
#endif
		front_buffer->get_surface()->Flip(NULL, flags);
	}
	else
	{
		// Use surface Blt() because it can do some clipping for us,
		// and because manual copy is only faster if backbuffer is in sysmem.
		// (which isnn't nessesary the case here. it may just be a windows app)

		DDBLTFX ddbltfx;
		ddbltfx.dwSize = sizeof(ddbltfx);

		DWORD flags = DDBLT_WAIT;

		// I love you too Microsoft. The titlebar is part of the window!! LAMERS!
		// That is why we start juggling with client rects and other stuff...
		RECT client_rect;
		GetClientRect(get_hwnd(), &client_rect);

		RECT dest_rect;
		GetWindowRect(get_hwnd(), &dest_rect);

		POINT Pt;
		Pt.x=0;
		Pt.y=0;
		ClientToScreen(get_hwnd(),&Pt);

		dest_rect.top += Pt.y-dest_rect.top;
		dest_rect.right = dest_rect.left + back_buffer->get_width()+(Pt.x-dest_rect.left);
		dest_rect.left  += (Pt.x-dest_rect.left);
		dest_rect.bottom = dest_rect.top + back_buffer->get_height();

		front_buffer->get_surface()->Blt(
			&dest_rect,
			back_buffer->get_surface(),
			NULL,
			flags,
			&ddbltfx);
	}
	signal_postflip();
}

void CL_DisplayCard_DirectDraw::put_display(const CL_Rect &rect)
{
	// Use surface Blt() because it can do some clipping for us,
	// and because manual copy is only faster if backbuffer is in sysmem.
	// (which isnn't nessesary the case here. it may just be a windows app)

	DDBLTFX ddbltfx;
	ddbltfx.dwSize = sizeof(ddbltfx);

	DWORD flags = DDBLT_WAIT;

	// I love you too Microsoft. The titlebar is part of the window!! LAMERS!
	// That is why we start juggling with client rects and other stuff...
	RECT src_rect;
	if (can_flip)
	{
		src_rect.left = rect.x1;
		src_rect.top = rect.y1;
		src_rect.right = rect.x2;
		src_rect.bottom = rect.y2;
	}
	else
	{
		GetWindowRect(get_hwnd(), &src_rect);

		RECT client_rect;
		GetClientRect(get_hwnd(), &client_rect);

		POINT Pt;
		Pt.x=0;
		Pt.y=0;
		ClientToScreen(get_hwnd(),&Pt);

		src_rect.top += (Pt.y-src_rect.top);
		src_rect.left  += (Pt.x-src_rect.left);

		src_rect.right = src_rect.left + rect.x2;
		src_rect.bottom = src_rect.top + rect.y2;
		src_rect.left += rect.x1;
		src_rect.top += rect.y1;
	}

	front_buffer->get_surface()->Blt(
		NULL,
		back_buffer->get_surface(),
		&src_rect,
		flags,
		&ddbltfx);
}

void CL_DisplayCard_DirectDraw::sync_buffers()
{
	// Use surface Blt() because it can do some clipping for us,
	// and because manual copy is only faster if backbuffer is in sysmem.
	// (which isnn't nessesary the case here. it may just be a windows app)

	DDBLTFX ddbltfx;
	ddbltfx.dwSize = sizeof(ddbltfx);

	DWORD flags = DDBLT_WAIT;

	// I love you too Microsoft. The titlebar is part of the window!! LAMERS!
	// That is why we start juggling with client rects and other stuff...
	RECT src_rect;
	if (can_flip)
	{
		src_rect.left = 0;
		src_rect.top = 0;
		src_rect.right = front_buffer->get_width();
		src_rect.bottom = front_buffer->get_height();
	}
	else
	{
		GetWindowRect(get_hwnd(), &src_rect);

		RECT client_rect;
		GetClientRect(get_hwnd(), &client_rect);

		POINT Pt;
		Pt.x=0;
		Pt.y=0;
		ClientToScreen(get_hwnd(),&Pt);

		src_rect.top += (Pt.y-src_rect.top);
		src_rect.right = src_rect.left + back_buffer->get_width()+(Pt.x-src_rect.left);
		src_rect.left  += (Pt.x-src_rect.left);
		src_rect.bottom = src_rect.top + back_buffer->get_height();
	}

	back_buffer->get_surface()->Blt(
		NULL,
		front_buffer->get_surface(),
		&src_rect,
		flags,
		&ddbltfx);
}

void CL_DisplayCard_DirectDraw::set_videomode(
	int width,
	int height,
	int bpp,
	bool fullscreen,
	bool allow_resize,
	bool video_memory)
{
	HRESULT err;

	deinit();

	err = DirectDrawCreate(card_guid, &directdraw, NULL); 
	cl_assert(err == DD_OK);

	if (video_memory == false)
	{
		use_software_surfaces = true;
	}
	create_window(width, height, bpp, fullscreen, allow_resize);

	// Connect directdraw object to window and setup flipping system:
//	int flags = DDSCL_NORMAL | DDSCL_MULTITHREADED; //CL is now multithreaded
	int flags = DDSCL_NORMAL;

	if (fullscreen)
	{
		flags = 
			DDSCL_EXCLUSIVE |
			DDSCL_FULLSCREEN |
			DDSCL_ALLOWREBOOT|
			DDSCL_ALLOWMODEX;
	}

	err = directdraw->SetCooperativeLevel(
		get_hwnd(),
		flags);

	cl_assert(err == DD_OK);

	// If fullscreen, do a mode switch:
	if (fullscreen)
	{
		err = directdraw->SetDisplayMode(width, height, bpp);
		cl_assert(err == DD_OK);

		create_fullscreen_targets(video_memory);
	}
	else
	{
		create_windowed_targets(video_memory);
	}

	// If there already was some surfaces attached to this
	// displaycard, recreate them using the new ddraw object:
	CL_Blit_DX::create_all_surfaces();

	// Setup generic displaycard internals:
	CL_DisplayCard_Generic::set_gfxmode(
		width,
		height,
		bpp,
		fullscreen,
		allow_resize);
}

void CL_DisplayCard_DirectDraw::clear_display(
	float red,
	float green,
	float blue,
	float alpha)
{
	fill_rect(
		0,
		0,
		get_width(),
		get_height(),
		red,
		green,
		blue,
		alpha);
}

/*
void CL_DisplayCard_DirectDraw::draw_rect(
	int x1,
	int y1,
	int x2,
	int y2,
	float r,
	float g,
	float b,
	float a)
{

}
*/

void CL_DisplayCard_DirectDraw::fill_rect(
	int x1,
	int y1,
	int x2,
	int y2,
	float r,
	float g,
	float b,
	float a)
{
	// If the color is opaque, we use directdraw's hw accelerated fill function.
	// Otherwise we use the generic software implementation.

	if (a <= 0.01) return;
	if (a <= 0.99)
	{
		CL_DisplayCard_Generic::fill_rect(x1, y1, x2, y2, r, g, b, a);
		return;
	}
	
	// Translate coords into position:
	int trans_x = get_translate_offset_x();
	int trans_y = get_translate_offset_y();
	x1 += trans_x;
	x2 += trans_x;
	y1 += trans_y;
	y2 += trans_y;

	CL_ClipRect rect(x1, y1, x2, y2);

	CL_ClipRect cur_clip = get_clip_rect();
	if (cur_clip.test_all_clipped(rect)) 
	{
		return;
	}

	CL_ClipRect crect = cur_clip.clip(rect);
	CL_ColorMap cmap(get_back_buffer());
	unsigned int fill_color = cmap.calc_color(r, g, b, a);

	DDBLTFX ddbltfx;
	ddbltfx.dwSize=sizeof(ddbltfx);
	ddbltfx.dwFillColor = fill_color;

	RECT dest;
	dest.left = crect.m_x1;
	dest.top = crect.m_y1;
	dest.right = crect.m_x2;
	dest.bottom = crect.m_y2;

	back_buffer->get_surface()->Blt(
		&dest,
		NULL,
		NULL,
		DDBLT_COLORFILL|DDBLT_WAIT,
		&ddbltfx);	// clear screen
}

std::string CL_DisplayCard_DirectDraw::get_name()
{
	return card_name;
}

const std::list<CL_VidMode*> &CL_DisplayCard_DirectDraw::get_videomodes()
{
	return videomodes;
}

// CL_DisplayCard_Generic functions:
// ---------------------------------

CL_Blitters CL_DisplayCard_DirectDraw::create_hw_blitters(
	CL_SurfaceProvider *provider)
{
	// Determine wether we can/want use a hw accelerated blitter:

	CL_Blitters ret;

	// Use software blitting if backbuffer in system memory
	if (use_software_surfaces) 
	{
		return ret;
	}

	CL_Blit_DX *blitter = new CL_Blit_DX(this, provider);
	bool res = blitter->init_surface(&ret);
	if (!res)
	{
		delete blitter;
	}

	return ret;
}

// Implementation, init/deinit functions:
// --------------------------------------

void CL_DisplayCard_DirectDraw::init_palette()
{
	// Create a grayscaled palette.

	delete[] pal.palette;

	pal.num_colors = 256;
	pal.palette = new unsigned char[pal.num_colors*3];

	for (int i=0; i<pal.num_colors; i++)
	{
		pal.palette[i*3 + 0] = i;
		pal.palette[i*3 + 1] = i;
		pal.palette[i*3 + 2] = i;
	}
}

void CL_DisplayCard_DirectDraw::enumerate_displaymodes()
{
	HRESULT err;
	LPDIRECTDRAW directdraw;

	err = DirectDrawCreate(card_guid, &directdraw, NULL); 
	cl_assert(err == DD_OK);

	err = directdraw->EnumDisplayModes(
		DDEDM_STANDARDVGAMODES,
		NULL,
		(void *) &videomodes,
		enumerate_callback);

// Remarked because NT 4 fails to enumerate display modes. We will just
// have to live without that feature on that platform.
//	cl_assert(err == DD_OK);

	directdraw->Release();
}

HRESULT CALLBACK CL_DisplayCard_DirectDraw::enumerate_callback(
	LPDDSURFACEDESC lpDDSurfaceDesc,
	LPVOID lpContext)
{
	std::list<CL_VidMode*> *v_list=(std::list<CL_VidMode*> *) lpContext;

	DDSURFACEDESC surf_desc = *lpDDSurfaceDesc;

	bool modex = false;
	if ((surf_desc.ddsCaps.dwCaps & DDSCAPS_MODEX) == DDSCAPS_MODEX) modex = true;

	v_list->push_back(
		new CL_VidMode(
			surf_desc.dwWidth,
			surf_desc.dwHeight,
			surf_desc.ddpfPixelFormat.dwRGBBitCount,
			modex));

	return DDENUMRET_OK;
}

void CL_DisplayCard_DirectDraw::destroy_displaymodes()
{
	while (videomodes.empty() == false)
	{
		delete *videomodes.begin();
		videomodes.erase(videomodes.begin());
	}
}

void CL_DisplayCard_DirectDraw::create_fullscreen_targets(bool video_memory)
{
	try // to create a surface flipping system with two surfaces.
	{
		// setup the primary surface (the flipping system):
		HRESULT err;

		DDSURFACEDESC surf_desc;
		memset(&surf_desc, 0, sizeof(surf_desc));
		surf_desc.dwSize = sizeof(DDSURFACEDESC);

		surf_desc.dwFlags =
			DDSD_CAPS |
			DDSD_BACKBUFFERCOUNT;

		surf_desc.ddsCaps.dwCaps =
			DDSCAPS_PRIMARYSURFACE |
			DDSCAPS_FLIP |
			DDSCAPS_COMPLEX;

		if (video_memory) surf_desc.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
		else surf_desc.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;

		// 3 buffers are more effecient, but at the price of memory.
		// We should make it possible for the application to specify the
		// buffer count.
		surf_desc.dwBackBufferCount = 2;

		LPDIRECTDRAWSURFACE primary_surface;
    directdraw->SetCooperativeLevel(hwnd,DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN|DDSCL_ALLOWREBOOT|DDSCL_ALLOWMODEX);
		err = directdraw->CreateSurface(&surf_desc, &primary_surface, NULL);
		if (err != DD_OK) throw CL_Error("Couldn't create flipping system.");

		// Get the backbuffer surface from the primary/front:
		DDSCAPS backbuffer_desc;
		backbuffer_desc.dwCaps = DDSCAPS_BACKBUFFER;

		LPDIRECTDRAWSURFACE backbuffer_surface;
		err = primary_surface->GetAttachedSurface(
			&backbuffer_desc,
			&backbuffer_surface);

		if (err != DD_OK)
		{
			primary_surface->Release();
			throw CL_Error("Couldn't create flipping system.");
		}

		// Wrap the directx front and backbuffer surfaces into the CL_Target interface:
		front_buffer = new CL_Target_DX(primary_surface);
		back_buffer = new CL_Target_DX(backbuffer_surface);

		can_flip = true;
	}
	catch (CL_Error err) // not enough memory, fallback to system memory backbuffer.
	{
		create_windowed_targets(video_memory);
	}
}

void CL_DisplayCard_DirectDraw::create_windowed_targets(bool video_memory)
{
	// setup the front buffer surface:
	HRESULT err;

	DDSURFACEDESC surf_desc;
	memset(&surf_desc, 0, sizeof(surf_desc));
	surf_desc.dwSize = sizeof(DDSURFACEDESC);
	surf_desc.dwFlags = DDSD_CAPS;
	surf_desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	LPDIRECTDRAWSURFACE primary_surface;
	err = directdraw->CreateSurface(&surf_desc, &primary_surface, NULL);
	if (err != DD_OK) throw CL_Error("Couldn't create front buffer.");

	// Wrap front buffer into the CL_Target interface:
	RECT client_rect;
	GetClientRect(get_hwnd(), &client_rect);

	RECT dest_rect;
	GetWindowRect(get_hwnd(), &dest_rect);

	POINT Pt;
	Pt.x=0;
	Pt.y=0;
	ClientToScreen(get_hwnd(),&Pt);

	dest_rect.top += (Pt.y-dest_rect.top);
	dest_rect.left  += (Pt.x-dest_rect.left);
	front_buffer = new CL_FrontbufferTarget_DX(
		primary_surface,
		this/*,
		dest_rect.right-dest_rect.left,
		dest_rect.bottom-dest_rect.top*/);

	backbuffer_in_video_memory = video_memory;
	create_windowed_backbuffer(
		dest_rect.right-dest_rect.left,
		dest_rect.bottom-dest_rect.top);

	// Clip blits to the window borders:
	LPDIRECTDRAWCLIPPER clipper;
	err = directdraw->CreateClipper(0, &clipper, NULL);
	cl_assert(err == DD_OK);
	clipper->SetHWnd(0, get_hwnd());

	// attach clipper to front buffer:
	front_buffer->get_surface()->SetClipper(clipper);
	clipper->Release();

	can_flip = false;
}

void CL_DisplayCard_DirectDraw::create_windowed_backbuffer(int width, int height)
{
	if (width == 0 || height == 0) return; // minimized.

	HRESULT err = 0;

	// clean up previous backbuffer:
	delete back_buffer;
	back_buffer = NULL;

	// create back buffer surface: (trying in framebuffer first)

	DDSURFACEDESC surf_desc;
	memset(&surf_desc, 0, sizeof(surf_desc));
	surf_desc.dwSize = sizeof(DDSURFACEDESC);
	surf_desc.dwFlags=DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
	surf_desc.dwWidth = width;
	surf_desc.dwHeight = height;
	surf_desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	if (backbuffer_in_video_memory) surf_desc.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
	else surf_desc.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;

	LPDIRECTDRAWSURFACE backbuffer_surface;

//	directdraw->SetCooperativeLevel(hwnd,DDSCL_NORMAL| DDSCL_MULTITHREADED); //Make sure we set the co-op level
	directdraw->SetCooperativeLevel(hwnd,DDSCL_NORMAL); //Make sure we set the co-op level
	err = directdraw->CreateSurface(&surf_desc, &backbuffer_surface, NULL);

	if (err != DD_OK) // damn. Not enough memory in video, fallback to system memory.
	{
		surf_desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;

		err = directdraw->CreateSurface(&surf_desc, &backbuffer_surface, NULL);
		cl_assert(err == DD_OK);
	}

	// Wrap back buffer into the CL_Target interface:
	back_buffer = new CL_Target_DX(backbuffer_surface);
}

// Implementation, nicer data retrieval:
// ------------------------------------

DDCAPS CL_DisplayCard_DirectDraw::get_hel_caps()
{
	HRESULT err;
	DDCAPS hel_caps;
	hel_caps.dwSize = sizeof(DDCAPS);
 
	err = directdraw->GetCaps(NULL, &hel_caps);	
	cl_assert(err == DD_OK);

	return hel_caps;
}

DDCAPS CL_DisplayCard_DirectDraw::get_hal_caps()
{
	HRESULT err;
	DDCAPS hal_caps;
	hal_caps.dwSize = sizeof(DDCAPS);
 
	err = directdraw->GetCaps(&hal_caps, NULL);
	cl_assert(err == DD_OK);

	return hal_caps;
}

CL_DisplayCard_DirectDraw::CL_FrontbufferTarget_DX::CL_FrontbufferTarget_DX(
	LPDIRECTDRAWSURFACE surf,
	CL_DisplayCard_DirectDraw *parent)
: CL_Target_DX(surf), m_parent(parent)
{
}

void *CL_DisplayCard_DirectDraw::CL_FrontbufferTarget_DX::get_data() const
{
	RECT client_rect;
	GetClientRect(m_parent->get_hwnd(), &client_rect);

	RECT dest_rect;
	GetWindowRect(m_parent->get_hwnd(), &dest_rect);

	POINT Pt;
	Pt.x=0;
	Pt.y=0;
	ClientToScreen(m_parent->get_hwnd(),&Pt);

	dest_rect.top += (Pt.y-dest_rect.top);
	dest_rect.left  += (Pt.x-dest_rect.left);
	return (void *) &((unsigned char *) m_data)[dest_rect.left*(m_depth/8)+dest_rect.top*m_pitch];
}

void CL_DisplayCard_DirectDraw::on_resize(int x, int y)
{
	if (is_fullscreen() == false)
	{
		create_windowed_backbuffer(x, y);
	}
}
