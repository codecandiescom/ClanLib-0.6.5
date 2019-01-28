/*
	$Id: blit_opaque.h,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_blit_opaque
#define header_blit_opaque

#include "blitters.h"
class CL_SurfaceProvider;

class CL_Blit_Opaque :
	public CL_Blit_NoClip,
	public CL_Blit_Clip,
	public CL_Blit_Scale_NoClip,
	public CL_Blit_Scale_Clip
{
protected:
	unsigned char *image;
	unsigned int image_pitch, image_bytes_per_pixel;
	unsigned int width, height, no_sprs;

public:
	CL_Blit_Opaque(
		CL_SurfaceProvider *provider,
		int bytes_per_pixel,
		CL_Target *target);

	virtual ~CL_Blit_Opaque();

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
