/*
	$Id: blit_directfb.h,v 1.2 2001/09/08 19:12:46 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#ifndef header_blit_directfb
#define header_blit_directfb

#include "../Generic/blitters.h"
#include "display_directfb.h"

class CL_Blit_DirectFB :
	public CL_Blit_NoClip,
	public CL_Blit_Clip,
	public CL_Blit_Scale_NoClip,
	public CL_Blit_Scale_Clip
{
 public:
  CL_Blit_DirectFB (CL_DirectFB_DisplayCard *target, CL_SurfaceProvider *provider);
  virtual ~CL_Blit_DirectFB();

  bool init_surface(CL_Blitters *blitters);

  virtual void blt_noclip(
			  CL_Target *target,
			  int x,
			  int y,
			  int spr_no);

  virtual void blt_clip(
			CL_Target *target,
			int x,
			int y,
			int spr_no,
			const CL_ClipRect &clip);
	
  virtual void blt_scale_noclip(
				CL_Target *target,
				int x,
				int y,
				int dest_width,
				int dest_height,
				int spr_no);

  virtual void blt_scale_clip(
			      CL_Target *target,
			      int x,
			      int y,
			      int dest_width,
			      int dest_height,
			      int spr_no,
			      const CL_ClipRect &clip);

  static void reload_all_surfaces();
  static void release_all_surfaces();
  static void create_all_surfaces();

 private:
  CL_DirectFB_DisplayCard *m_target;
  CL_SurfaceProvider *m_provider;
  unsigned int m_width, m_height;
  DFBSurfaceBlittingFlags m_flags;

  IDirectFB *dfb;
  IDirectFBSurface *m_surface;

  bool reload(bool lock_provider, bool create_surface);

  static std::list<CL_Blit_DirectFB*> m_loaded_surfaces;
};

#endif
