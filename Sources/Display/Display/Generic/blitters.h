/*
	$Id: blitters.h,v 1.2 2001/04/12 19:58:17 vaidhy Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
	
	TODO:
	
	 - Add blitters for primitive types: lines, triangles and polygons.
*/

#ifndef header_blitters
#define header_blitters

class CL_Target;
#include <stdlib.h>
#include "API/Display/Display/cliprect.h"
/*
#include "API/Display/Display/surfaceprovider.h"
#include "API/Display/Display/target.h"
#include "API/Display/Display/pixeldata.h"
*/
class CL_RefCountable
{
public:
	void add_ref() { ref++; }
	int release_ref() { ref--; return ref; }

	virtual ~CL_RefCountable() { return; }
protected:
	CL_RefCountable() { ref = 0; }
private:
	int ref;
};

class CL_Blit_NoClip : virtual public CL_RefCountable
{
public:
	virtual ~CL_Blit_NoClip() { return; }

	virtual void blt_noclip(
		CL_Target *target,
		int x,
		int y,
		int spr_no)=0;
};

class CL_Blit_Clip : virtual public CL_RefCountable
{
public:
	virtual ~CL_Blit_Clip() { return; }

	virtual void blt_clip(
		CL_Target *target,
		int x,
		int y,
		int spr_no,
		const CL_ClipRect &clip)=0;
};

class CL_Blit_Scale_NoClip : virtual public CL_RefCountable
{
public:
	virtual ~CL_Blit_Scale_NoClip() { return; }

	virtual void blt_scale_noclip(
		CL_Target *target,
		int x,
		int y,
		int dest_width,
		int dest_height,
		int spr_no)=0;
};

class CL_Blit_Scale_Clip : virtual public CL_RefCountable
{
public:
	virtual ~CL_Blit_Scale_Clip() { return; }

	virtual void blt_scale_clip(
		CL_Target *target,
		int x,
		int y,
		int dest_width,
		int dest_height,
		int spr_no,
		const CL_ClipRect &clip)=0;
};

class CL_Blitters
{
public:
	CL_Blitters()
	{
		noclip = NULL;
		clip = NULL;
		scale_noclip = NULL;
		scale_clip = NULL;
	}
	
	void delete_all()
	{
		delete_noclip();
		delete_clip();
		delete_scale_noclip();
		delete_scale_clip();
	}
	
	void set_noclip(CL_Blit_NoClip *ptr)
	{
		delete_noclip();
		noclip = ptr;
		ptr->add_ref();
	}
	
	void set_clip(CL_Blit_Clip *ptr)
	{
		delete_clip();
		clip = ptr;
		ptr->add_ref();
	}
	
	void set_scale_noclip(CL_Blit_Scale_NoClip *ptr)
	{
		delete_scale_noclip();
		scale_noclip = ptr;
		ptr->add_ref();
	}
	
	void set_scale_clip(CL_Blit_Scale_Clip *ptr)
	{
		delete_scale_clip();
		scale_clip = ptr;
		ptr->add_ref();
	}
	
	void delete_noclip() { remove(noclip); noclip = NULL; }
	void delete_clip() { remove(clip); clip = NULL; }
	void delete_scale_noclip() { remove(scale_noclip); scale_noclip = NULL; }
	void delete_scale_clip() { remove(scale_clip); scale_clip = NULL; }
	
	bool test_noclip() { return (noclip == NULL) ? false : true; }
	bool test_clip() { return (clip == NULL) ? false : true; }
	bool test_scale_noclip() { return (scale_noclip == NULL) ? false : true; }
	bool test_scale_clip() { return (scale_clip == NULL) ? false : true; }
	
	bool test_complete()
	{
		return
			test_noclip() &&
			test_clip() &&
			test_scale_noclip() &&
			test_scale_clip();
	}
	
	CL_Blit_NoClip *get_noclip() { return noclip; }
	CL_Blit_Clip *get_clip() { return clip; }
	CL_Blit_Scale_NoClip *get_scale_noclip() { return scale_noclip; }
	CL_Blit_Scale_Clip *get_scale_clip() { return scale_clip; }

private:
	CL_Blit_NoClip *noclip;
	CL_Blit_Clip *clip;
	CL_Blit_Scale_NoClip *scale_noclip;
	CL_Blit_Scale_Clip *scale_clip;

	void remove(CL_RefCountable *ptr)
	{
		if (ptr != NULL && ptr->release_ref() == 0) delete ptr;
	}
};

#endif
