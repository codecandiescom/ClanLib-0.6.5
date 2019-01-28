/*
	$Id: core.h,v 1.41 2002/03/01 22:00:15 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//: <p>This is the heart of ClanLib. The core library contain the glue that
//: binds all other clanlib libraries together. It contain general platform
//: independence classes and functions. It also setup the keep alive pump and
//: other fundamental attributes of ClanLib.</p>
//! Global=Core

#ifndef header_core
#define header_core

#define CL_VERSION(x,y,z)	( (x << 16) | (y << 8) | (z) )
#define CL_CURRENT_VERSION	CL_VERSION(0,6,0)
#define CL_VERSION_STRING "0.6.0"
#define CL_RELEASE_NAME "When the shit hits the fan"

#ifdef WIN32
#pragma warning( disable : 4786)
#endif

#ifdef __BORLANDC__
#define BORLAND
#endif

#include "Core/System/setupcore.h"
#include "Core/System/keep_alive.h"
#include "Core/System/system.h"
#include "Core/System/cl_assert.h"
#include "Core/System/error.h"
#include "Core/System/thread.h"
#include "Core/System/threadfunc_v0.h"
#include "Core/System/threadfunc_v1.h"
#include "Core/System/threadfunc_v2.h"
#include "Core/System/mutex.h"
#include "Core/System/clanstring.h"
#include "Core/System/timer.h"
#include "Core/System/event_listener.h"
#include "Core/System/event_trigger.h"
#include "Core/System/console_window.h"

#include "Core/IOData/cl_endian.h"
#include "Core/IOData/inputsource.h"
#include "Core/IOData/inputsource_file.h"
#include "Core/IOData/inputsource_memory.h"
#include "Core/IOData/inputsource_provider.h"
#include "Core/IOData/inputsource_provider_file.h"
#include "Core/IOData/outputsource.h"
#include "Core/IOData/outputsource_file.h"
#include "Core/IOData/outputsource_memory.h"
#include "Core/IOData/outputsource_provider.h"
#include "Core/IOData/directory_scanner.h"

#include "Core/Resources/resource_manager.h"
#include "Core/Resources/resourcetype.h"
#include "Core/Resources/resource.h"
#include "Core/Resources/resourceoptions.h"
#include "Core/Resources/resourceoption.h"
#include "Core/Resources/resourcetype_boolean.h"
#include "Core/Resources/resourcetype_integer.h"
#include "Core/Resources/resourcetype_string.h"
#include "Core/Resources/resourcetype_raw.h"
#include "Core/Resources/datafile_compiler.h"

#include "Core/Math/cl_vector.h"
#include "Core/Math/vector2.h"
#include "Core/Math/bezier.h"
#include "Core/Math/math.h"
#include "Core/Math/rect.h"
#include "Core/Math/size.h"
#include "Core/Math/point.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanCore.lib")
#else
#pragma comment(lib, "clanCored.lib")
#endif
#endif


#endif
