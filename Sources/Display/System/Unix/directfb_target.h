/*
	$Id: directfb_target.h,v 1.3 2001/09/08 19:12:49 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#ifndef header_directfb_target
#define header_directfb_target

#ifdef USE_DIRECTFB

#include <directfb.h>

#include "display_target.h"


class CL_DirectFBTarget : public CL_DisplayTarget
{
 public:
  virtual const char *get_long_name();

  virtual const char *get_short_name();

  virtual void init();

  virtual void deinit();

 protected:
  IDirectFB *dfb;
};

#endif

#endif
