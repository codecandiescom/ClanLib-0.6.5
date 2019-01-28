/*
	$Id: blit_directfb.cpp,v 1.4 2001/11/01 19:19:55 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <API/Display/Display/surfaceprovider.h>
#include "../Generic/colormap.h"
#include "API/Display/Display/pixeldata.h"

#include "blit_directfb.h"


#define MAX(a,b) ((a) < (b) ? (b) : (a))


std::list<CL_Blit_DirectFB*> CL_Blit_DirectFB::m_loaded_surfaces;

CL_Blit_DirectFB::CL_Blit_DirectFB(CL_DirectFB_DisplayCard *target, CL_SurfaceProvider *provider)
{
  DFBResult ret;

  ret = DirectFBCreate (&dfb);
  if (ret)
    {
      DirectFBError ("CL_Blit_DirectFB - DirectFBCreate", ret);
      throw new CL_Error ("CL_Blit_DirectFB - DirectFBCreate failed");
    }

  m_target   = target;
  m_provider = provider;
  m_surface  = NULL;
}

CL_Blit_DirectFB::~CL_Blit_DirectFB()
{
  if (m_surface != NULL) 
    {
      m_surface->Release (m_surface);
      m_loaded_surfaces.remove(this);
    }

  dfb->Release (dfb);
}

bool CL_Blit_DirectFB::init_surface (CL_Blitters *blitters)
{
  if (!reload(false, true))
    return false;

  blitters->set_clip(this);
  blitters->set_noclip(this);
  blitters->set_scale_noclip(this);
  blitters->set_scale_clip(this);

  m_loaded_surfaces.push_back(this);

  return true;
}

void CL_Blit_DirectFB::blt_noclip(
				  CL_Target *target,
				  int x,
				  int y,
				  int spr_no)
{
  IDirectFBSurface *dest = ((CL_Target_DirectFB*)target)->GetSurface();
  DFBRectangle source = { 0, spr_no*m_height, m_width, m_height };

  dest->SetBlittingFlags (dest, m_flags);
  dest->Blit (dest, m_surface, &source, x, y);
}

void CL_Blit_DirectFB::blt_clip(
				CL_Target *target,
				int x,
				int y,
				int spr_no,
				const CL_ClipRect &clip)
{
  blt_noclip (target, x, y, spr_no);
}

void CL_Blit_DirectFB::blt_scale_noclip(
					CL_Target *target,
					int x,
					int y,
					int dest_width,
					int dest_height,
					int spr_no)
{
  IDirectFBSurface *dest = ((CL_Target_DirectFB*)target)->GetSurface();
  DFBRectangle source = { 0, spr_no*m_height, m_width, m_height };
  DFBRectangle destination = { x, y, dest_width, dest_height };

  dest->SetBlittingFlags (dest, m_flags);
  dest->StretchBlit (dest, m_surface, &source, &destination);
}

void CL_Blit_DirectFB::blt_scale_clip(
				      CL_Target *target,
				      int x,
				      int y,
				      int dest_width,
				      int dest_height,
				      int spr_no,
				      const CL_ClipRect &clip)
{
  blt_scale_noclip (target, x, y, dest_width, dest_height, spr_no);
}

bool CL_Blit_DirectFB::reload(bool lock_provider, bool create_surface)
{
  DFBResult ret;

  if (lock_provider)
    m_provider->lock();

  unsigned int num_lines = m_provider->get_height()*m_provider->get_num_frames();

  CL_PixelData alpha(
		     0,
		     0,
		     0,
		     255,
		     m_provider,
		     1);

  int bytes_per_line = alpha.get_bytes_per_line();
  unsigned char *alpha_data = new unsigned char[num_lines*bytes_per_line];

  bool has_alpha = false;
  bool m_transparent = false;

  int alpha_width = alpha.get_width();
  for (unsigned int y=0; y<num_lines; y++)
    {
      unsigned char *l = &alpha_data[y*bytes_per_line];
      alpha.get_line_pixel_to_dest(y, l);
      
      for (int x=0;x<alpha_width;x++)
	{
	  if (l[x] == 0)
	    m_transparent = true;
	  if (l[x] != 0 && l[x] != 255) 
	    {
	      m_transparent = true;
	      has_alpha = true;
	      break;
	    }
	}
    }
  
  if (has_alpha)
    {
      m_flags = DSBLIT_BLEND_ALPHACHANNEL;
      m_transparent = false;
    }
  else if (m_transparent)
    m_flags = DSBLIT_SRC_COLORKEY;
  else
    m_flags = DSBLIT_NOFX;


  CL_PixelData *input;

  if (has_alpha)
    input = new CL_PixelData (
			      0x00ff0000,
			      0x0000ff00,
			      0x000000ff,
			      0xff000000,
			      m_provider,
			      4);
  else
    input = new CL_PixelData (
			      m_target->get_target()->get_red_mask(),
			      m_target->get_target()->get_green_mask(),
			      m_target->get_target()->get_blue_mask(),
			      m_target->get_target()->get_alpha_mask(),
			      m_provider,
			      (m_target->get_target()->get_depth()+7)/8);

  m_width = m_provider->get_width();
  m_height = m_provider->get_height();

  if (create_surface)
    {
      if (m_surface)
	{
	  m_surface->Release (m_surface);
	  m_surface = NULL;
	}


      DFBSurfaceDescription dsc;

      dsc.flags  = (DFBSurfaceDescriptionFlags) (DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_PIXELFORMAT);
      dsc.width  = m_width;
      dsc.height = m_height*m_provider->get_num_frames();

      if (has_alpha)
	dsc.pixelformat = DSPF_ARGB;
      else
	{
	  IDirectFBSurface *surface = ((CL_Target_DirectFB*)(m_target->get_target()))->GetSurface();

	  surface->GetPixelFormat (surface, &dsc.pixelformat);
	}

      ret = dfb->CreateSurface (dfb, &dsc, &m_surface);
      if (ret)
	{
	  DirectFBError ("CL_Blit_DirectFB - CreateSurface", ret);
	  delete[] alpha_data;
	  delete input;
	  return false;
	}
    }


  
  char *surface_data;
  int   surface_pitch;

  ret = m_surface->Lock (m_surface, DSLF_WRITE, (void**)&surface_data, &surface_pitch);
  if (ret)
    {
      DirectFBError ("CL_Blit_DirectFB - Lock", ret);
      delete[] alpha_data;
      delete input;
      m_surface->Release (m_surface);
      m_surface = NULL;
      return false;
    }


  /* FIXME: This code has to be cleaned up, we should not use the target's format.
     For now it's ok, because the created surface has the same format. */

  if (m_transparent)
    {
      int input_bytes_per_line = input->get_bytes_per_line();
      unsigned char *image_data = new unsigned char[num_lines*input_bytes_per_line];

      switch ((m_target->get_target()->get_depth()+7)/8)
	{
	case 1:
	  {
	    for (unsigned int y=0;y<num_lines;y++)
	      {
		unsigned char *input_data = &image_data[y*input_bytes_per_line];
		input->get_line_pixel_to_dest(y, input_data);
		
		unsigned char *a = &alpha_data[y*bytes_per_line];
		for (unsigned int x=0;x<m_width;x++)
		  {
		    if (a[x] == 255)
		      {
			surface_data[x] = MAX (input_data[x], 1);
		      }
		    else
		      {
			surface_data[x] = 0;
		      }
		  }
		surface_data += surface_pitch;
	      }
	  }
	  break;
	case 2:
	  {
	    for (unsigned int y=0;y<num_lines;y++)
	      {
		unsigned char *input_data = &image_data[y*input_bytes_per_line];
		input->get_line_pixel_to_dest(y, input_data);
		unsigned char *a = &alpha_data[y*bytes_per_line];
		
		for (unsigned int x=0;x<m_width;x++)
		  {
		    if (a[x] == 255)
		      {
			((unsigned short *) surface_data)[x] = MAX (((unsigned short *) input_data)[x], 1);
		      }
		    else
		      {
			((unsigned short *) surface_data)[x] = 0;
		      }
		  }
		surface_data += surface_pitch;
	      }
	  }
	  break;
	case 3:
	  {
	    // Our 24 bit support really sucks!
	    cl_assert(false);
	  }
	  break;
	case 4:
	  {
	    for (unsigned int y=0;y<num_lines;y++)
	      {
		unsigned char *input_data = &image_data[y*input_bytes_per_line];
		input->get_line_pixel_to_dest(y, input_data);
		unsigned char *a = &alpha_data[y*bytes_per_line];
		
		for (unsigned int x=0;x<m_width;x++)
		  {
		    if (a[x] == 255)
		      {
			((unsigned int *) surface_data)[x] = MAX (((unsigned int *) input_data)[x], 1);
		      }
		    else
		      {
			((unsigned int *) surface_data)[x] = 0;
		      }
		  }
		surface_data += surface_pitch;
	      }
	  }
	  break;
	}
      delete[] image_data;
    }
  else
    {
      int bytes_per_line = input->get_bytes_per_pixel()*input->get_width();
      for (unsigned int y=0;y<num_lines;y++)
	{
	  memcpy(
		 &surface_data[y*surface_pitch],
		 input->get_line_pixel(y),
		 bytes_per_line);
	}
    }
  
  m_surface->Unlock (m_surface);
  

  if (lock_provider)
    m_provider->unlock();

  delete[] alpha_data;
  delete input;
  
  return true;
}

void CL_Blit_DirectFB::reload_all_surfaces()
{
  for (
       std::list<CL_Blit_DirectFB*>::iterator counter = m_loaded_surfaces.begin();
       counter != m_loaded_surfaces.end();
       counter++)
    {
      (*counter)->reload(true, false);
    }
}

void CL_Blit_DirectFB::release_all_surfaces()
{
  for (
       std::list<CL_Blit_DirectFB*>::iterator counter = m_loaded_surfaces.begin();
       counter != m_loaded_surfaces.end();
       counter++)
    {
      (*counter)->m_surface->Release( (*counter)->m_surface );
      (*counter)->m_surface = NULL;
    }
}

void CL_Blit_DirectFB::create_all_surfaces()
{
  for (
       std::list<CL_Blit_DirectFB*>::iterator counter = m_loaded_surfaces.begin();
       counter != m_loaded_surfaces.end();
       counter++)
    {
      (*counter)->reload(true, true);
    }
}
