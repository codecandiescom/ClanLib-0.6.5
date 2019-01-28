/*
	$Id: target_directfb.cpp,v 1.2 2002/02/26 20:38:11 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_DIRECTFB

#include <Display/Display/DirectFB/target_directfb.h>

CL_Target_DirectFB::CL_Target_DirectFB (IDirectFBSurface *surface)
{
  DFBResult ret;

  ret = DirectFBCreate (&dfb);
  if (ret)
    {
      DirectFBError ("CL_Target_DirectFB - DirectFBCreate", ret);
      throw new CL_Error ("CL_Target_DirectFB - DirectFBCreate failed");
    }

  if (!surface)
    {
      DFBSurfaceDescription dsc;

      dsc.flags = DSDESC_CAPS;
      dsc.caps  = (DFBSurfaceCapabilities)(DSCAPS_PRIMARY | DSCAPS_FLIPPING);

      ret = dfb->CreateSurface (dfb, &dsc, &surface);
      if (ret)
	{
	  DirectFBError ("CL_Target_DirectFB - CreateSurface", ret);
	  dfb->Release (dfb);
	  throw new CL_Error ("CL_Target_DirectFB - CreateSurface failed");
	}
    }
  else
    surface->AddRef (surface);

  this->surface = surface;
  this->locked = false;
}
		
CL_Target_DirectFB::~CL_Target_DirectFB()
{
  surface->Release (surface);
  dfb->Release (dfb);
}

void CL_Target_DirectFB::lock()
{
  DFBResult ret;

  ret = surface->Lock (surface,
		       DSLF_WRITE, /* FIXME */
		       &data, &pitch);
  if (ret)
    {
      DirectFBError ("CL_Target_DirectFB - Lock", ret);
      throw new CL_Error ("CL_Target_DirectFB - Lock failed");
    }

  locked = true;
}

void CL_Target_DirectFB::unlock()
{
  if (!locked)
    throw new CL_Error ("CL_Target_DirectFB - unlock called, but target not locked");

  surface->Unlock (surface);
  locked = false;
}

bool CL_Target_DirectFB::is_video() const
{
  /* FIXME */
  return true;
}

void *CL_Target_DirectFB::get_data() const
{
  if (!locked)
    throw new CL_Error ("CL_Target_DirectFB - get_data called, but target not locked");
      
  return data;
}

unsigned int CL_Target_DirectFB::get_num_frames() const
{
  /* FIXME */
  return 2;
}

unsigned int CL_Target_DirectFB::get_width() const
{
  unsigned int width = 0;

  surface->GetSize (surface, &width, NULL);

  return width;
}

unsigned int CL_Target_DirectFB::get_height() const
{
  unsigned int height = 0;

  surface->GetSize (surface, NULL, &height);

  return height;
}

unsigned int CL_Target_DirectFB::get_pitch() const
{
  if (!locked)
    throw new CL_Error ("CL_Target_DirectFB - get_pitch called, but target not locked");
      
  return pitch;
}
	
unsigned int CL_Target_DirectFB::get_depth() const
{
  DFBSurfacePixelFormat format;

  surface->GetPixelFormat (surface, &format);

  return DFB_BYTES_PER_PIXEL (format)*8;
}

unsigned int CL_Target_DirectFB::get_red_mask() const
{
  DFBSurfacePixelFormat format;

  surface->GetPixelFormat (surface, &format);

  switch (format)
    {
    case DSPF_A8:
      return 0;
    case DSPF_RGB15:
      return 0x007c00;
    case DSPF_RGB16:
      return 0x00f800;
    case DSPF_RGB24:
    case DSPF_RGB32:
    case DSPF_ARGB:
      return 0xff0000;
    default:
      throw new CL_Error ("CL_Target_DirectFB - unknown pixel format");
    }
}

unsigned int CL_Target_DirectFB::get_green_mask() const
{
  DFBSurfacePixelFormat format;

  surface->GetPixelFormat (surface, &format);

  switch (format)
    {
    case DSPF_A8:
      return 0;
    case DSPF_RGB15:
      return 0x0003e0;
    case DSPF_RGB16:
      return 0x0007e0;
    case DSPF_RGB24:
    case DSPF_RGB32:
    case DSPF_ARGB:
      return 0x00ff00;
    default:
      throw new CL_Error ("CL_Target_DirectFB - unknown pixel format");
    }
}

unsigned int CL_Target_DirectFB::get_blue_mask() const
{
  DFBSurfacePixelFormat format;

  surface->GetPixelFormat (surface, &format);

  switch (format)
    {
    case DSPF_A8:
      return 0;
    case DSPF_RGB15:
    case DSPF_RGB16:
      return 0x00001f;
    case DSPF_RGB24:
    case DSPF_RGB32:
    case DSPF_ARGB:
      return 0x0000ff;
    default:
      throw new CL_Error ("CL_Target_DirectFB - unknown pixel format");
    }
}

unsigned int CL_Target_DirectFB::get_alpha_mask() const
{
  DFBSurfacePixelFormat format;

  surface->GetPixelFormat (surface, &format);

  switch (format)
    {
    case DSPF_A8:
      return 0xff;
    case DSPF_RGB15:
    case DSPF_RGB16:
    case DSPF_RGB24:
    case DSPF_RGB32:
      return 0;
    case DSPF_ARGB:
      return 0xff000000;
    default:
      throw new CL_Error ("CL_Target_DirectFB - unknown pixel format");
    }
}

CL_Palette *CL_Target_DirectFB::get_palette() const
{
  return NULL;
}

void CL_Target_DirectFB::set_palette(CL_Palette *pal)
{
}

bool CL_Target_DirectFB::is_indexed() const
{
  /* FIXME */
  return false;
}




void CL_Target_DirectFB::SetClip (const CL_ClipRect &clip)
{
  DFBRegion region = { clip.m_x1, clip.m_y1, clip.m_x2 - 1, clip.m_y2 - 1 };

  surface->SetClip (surface, &region);
}

void CL_Target_DirectFB::Flip (DFBRegion *region, DFBSurfaceFlipFlags flags)
{
  DFBResult ret;

  ret = surface->Flip (surface, region, flags);
  if (ret)
    DirectFBError ("CL_Target_DirectFB - Flip", ret);
}

void CL_Target_DirectFB::Clear (float r, float g, float b, float a)
{
  unsigned int width, height;

  surface->GetSize (surface, &width, &height);

  surface->SetDrawingFlags (surface, DSDRAW_NOFX);

  surface->SetColor (surface,
		     (__u8) (r * 255.0f),
		     (__u8) (g * 255.0f),
		     (__u8) (b * 255.0f),
		     (__u8) (a * 255.0f));

  surface->FillRectangle (surface, 0, 0, width, height);
}

void CL_Target_DirectFB::FillRectangle (int x1, int y1, int x2, int y2,
					float r, float g, float b, float a)
{
  if (a < 1.0f)
    surface->SetDrawingFlags (surface, DSDRAW_BLEND);
  else
    surface->SetDrawingFlags (surface, DSDRAW_NOFX);

  surface->SetColor (surface,
		     (__u8) (r * 255.0f),
		     (__u8) (g * 255.0f),
		     (__u8) (b * 255.0f),
		     (__u8) (a * 255.0f));

  surface->FillRectangle (surface, x1, y1, x2 - x1, y2 - y1);
}

void CL_Target_DirectFB::DrawRectangle (int x1, int y1, int x2, int y2,
					float r, float g, float b, float a)
{
  if (a < 1.0f)
    surface->SetDrawingFlags (surface, DSDRAW_BLEND);
  else
    surface->SetDrawingFlags (surface, DSDRAW_NOFX);

  surface->SetColor (surface,
		     (__u8) (r * 255.0f),
		     (__u8) (g * 255.0f),
		     (__u8) (b * 255.0f),
		     (__u8) (a * 255.0f));

  surface->DrawRectangle (surface, x1, y1, x2 - x1, y2 - y1);
}

void CL_Target_DirectFB::DrawLine (int x1, int y1, int x2, int y2,
				   float r, float g, float b, float a)
{
  if (a < 1.0f)
    surface->SetDrawingFlags (surface, DSDRAW_BLEND);
  else
    surface->SetDrawingFlags (surface, DSDRAW_NOFX);

  surface->SetColor (surface,
		     (__u8) (r * 255.0f),
		     (__u8) (g * 255.0f),
		     (__u8) (b * 255.0f),
		     (__u8) (a * 255.0f));

  surface->DrawLine (surface, x1, y1, x2, y2);
}

IDirectFBSurface *CL_Target_DirectFB::GetSurface () const
{
  return surface;
}


#endif
