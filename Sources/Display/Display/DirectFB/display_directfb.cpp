/*
	$Id: display_directfb.cpp,v 1.2 2002/02/26 20:38:11 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_DIRECTFB


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <math.h>

#include <API/Display/Display/mousecursor.h>
#include <API/Display/Display/cliprect.h>
#include <API/Display/Display/palette.h>
#include <API/Display/Display/vidmode.h>
#include <API/Core/System/error.h>
#include <API/Core/System/cl_assert.h>
#include <API/Core/Math/rect.h>
#include <Display/Display/Generic/colormap.h>
#include <Display/Display/Generic/blitters.h>
#include <Display/Display/Generic/displaycard_generic.h>
#include <Display/Display/DirectFB/display_directfb.h>
#include <Display/Display/DirectFB/target_directfb.h>
#include <Display/Display/DirectFB/blit_directfb.h>

CL_DirectFB_DisplayCard::CL_DirectFB_DisplayCard(int card_no) : CL_DisplayCard_Generic(card_no)
{
  DFBResult ret;

  m_initialized = false;

  m_palette = new CL_Palette();

  m_target = NULL;

  ret = DirectFBCreate (&dfb);
  if (ret)
    {
      DirectFBError ("CL_DirectFB_DisplayCard - DirectFBCreate", ret);
      throw new CL_Error ("CL_DirectFB_DisplayCard - DirectFBCreate failed");
    }

  ret = dfb->GetDisplayLayer (dfb, DLID_PRIMARY, &layer);
  if (ret)
    {
      DirectFBError ("CL_DirectFB_DisplayCard - GetDisplayLayer", ret);
      dfb->Release (dfb);
      throw new CL_Error ("CL_DirectFB_DisplayCard - GetDisplayLayer failed");
    }

  layer->EnableCursor (layer, 1);
}

CL_DirectFB_DisplayCard::~CL_DirectFB_DisplayCard()
{
  //  CL_MouseCursor::hide();

  if (m_target)
    delete m_target;

  layer->Release (layer);
  dfb->Release (dfb);

  delete m_palette;
}

void CL_DirectFB_DisplayCard::flip_display(bool sync)
{
  cl_assert(m_initialized); // card not initialized!
	
  signal_preflip();

  m_target->Flip (NULL, sync ? DSFLIP_WAITFORSYNC : (DFBSurfaceFlipFlags) 0);

  signal_postflip();
}

void CL_DirectFB_DisplayCard::put_display(const class CL_Rect &rect)
{
  DFBRegion region = { rect.x1, rect.y1, rect.x2 - 1, rect.y2 - 1 };


  cl_assert(m_initialized);

  signal_preflip();

  m_target->Flip (&region, DSFLIP_BLIT);

  signal_postflip();
}

void CL_DirectFB_DisplayCard::sync_buffers()
{
  cl_assert(m_initialized);

  signal_preflip();

  m_target->Flip (NULL, DSFLIP_BLIT);

  signal_postflip();
}

CL_Blitters CL_DirectFB_DisplayCard::create_hw_blitters (CL_SurfaceProvider *provider)
{
  // Determine wether we can/want use a hw accelerated blitter:
  CL_Blitters ret;

  CL_Blit_DirectFB *blitter = new CL_Blit_DirectFB (this, provider);
  bool res = blitter->init_surface(&ret);
  if (!res)
    {
      delete blitter;
    }

  return ret;
}

void CL_DirectFB_DisplayCard::set_palette(CL_Palette *pal)
{
  m_palette = new CL_Palette(pal->palette);
}

CL_Palette *CL_DirectFB_DisplayCard::get_palette()
{
  return m_palette;
}

void CL_DirectFB_DisplayCard::set_videomode(
				       int  width,
				       int  height,
				       int  bpp,
				       bool fullscreen,
				       bool allow_resize,
				       bool video_memory)
{
  DFBResult ret;

  if (m_target)
    {
      delete m_target;
      m_target = NULL;
    }

  fullscreen = true;

  set_gfxmode( width, height, bpp, fullscreen, allow_resize );

  if (fullscreen)
    dfb->SetCooperativeLevel (dfb, DFSCL_FULLSCREEN);
  else
    dfb->SetCooperativeLevel (dfb, DFSCL_NORMAL);

  ret = dfb->SetVideoMode (dfb, get_width(), get_height(), bpp);
  if (ret)
    {
      char buf[200];

      sprintf (buf, "CL_DirectFB_DisplayCard - SetVideoMode (%dx%d @%dbit)",
	       get_width(), get_height(), bpp);

      DirectFBError (buf, ret);

      if (fullscreen)
	{
	  cerr << "CL_DirectFB_DisplayCard: Falling back to DFSCL_NORMAL" << endl;
	  dfb->SetCooperativeLevel (dfb, DFSCL_NORMAL);
	  ret = dfb->SetVideoMode (dfb, get_width(), get_height(), bpp);
	}

      if (ret)
	throw CL_Error ( "cannot set mode!" );
    }

  m_target = new CL_Target_DirectFB();

  m_initialized = true;
}

bool CL_DirectFB_DisplayCard::is_initialized()
{
  return m_initialized;
}

const std::list<CL_VidMode*> &CL_DirectFB_DisplayCard::get_videomodes()
{
  cl_assert(false); // not implemented yet!

  static std::list<CL_VidMode*> ret;

  return ret;
}

void CL_DirectFB_DisplayCard::draw_rect (int x1, int y1,
					 int x2, int y2,
					 float r, float g, float b, float a)
{
  m_target->DrawRectangle (x1, y1, x2, y2, r, g, b, a);
}

void CL_DirectFB_DisplayCard::fill_rect (int x1, int y1,
					 int x2, int y2,
					 float r, float g, float b, float a)
{
  m_target->FillRectangle (x1, y1, x2, y2, r, g, b, a);
}

void CL_DirectFB_DisplayCard::draw_line (int x1, int y1,
					 int x2, int y2,
					 float r, float g, float b, float a)
{
  m_target->DrawLine (x1, y1, x2, y2, r, g, b, a);
}


void CL_DirectFB_DisplayCard::clear_display (float r, float g, float b, float a)
{
  m_target->Clear (r, g, b, a);
}

void CL_DirectFB_DisplayCard::push_clip_rect()
{
  m_clip_stack.push_front(m_cur_clip);
}

void CL_DirectFB_DisplayCard::push_clip_rect (const CL_ClipRect &clip)
{
  push_clip_rect();
  m_cur_clip = m_cur_clip.clip (clip);

  if (m_target)
    m_target->SetClip (clip);
}

void CL_DirectFB_DisplayCard::set_clip_rect (const CL_ClipRect &clip)
{
  m_cur_clip = clip;

  if (m_target)
    m_target->SetClip (clip);
}

void CL_DirectFB_DisplayCard::pop_clip_rect()
{
  m_cur_clip = m_clip_stack.front();
  m_clip_stack.pop_front();

  if (m_target)
    m_target->SetClip (m_cur_clip);
}

#endif
