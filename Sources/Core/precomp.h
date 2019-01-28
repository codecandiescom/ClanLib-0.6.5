/*
	$Id: precomp.h,v 1.10 2001/12/14 02:39:57 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_precomp
#define header_precomp


#ifdef WIN32
#pragma warning (disable:4786)
#include <windows.h>
#include <process.h>
#include <ddraw.h>
#include <d3d.h>
#include <dinput.h>
#include <dsound.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <conio.h>
#endif

#include "API/signals.h"
#include "API/Core/System/error.h"

#ifdef __BORLANDC__
#define BAD_MATH
#endif

#ifdef BAD_MATH
#define sqrtf sqrt
#define acosf acos
#define sinf sin
#define cosf cos
#endif

#ifdef _DEBUG
#define DEBUG
#endif

#ifdef WIN32
#define BREAKPOINT
#else
#define BREAKPOINT asm("int $03");
#endif

#endif
