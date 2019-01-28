/*
	$Id: display.h,v 1.11 2002/03/01 22:00:16 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//: <p>ClanLib display and input library. This provide you with 2D display
//: support, and the input devices attached to that display.</p>
//! Global=Display

#ifndef header_display_api
#define header_display_api

#ifdef WIN32
#pragma warning( disable : 4786)
#endif

#include "Display/setupdisplay.h"

#include "Display/Display/display.h"
#include "Display/Display/vidmode.h"
#include "Display/Display/displaycard.h"
#include "Display/Display/surface.h"
#include "Display/Display/palette.h"
#include "Display/Display/pixelformat.h"
#include "Display/Display/surfaceprovider.h"
#include "Display/Display/res_surface.h"
#include "Display/Display/mousecursor.h"
#include "Display/Display/mousecursor_provider.h"
#include "Display/Display/pixeldata.h"

#include "Display/SurfaceProviders/generic_surfaceprovider.h"
#include "Display/SurfaceProviders/provider_pcx.h"
#include "Display/SurfaceProviders/provider_bmp.h"
#include "Display/SurfaceProviders/provider_targa.h"
#include "Display/SurfaceProviders/canvas.h"
#include "Display/SurfaceProviders/sprite.h"
#include "Display/SurfaceProviders/sprite2.h"
#include "Display/SurfaceProviders/sprite_subarray_provider.h"
#include "Display/SurfaceProviders/sprite_subsection_provider.h"

#include "Display/Input/input.h"
#include "Display/Input/inputbutton.h"
#include "Display/Input/inputaxis.h"
#include "Display/Input/inputhat.h"
#include "Display/Input/inputcursor.h"
#include "Display/Input/key.h"
#include "Display/Input/inputbuffer.h"
#include "Display/Input/inputdevice.h"
#include "Display/Input/keyboard.h"
#include "Display/Input/mouse.h"
#include "Display/Input/inputbutton_basic.h"
#include "Display/Input/inputbutton_group.h"
#include "Display/Input/inputaxis_basic.h"
#include "Display/Input/inputaxis_group.h"
#include "Display/Input/inputbutton_to_axis_analog.h"
#include "Display/Input/inputbutton_to_axis_digital.h"

#include "Display/Font/font.h"
#include "Display/Font/font_description.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanDisplay.lib")
#else
#pragma comment(lib, "clanDisplayd.lib")
#endif
#endif

#endif
