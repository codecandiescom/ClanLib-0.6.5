/*
	$Id: surface_generic.cpp,v 1.5 2002/06/13 23:44:02 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <API/Display/Display/display.h>
#include <API/Display/Display/displaycard.h>
#include <API/Display/Display/surface.h>
#include <API/Display/Display/surfaceprovider.h>
#include <Display/Display/Generic/displaycard_generic.h>
#include <Display/Display/Generic/surface_generic.h>
#include <Display/Display/Generic/blit_dynamic.h>
#include "cardsurface_blitter.h"
#include "colormap.h"
#include "blit_macros.h"

extern "C"
{
	#include <Hermes/H_Conv.h>
	#include <Hermes/H_Pal.h>
}

CL_Surface_Generic::CL_Surface_Generic(
	CL_SurfaceProvider *_provider,
	bool _delete_provider,
	bool _dynamic)
{
	ref_count = 0;
	provider = _provider;
	delete_provider = _delete_provider;
	translate_x = 0;
	translate_y = 0;
	dynamic = _dynamic;
	
	provider->lock();
	width = provider->get_width();
	height = provider->get_height();
	no_sprs = provider->get_num_frames();
	translate_x = provider->get_translate_offset_x();
	translate_y = provider->get_translate_offset_y();
	provider->unlock();

	card_surfaces = new CL_CardSurface*[CL_Display::cards.size()];

	int num_cards = CL_Display::cards.size();
	for (int i=0; i<num_cards; i++)
	{
		card_surfaces[i] = NULL;
	}
}

CL_Surface_Generic::~CL_Surface_Generic()
{
	int num_cards = CL_Display::cards.size();
	for (int i=0; i<num_cards; i++) delete card_surfaces[i];
	delete[] card_surfaces;

	if (delete_provider == true) delete provider;
}

int CL_Surface_Generic::add_reference()
{
	return ++ref_count;
}

int CL_Surface_Generic::release_reference()
{
	return --ref_count;
}

int CL_Surface_Generic::get_reference_count ()
{
	return ref_count;
}

inline int CL_Surface_Generic::ensure_surface_prepared(CL_DisplayCard *card)
{
	int card_no = card == NULL ? CL_Display::get_current_card()->get_card_no() : card->get_card_no();
	if (card_surfaces[card_no] == NULL)
	{
		provider->lock();

		translate_x = provider->get_translate_x();
		translate_y = provider->get_translate_y();

		width = provider->get_width();
		height = provider->get_height();
		no_sprs = provider->get_num_frames();

		CL_DisplayCard_Generic *gen = ((CL_DisplayCard_Generic *) CL_Display::cards[card_no]);
		if (dynamic)
		{
			card_surfaces[card_no] = new CL_CardSurface_Blitter(gen, provider, true);
		}
		else
		{
			card_surfaces[card_no] = gen->create_cardsurface_video(provider);
			if (card_surfaces[card_no] == NULL) card_surfaces[card_no] = gen->create_cardsurface_system(provider);
		}

		provider->unlock();
	}

	return card_no;
}

CL_CardSurface *CL_Surface_Generic::get_cardsurface(int surface_num) 
{ 
	return card_surfaces[ensure_surface_prepared(CL_Display::cards[surface_num])]; 
}

CL_CardSurface *CL_Surface_Generic::get_cardsurface(CL_DisplayCard *card) 
{ 
	return card_surfaces[ensure_surface_prepared(card)];
}

void CL_Surface_Generic::flush(CL_DisplayCard *card)
{
	if (card == NULL)
	{
		delete card_surfaces[CL_Display::get_current_card()->get_card_no()];
		card_surfaces[CL_Display::get_current_card()->get_card_no()] = NULL;
	}
	else
	{
		delete card_surfaces[card->get_card_no()];
		card_surfaces[card->get_card_no()] = NULL;
	}
}

void CL_Surface_Generic::reload()
{
	bool provider_locked = false;

	int num_cards = CL_Display::cards.size();
	for (int i=0; i<num_cards; i++)
	{
		if (card_surfaces[i] != NULL)
		{
			if (!provider_locked)
			{
				provider->lock();
				translate_x = provider->get_translate_x();
				translate_y = provider->get_translate_y();

				width = provider->get_width();
				height = provider->get_height();
				no_sprs = provider->get_num_frames();

				provider_locked = true;
			}
			card_surfaces[i]->reload();
		}
	}

	if (provider_locked) provider->unlock();
}

bool CL_Surface_Generic::is_video(CL_DisplayCard *card) const
{
	int card_no = card == NULL ? CL_Display::get_current_card()->get_card_no() : card->get_card_no();

	return card_surfaces[card_no] != NULL && card_surfaces[card_no]->is_video();
}

bool CL_Surface_Generic::is_loaded(CL_DisplayCard *card) const
{
	int card_no = card == NULL ? CL_Display::get_current_card()->get_card_no() : card->get_card_no();

	return card_surfaces[card_no] != NULL;
}

bool CL_Surface_Generic::convert_video(CL_DisplayCard *card)
{
	if (dynamic) return false;

	int card_no = card == NULL ? CL_Display::get_current_card()->get_card_no() : card->get_card_no();

	translate_x = provider->get_translate_x();
	translate_y = provider->get_translate_y();

	width = provider->get_width();
	height = provider->get_height();
	no_sprs = provider->get_num_frames();

	if (card_surfaces[card_no] == NULL)
	{
		CL_DisplayCard_Generic *gen = ((CL_DisplayCard_Generic *) CL_Display::cards[card_no]);
		card_surfaces[card_no] = gen->create_cardsurface_video(provider);

		if (card_surfaces[card_no] == NULL) return false;
	}
	else
	{
		if (card_surfaces[card_no]->is_video()) return true;

		if (card_surfaces[card_no]->can_convert_video())
		{
			return card_surfaces[card_no]->convert_video();
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool CL_Surface_Generic::convert_system(CL_DisplayCard *card)
{
	if (dynamic) return true;

	int card_no = card == NULL ? CL_Display::get_current_card()->get_card_no() : card->get_card_no();

	if (card_surfaces[card_no] == NULL)
	{
		CL_DisplayCard_Generic *gen = ((CL_DisplayCard_Generic *) CL_Display::cards[card_no]);
		card_surfaces[card_no] = gen->create_cardsurface_system(provider);

		if (card_surfaces[card_no] == NULL) return false;
	}
	else
	{
		if (!card_surfaces[card_no]->is_video()) return true;

		if (card_surfaces[card_no]->can_convert_system())
		{
			return card_surfaces[card_no]->convert_system();
		}
		else
		{
			return false;
		}
	}
	return true;
}

CL_SurfaceProvider *CL_Surface_Generic::get_provider() const
{
	return provider;
}

void CL_Surface_Generic::put_screen(
	int x,
	int y,
	int spr_no,
	CL_DisplayCard *card)
{
	card_surfaces[ensure_surface_prepared(card)]->put_screen(
		x+translate_x,
		y+translate_y,
		spr_no,
		NULL);
}

void CL_Surface_Generic::put_screen(
	int x,
	int y,
	float scale_x,
	float scale_y,
	int spr_no,
	CL_DisplayCard *card)
{
	card_surfaces[ensure_surface_prepared(card)]->put_screen(
		x+translate_x,
		y+translate_y,
		scale_x,
		scale_y,
		spr_no,
		NULL);
}

void CL_Surface_Generic::put_screen(
	int x,
	int y,
	int size_x,
	int size_y,
	int spr_no,
	CL_DisplayCard *card)
{
	card_surfaces[ensure_surface_prepared(card)]->put_screen(
		x+translate_x,
		y+translate_y,
		size_x,
		size_y,
		spr_no,
		NULL);
}

void CL_Surface_Generic::put_target(
	int x,
	int y,
	int spr_no,
	CL_Target *target)
{
	CL_Blit_Dynamic blitter(provider, target);
	
	// Calc clipping:
	CL_ClipRect t_cliprect = target->get_clip_rect();
	CL_ClipRect s_cliprect(x, y, provider->get_width()+x, provider->get_height()+y);

	// todo: return if source clip rect is totally outside target clip rect

	if (t_cliprect.test_clipped(s_cliprect))
	{
		s_cliprect = t_cliprect.clip(s_cliprect);

		blitter.blt_clip(
			target,
			x,
			y,
			spr_no,
			s_cliprect);
	}
	else
	{
		blitter.blt_noclip(
			target,
			x,
			y,
			spr_no);
	}
}
