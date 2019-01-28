/*
	$Id: setupdisplay_win32.cpp,v 1.7 2002/01/22 10:14:04 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#include "Core/precomp.h"

#define INITGUID
#include <ddraw.h>
#include <dinput.h>
#include <Hermes/hermes.h>

#include "API/Display/setupdisplay.h"
#include "API/Display/Display/display.h"

#include "Display/Display/DirectDraw/displaycard_directdraw.h"
#include "Display/Input/Win32/input_dx.h"

#include "Display/System/Generic/setupdisplay_generic.h"

/////////////////////////////////////////////////////////////////////////////
// clanGL hookup interface:

bool global_uses_opengl = false;
void (*opengl_init_function)()= NULL;

/////////////////////////////////////////////////////////////////////////////
// CL_SetupDisplay initialization

static CL_Input_DX *input_dx = NULL;
static int init_ref_count = 0;

void CL_SetupDisplay::init(bool register_resources_only)
{
	init_ref_count++;
	if (init_ref_count > 1) return;

	CL_SetupDisplay_Generic::init();

	if (register_resources_only) return;

	Hermes_Init();

	// Init input (note: this should be altered, too ugly impl.):
	input_dx = new CL_Input_DX;

	// todo: enumerate directdraw display cards.

	if (global_uses_opengl && opengl_init_function != NULL)
	{
		opengl_init_function();
	}
	else
	{
		CL_Display::cards.push_back(
			new CL_DisplayCard_DirectDraw(
				0,                // card_no
				NULL,             // card_guid, NULL = default screen,
				"default card")); // card_name
	}
}
	
/////////////////////////////////////////////////////////////////////////////
// CL_SetupDisplay deinitialization

void CL_SetupDisplay::deinit()
{
	init_ref_count--;
	if (init_ref_count > 0) return;

	Hermes_Done();

	int size = CL_Display::cards.size();
	for (int i = 0; i < size; i++)
	{
		delete CL_Display::cards[i];
	}

	CL_Display::cards.clear();

	// input_dx might be NULL if one has called CL_SetupDisplay::init(true)
	if (input_dx) delete input_dx;

	CL_SetupDisplay_Generic::deinit();
}

/////////////////////////////////////////////////////////////////////////////
// CL_SetupDisplay implementation
