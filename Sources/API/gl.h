/*
	$Id: gl.h,v 1.9 2002/03/01 22:00:16 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//: <p>ClanLib OpenGL support. Contains OpenGL binding code and 3D helper
//: frameworks.</p>
//! Global=GL

#ifndef header_gl
#define header_gl

#ifdef WIN32
	#pragma warning (disable:4786)

	#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include "GL/setupgl.h"
#include "GL/opengl.h"
#include "GL/texture.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanGL.lib")
#else
#pragma comment(lib, "clanGLd.lib")
#endif
#endif

#endif
