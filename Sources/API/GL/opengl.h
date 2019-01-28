/*
	$Id: opengl.h,v 1.12 2002/03/05 21:52:13 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanGL="OpenGL"
//! header=gl.h

#ifndef header_opengl
#define header_opengl

#include <string>

//: Extension procedure address typedef for OpenGL.
typedef void (CL_ProcAddress)();

//: OpenGL "main" class.
//- <p>CL_OpenGL is a class containing some functions that are general for
//- both OpenGL and ClanLib.</p>
class CL_OpenGL
{
public:
//! Operations:
	//: Begin 2d
  //- You can use standard ClanLib 2D graphics display code while using OpenGL.
  //- ClanLib will save and restore the state before and after each call.
  //- If you plan to call alot of 2D drawing code, call begin_2d(), do the drawing,
  //- then call end_2d().  This greatly reduces the overhead per call of 2D code.
	static void begin_2d();

	//: End 2d
  //- Ends the current 2D session started by begin_2d()
	static void end_2d();

	//: Enable 2d projection
	static void enable_2d_projection();

	//: Disable 2d projection
	static void disable_2d_projection();

	//: Set use gl
	static void set_use_gl( int v );

	//: Set buffer size
	static void set_buffer_size( int v );

	//: Set level
	static void set_level( int v );

	//: Set rgba
	static void set_rgba( int v );

	//: Set doublebuffer
	static void set_doublebuffer( int v );

	//: Set stereo
	static void set_stereo( int v );

	//: Set aux buffers
	static void set_aux_buffers( int v );

	//: Set red size
	static void set_red_size( int v );

	//: Set green size
	static void set_green_size( int v );

	//: Set blue size
	static void set_blue_size( int v );

	//: Set alpha size
	static void set_alpha_size( int v );

	//: Set depth size
	static void set_depth_size( int v );

	//: Set stencil size
	static void set_stencil_size( int v );

	//: Set accum red size
	static void set_accum_red_size( int v );

	//: Set accum green size
	static void set_accum_green_size( int v );

	//: Set accum blue size
	static void set_accum_blue_size( int v );

	//: Set accum alpha size
	static void set_accum_alpha_size( int v );

	//: Get OpenGL extension specific function address.
	static CL_ProcAddress *get_proc_address(const std::string &function_name);
};

#endif
