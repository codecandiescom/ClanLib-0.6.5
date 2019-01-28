/*
	$Id: target_directfb.h,v 1.1 2001/05/03 16:04:06 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_target_directfb
#define header_target_directfb

#ifdef USE_DIRECTFB

#include <directfb.h>

#include "API/Display/Display/target.h"
#include "API/Display/Display/palette.h"

class CL_Target_DirectFB : public CL_Target
{
 public:
  CL_Target_DirectFB (IDirectFBSurface *surface = NULL);
		
  virtual ~CL_Target_DirectFB();

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

 public:
  void SetClip (const CL_ClipRect &clip);
  void Flip (DFBRegion *region, DFBSurfaceFlipFlags flags);

  void Clear (float r, float g, float b, float a);
  void FillRectangle (int x1, int y1, int x2, int y2, float r, float g, float b, float a);
  void DrawRectangle (int x1, int y1, int x2, int y2, float r, float g, float b, float a);
  void DrawLine (int x1, int y1, int x2, int y2, float r, float g, float b, float a);

  IDirectFBSurface *GetSurface() const;

 private:
  IDirectFB *dfb;
  IDirectFBSurface *surface;

  bool  locked;
  void *data;
  int   pitch;
};

#endif /*USE_DIRECTFB*/

#endif
