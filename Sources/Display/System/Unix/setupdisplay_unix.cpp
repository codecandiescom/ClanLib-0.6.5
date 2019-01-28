/*
	$Id: setupdisplay_unix.cpp,v 1.12 2002/01/22 10:14:03 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "display_target.h"
#include "Core/System/Unix/appconf.h"
#include "API/Core/System/error.h"
#include "API/display.h"
#include "Display/System/Generic/setupdisplay_generic.h"
#include <Hermes/Hermes.h>
#include "setupdisplay_unix.h"

#ifdef USE_X11
#include "x11_target.h"
#endif

#ifdef USE_GGI
#include "ggi_target.h"
#endif

#ifdef USE_FBDEV
#include "fbdev_target.h"
#endif

#ifdef USE_DIRECTFB
#include "directfb_target.h"
#endif

// we do not keep the list of display targets static because we want clanGL
// to be able to create a GLX target for us. -- mbn
std::vector<CL_DisplayTarget*> targets;

static void init_target_list();
static void deinit_target_list();
static CL_DisplayTarget *chosen_display = NULL;
static CL_DisplayCard *cl_current_displaycard = NULL;

static int display_ref_count = 0;
void CL_SetupDisplay::init(bool register_resources_only)
{
	display_ref_count++;
	if (display_ref_count > 1) return;

 	CL_SetupDisplay_Generic::init();

	if (register_resources_only) return;

	Hermes_Init();

	init_target_list();
	int num_targets = targets.size();

	FileConfig config("clanlib");
	std::string display = config.readEntry("display", "default");

	// Check if any library forces a particular display target:
	if (strcmp(CL_Force_DispTarget::get_name().c_str(), "") != 0)
	{
		display = CL_Force_DispTarget::get_name();
	}

	// try and use the X11 target, if it is available. If it is not, ask the user.
	if (display == "default")
	{
		bool found = false;
		display = "x11";
		for (int i=0;i<num_targets;i++) 
		{
			if (!strcasecmp(targets[i]->get_short_name(),display.c_str()))
			{
				found = true;
				break;
			}
		}
		
		if (found == false) display = "ask";
	}

	// If we do not have a display variable, ask user for target:
	if (getenv("DISPLAY")==NULL && display=="x11") 
		display="ask";

	if (getenv("CLANLIB_FORCE_X11_DISPLAY"))  
		display = "x11"; 
	

	char choice[16];
	if (display == "ask")
	{
		std::cout << std::endl << "Please select a display target by entering its abbreviation: " << std::endl;
		
		for (int i=0;i<num_targets;i++) 
		{
			std::cout << "	[" << targets[i]->get_short_name() << "] ";
			std::cout << targets[i]->get_long_name() << std::endl;
		}

		std::cout << "Your choice: ";
		std::cin >> choice;
	}
	else
	{
		strcpy(choice, display.c_str());
	}

	for (int i=0;i<num_targets;i++) 
	{
		if (!strcasecmp(targets[i]->get_short_name(),choice))
		{
			chosen_display = targets[i];
			break;
		}
	}

	if (!chosen_display)
	{
		throw CL_Error("Cannot open display target");
	}

	chosen_display->init();

	cl_current_displaycard = CL_Display::cards[0];
}

void CL_SetupDisplay::deinit()
{
	display_ref_count--;
	if (display_ref_count > 0) return;

	if (chosen_display) chosen_display->deinit();

	int num_cards = CL_Display::cards.size();
	for (int i=0; i<num_cards; i++) delete CL_Display::cards[i];
	CL_Display::cards.clear();

	// Shut down any input part left behind:

	int num_keyboards = CL_Input::keyboards.size();
	int num_joysticks = CL_Input::joysticks.size();
	int num_pointers = CL_Input::pointers.size();

	for (int i=0; i<num_keyboards; i++) delete CL_Input::keyboards[i];
	for (int i=0; i<num_joysticks; i++) delete CL_Input::joysticks[i];
	for (int i=0; i<num_pointers; i++) delete CL_Input::pointers[i];

	deinit_target_list();
	Hermes_Done();

	CL_SetupDisplay_Generic::deinit();
}

void init_target_list()
{
#ifdef USE_X11
	targets.push_back(new CL_X11Target);
#endif

#ifdef USE_GGI
	targets.push_back(new CL_GGITarget);
#endif

#ifdef USE_FBDEV
	targets.push_back(new CL_FBDevTarget);
#endif

#ifdef USE_DIRECTFB
	targets.push_back(new CL_DirectFBTarget);
#endif
}

void deinit_target_list()
{
	int len = targets.size();
	for (int i=0; i<len; i++) delete targets[i];
	targets = std::vector<CL_DisplayTarget *>();
}

void CL_Display::select_card(CL_DisplayCard *card)
{
	cl_current_displaycard = card;
}

CL_DisplayCard *CL_Display::get_current_card()
{
	return cl_current_displaycard;
}

std::string CL_Force_DispTarget::name;

CL_Force_DispTarget::CL_Force_DispTarget(const char *_name)
{
	// Make sure there is only one display target force!
	cl_assert( name.empty() );

	name = _name;
}
