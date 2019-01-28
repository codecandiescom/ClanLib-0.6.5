/*
	$Id: blit_alphamask_rle.h,v 1.3 2001/09/02 17:33:37 starch Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	Alpha-only blitter.
	This file by Martin Starch (starch@dark.x.dtu.dk) 3/99
	------------------------------------------------------------------------
*/

#ifndef header_alphamask_blitter
#define header_alphamask_blitter

#include "blitters.h"
#include "colormap.h"

class CL_DisplayCard_Generic;
class CL_SurfaceProvider;
class CL_Blit_AlphaMask_RLE;

template <class T>
class BltRLERow_AlphaMask
{
protected:
	T rmask, gmask, bmask, rstart, gstart, bstart, rlength, glength, blength;
	T *lookup_r;
	T *lookup_g;
	T *lookup_b;

public:
	BltRLERow_AlphaMask(
		T _rmask,
		T _gmask,
		T _bmask,
		T _rstart,
		T _gstart,
		T _bstart,
		T _rlength,
		T _glength,
		T _blength) : 
			rmask(_rmask), gmask(_gmask), bmask(_bmask), 
			rstart(_rstart), gstart(_gstart), bstart(_bstart),
			rlength(_rlength), glength(_glength), blength(_blength)
	{
		int rsize = 2<<rlength;
		int gsize = 2<<glength;
		int bsize = 2<<blength;
		lookup_r = new T[rsize*256];
		lookup_g = new T[gsize*256];
		lookup_b = new T[bsize*256];

		int i;

		for (i=0;i<256;i++)
		{
			int g;
			for (g=0;g<rsize;g++) lookup_r[(g<<8)+i] = (T) (float(g)*i)/255;
			for (g=0;g<gsize;g++) lookup_g[(g<<8)+i] = (T) (float(g)*i)/255;
			for (g=0;g<bsize;g++) lookup_b[(g<<8)+i] = (T) (float(g)*i)/255;
		}
	}

	virtual ~BltRLERow_AlphaMask()
	{
		delete[] lookup_r;
		delete[] lookup_g;
		delete[] lookup_b;
	}

	virtual void show_alpha_row(
		T *src_data,
		T *dst_data,
		unsigned char *alpha_data,
		int rep)=0;
};

template <class T>
class BLTRLERow_AlphaMask_Direct : public BltRLERow_AlphaMask<T>
{
public:
	BLTRLERow_AlphaMask_Direct(
		T _rmask,
		T _gmask,
		T _bmask,
		T _rstart,
		T _gstart,
		T _bstart,
		T _rlength,
		T _glength,
		T _blength) : BltRLERow_AlphaMask<T>(_rmask, _gmask, _bmask, _rstart, _gstart, _bstart, _rlength, _glength, _blength)
	{
	}

	virtual void show_alpha_row(
		T *src_data,
		T *dst_data,
		unsigned char *alpha_data,
		int rep)
	{
		for (int x=0;x<rep;x++)
		{
			T a = alpha_data[x];
			T destval = dst_data[x];
			T r = lookup_r[(((destval&rmask)>>rstart)<<8)+a];	//(((destval&rmask)>>rstart)*a)>>8;
			T g = lookup_g[(((destval&gmask)>>gstart)<<8)+a];	//(((destval&gmask)>>gstart)*a)>>8;
			T b = lookup_b[(((destval&bmask)>>bstart)<<8)+a];	//(((destval&bmask)>>bstart)*a)>>8;

			T amix = 
				(r<<rstart) +
				(g<<gstart) + 
				(b<<bstart);

			dst_data[x] = amix + *(src_data++);
		}
	}
};

template <class T>
class BLTRLERow_AlphaMask_Buffered : public BltRLERow_AlphaMask<T>
{
public:
	BLTRLERow_AlphaMask_Buffered(
		T _rmask,
		T _gmask,
		T _bmask,
		T _rstart,
		T _gstart,
		T _bstart,
		T _rlength,
		T _glength,
		T _blength) : BltRLERow_AlphaMask<T>(_rmask, _gmask, _bmask, _rstart, _gstart, _bstart, _rlength, _glength, _blength)
	{
	}

	virtual void show_alpha_row(
		T *src_data,
		T *dst_data,
		unsigned char *alpha_data,
		int rep)
	{
		T dst_buf[1280];
		memcpy(dst_buf, dst_data, sizeof(T)*rep);

		for (int x=0;x<rep;++x)
		{
			T a = alpha_data[x];
			T destval = dst_buf[x];
			T r = lookup_r[(((destval&rmask)>>rstart)<<8)+a];
			T g = lookup_g[(((destval&gmask)>>gstart)<<8)+a];
			T b = lookup_b[(((destval&bmask)>>bstart)<<8)+a];
			T amix = 
				(r<<rstart) +
				(g<<gstart) + 
				(b<<bstart);
			dst_buf[x] = amix + *(src_data++);
		}
		memcpy(dst_data, dst_buf, sizeof(T)*rep);
	}
};

template <class T>
class SubBlitter_AlphaMask
{
public:
	static void blt_noclip(
		CL_Blit_AlphaMask_RLE *parent,
		CL_Target *target,
		BltRLERow_AlphaMask<T> *row_blitter,
		int x,
		int y,
		int spr_no);

	static void blt_clip(
		CL_Blit_AlphaMask_RLE *parent,
		CL_Target *target,
		BltRLERow_AlphaMask<T> *row_blitter,
		int x,
		int y,
		int spr_no, 
		const CL_ClipRect &clip);

	static void blt_scale_noclip(
		CL_Blit_AlphaMask_RLE *parent,
		CL_Target *target,
		BltRLERow_AlphaMask<T> *row_blitter,
		int x,
		int y,
		int dest_width,
		int dest_height,
		int spr_no);

	static void blt_scale_clip(
		CL_Blit_AlphaMask_RLE *parent,
		CL_Target *target,
		BltRLERow_AlphaMask<T> *row_blitter,
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

template <class T> 
class PixelConstructor_AlphaMask
{
public:
	static void save_pixel(
		CL_Target *target,
		CL_ColorMap *colormap,
		unsigned char *value,
		unsigned char *dest,
		unsigned char alpha,
		int x,
		int *cmd_pos)
	{
		T val = ((T *) value)[x];
		T r = (((val&target->get_red_mask())>>colormap->m_red_start)*alpha)>>8;
		T g = (((val&target->get_green_mask())>>colormap->m_green_start)*alpha)>>8;
		T b = (((val&target->get_blue_mask())>>colormap->m_blue_start)*alpha)>>8;

		*((T *) dest) = 
			(r<<colormap->m_red_start)+
			(g<<colormap->m_green_start)+
			(b<<colormap->m_blue_start);

		*cmd_pos += sizeof(T);
	}
};


class CL_Blit_AlphaMask_RLE :
	public CL_Blit_NoClip,
	public CL_Blit_Clip,
	public CL_Blit_Scale_NoClip,
	public CL_Blit_Scale_Clip
{
public:
	CL_Blit_AlphaMask_RLE(
		CL_DisplayCard_Generic *card,
		CL_SurfaceProvider *provider,
		unsigned char *image_data,
		int image_bytes_pr_line,
		unsigned char *alpha_data,
		int alpha_bytes_pr_line);
		
	virtual ~CL_Blit_AlphaMask_RLE();

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

////////////////////////////////////////////////////////////////////////////////////////
private:
	unsigned char **m_lines;
	unsigned char **m_alpha_lines;
	unsigned int m_num_lines;

	unsigned int m_width, m_height;

	enum
	{
		cmd_skip,
		cmd_alpha
	};

	friend class SubBlitter_AlphaMask<unsigned short>;
	friend class SubBlitter_AlphaMask<unsigned int>;

	static BltRLERow_AlphaMask<unsigned short> *bltrow_16bit;
	static BltRLERow_AlphaMask<unsigned int> *bltrow_32bit;

	void check_16bit_rowblitter(CL_Target *target);
	void check_32bit_rowblitter(CL_Target *target);

	void make_line(
		CL_Target *target,
		CL_SurfaceProvider *provider,
		int line_num,
		unsigned char *input_data,
		unsigned char *alpha_data);
};

#endif
