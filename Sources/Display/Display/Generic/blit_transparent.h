/*
	$Id: blit_transparent.h,v 1.2 2001/04/28 12:04:36 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	Transparent blitter.
	This file by Martin Starch (starch@dark.x.dtu.dk) 3/99
	------------------------------------------------------------------------
*/

#ifndef header_transparent_blitter
#define header_transparent_blitter

#include "blitters.h"
#include "colormap.h"

class CL_DisplayCard_Generic;
class CL_SurfaceProvider;
class CL_Blit_AlphaMask_RLE;
class CL_Blit_Transparent;

template <class T>
class BltRLERow_Transparent
{
public:
	static void show_row(
		T *src_data,
		T *dst_data,
		int rep)
	{
		memcpy(dst_data, src_data, rep*sizeof(T));
	}
};

template <class T>
class SubBlitter_Transparent
{
public:
	static void blt_noclip(
		CL_Blit_Transparent *parent,
		CL_Target *target,
		int x,
		int y,
		int spr_no);

	static void blt_clip(
		CL_Blit_Transparent *parent,
		CL_Target *target,
		int x,
		int y,
		int spr_no, 
		const CL_ClipRect &clip);

	static void blt_scale_noclip(
		CL_Blit_Transparent *parent,
		CL_Target *target,
		int x,
		int y,
		int dest_width,
		int dest_height,
		int spr_no);

	static void blt_scale_clip(
		CL_Blit_Transparent *parent,
		CL_Target *target,
		int x,
		int y,
		int dest_width,
		int dest_height,
		int spr_no,
		const CL_ClipRect &clip);
private:
	static void unpack_line(
		T *dest,
		unsigned char *transparent_dest,
		unsigned char *source_data,
		int num_pixels);
};


class CL_Blit_Transparent : 
	public CL_Blit_NoClip,
	public CL_Blit_Clip,
	public CL_Blit_Scale_NoClip,
	public CL_Blit_Scale_Clip
{
public:
	CL_Blit_Transparent(CL_DisplayCard_Generic *card, CL_SurfaceProvider *provider);
	virtual ~CL_Blit_Transparent();

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

private:
	CL_DisplayCard_Generic *m_card;
	CL_SurfaceProvider *m_provider;
	CL_Blit_AlphaMask_RLE *m_contained_alpha_blitter;

	unsigned char **m_lines;
	unsigned int m_num_lines;

	unsigned int m_width, m_height;

	enum RLECommand
	{
		cmd_skip,
		cmd_copy
	};

	friend class SubBlitter_Transparent<unsigned short>;
	friend class SubBlitter_Transparent<unsigned int>;

	void make_line(
		CL_Target *target,
		CL_SurfaceProvider *provider,
		int line_num,
		unsigned char *input_data,
		unsigned char *alpha_data);

};

#endif
