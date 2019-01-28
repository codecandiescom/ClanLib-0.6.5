/*
	$Id: blit_dynamic.h,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_blit_dynamic
#define header_blit_dynamic

#include "blitters.h"
#include "API/Display/Display/target.h"
#include <Hermes/Hermes.h>

class CL_SurfaceProvider;

class CL_Blit_Dynamic :
	public CL_Blit_NoClip,
	public CL_Blit_Clip,
	public CL_Blit_Scale_NoClip,
	public CL_Blit_Scale_Clip
{
protected:
	CL_SurfaceProvider *provider;

	HermesFormat *dest_fmt, *src_fmt;
	HermesHandle handle, palette;

	bool use_palette;


public:
	CL_Blit_Dynamic(
		CL_SurfaceProvider *provider,
		CL_Target *target);

	virtual ~CL_Blit_Dynamic();

	virtual void blt_noclip(
		CL_Target *dest_surf,
		int x,
		int y,
		int spr_no);

	virtual void blt_clip(
		CL_Target *dest_surf,
		int x,
		int y,
		int spr_no,
		const CL_ClipRect &clip);

	virtual void blt_scale_noclip(
		CL_Target *dest_surf,
		int x,
		int y,
		int dest_width,
		int dest_height,
		int spr_no);

	virtual void blt_scale_clip(
		CL_Target *dest_surf,
		int x,
		int y,
		int dest_width,
		int dest_height,
		int spr_no,
		const CL_ClipRect &clip);
};

#endif
