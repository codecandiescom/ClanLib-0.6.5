/*
	$Id: surface_generic.h,v 1.4 2002/06/13 23:44:02 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_surface_generic
#define header_surface_generic

#include "cardsurface.h"

class CL_Surface_Generic
{
public:
	CL_Surface_Generic(
		CL_SurfaceProvider *provider,
		bool delete_provider,
		bool dynamic);

	virtual ~CL_Surface_Generic();

	virtual void flush(CL_DisplayCard *card = NULL);
	virtual CL_SurfaceProvider *get_provider() const;

	virtual CL_CardSurface *get_cardsurface(int surface_num);
	virtual CL_CardSurface *get_cardsurface(CL_DisplayCard *card = NULL);

	// Display functions
	virtual void put_screen(
		int x,
		int y,
		int spr_no=0,
		CL_DisplayCard *card=NULL);

	virtual void put_screen(
		int x,
		int y,
		float scale_x,
		float scale_y,
		int spr_no=0,
		CL_DisplayCard *card=NULL);

	virtual void put_screen(
		int x,
		int y,
		int size_x,
		int size_y,
		int spr_no=0,
		CL_DisplayCard *card=NULL);
	
	virtual void put_target(
		int x,
		int y,
		int spr_no,
		CL_Target *target);

	virtual unsigned int get_width() const { return width; }
	virtual unsigned int get_height() const { return height; }
	virtual unsigned int get_num_frames() const { return no_sprs; }

	// is_video() returns true if in videomemory
	// on the specified card (null = current dispcard)
	virtual bool is_video(CL_DisplayCard *card = NULL) const;
	virtual bool is_loaded(CL_DisplayCard *card = NULL) const;

	// returns true if successfully loaded into videomemory, or already there
	virtual bool convert_video(CL_DisplayCard *card = NULL);

	// never fails! (or serious heap usage!)
	virtual bool convert_system(CL_DisplayCard *card = NULL);

	virtual void reload();

	int add_reference();
	int release_reference();

	//: Return the number of references to this surface
	int get_reference_count ();

protected:
	CL_SurfaceProvider *provider;
	bool delete_provider;
	CL_CardSurface **card_surfaces;
	int width, height, no_sprs;
	int translate_x, translate_y;
	bool dynamic;

	int ref_count;

	inline int ensure_surface_prepared(CL_DisplayCard *card);
};

#endif
