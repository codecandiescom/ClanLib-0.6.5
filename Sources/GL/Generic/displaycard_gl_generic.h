/*
	$Id: displaycard_gl_generic.h,v 1.8 2002/02/16 16:10:41 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_displaycard_gl_generic
#define header_displaycard_gl_generic

#ifdef USE_OPENGL

#include "API/Display/Display/palette.h"
#include "Display/Display/Generic/displaycard_generic.h"

#include <stack>

// this is UGLY. someone do something about it some day.
// OH MY GOD! This hack may burn down your PC and kill your cat!
#ifdef USE_X11
#include "Display/Display/X11/display_xwindow.h"
class CL_GL_DisplayCard_Generic : public CL_XWindow_CompatibleCard
#elif defined WIN32
#include "Display/Display/DirectDraw/displaycard_win32compatible.h"
class CL_GL_DisplayCard_Generic : public CL_DisplayCard_Win32Compatible
#else
class CL_GL_DisplayCard_Generic : public CL_DisplayCard_Generic
#endif
{
public:
	CL_GL_DisplayCard_Generic(int card_no)
#ifdef USE_X11
	: CL_XWindow_CompatibleCard(card_no)
#elif defined WIN32
	: CL_DisplayCard_Win32Compatible(card_no)
#else
	: CL_DisplayCard_Generic(card_no)
#endif
	{
		uses_count = 0;
		translate_stack.push(std::pair<int, int>(0,0));
		use_2d_projection = true;
	}	

	virtual void set_palette(CL_Palette *palette);
	virtual CL_Palette *get_palette();

	virtual std::string get_name() { return "OpenGL Display"; }
	virtual int get_total_memory() { return -1; }
	virtual bool is_initialized() { return initialized; }
	virtual const std::list<CL_VidMode*> &get_videomodes();

	virtual CL_Target *get_target();
	virtual CL_Target *get_frontbuffer();

	// whether the app uses OpenGL or not
	virtual bool uses_gl() { return m_uses_gl; }
	virtual int get_max_texture_size() = 0;

	virtual void begin_2d();
	virtual void end_2d();
	virtual void enable_2d_projection();
	virtual void disable_2d_projection();

	// display 2d translate support:
	virtual void push_translate_offset();
	virtual void push_translate_offset(int x, int y);
	virtual int  get_translate_offset_x();
	virtual int  get_translate_offset_y();
	virtual void set_translate_offset(int x, int y);
	virtual void pop_translate_offset();

	// set attribute functions
	virtual void set_use_gl( int v ) { gl_config.use_gl = v; }
	virtual void set_buffer_size( int v ) { gl_config.buffer_size = v; }
	virtual void set_level( int v ) { gl_config.level = v; }
	virtual void set_rgba( int v ) { gl_config.rgba = v; }
	virtual void set_doublebuffer( int v ) { gl_config.doublebuffer = v; }
	virtual void set_stereo( int v ) { gl_config.stereo = v; }
	virtual void set_aux_buffers( int v ) { gl_config.aux_buffers = v; }
	virtual void set_red_size( int v ) { gl_config.red_size = v; }
	virtual void set_green_size( int v ) { gl_config.green_size = v; }
	virtual void set_blue_size( int v ) { gl_config.blue_size = v; }
	virtual void set_alpha_size( int v ) { gl_config.alpha_size = v; }
	virtual void set_depth_size( int v ) { gl_config.depth_size = v; }
	virtual void set_stencil_size( int v ) { gl_config.stencil_size = v; }
	virtual void set_accum_red_size( int v ) { gl_config.accum_red_size = v; }
	virtual void set_accum_green_size( int v ) { gl_config.accum_green_size = v; }
	virtual void set_accum_blue_size( int v ) { gl_config.blue_size = v; }
	virtual void set_accum_alpha_size( int v ) { gl_config.alpha_size = v; }
	
	virtual void (*get_proc_address(const std::string& function_name))()=0;
	
	// Accelerator functions:
	// ----------------------
	virtual void clear_display(
		float red,
		float green,
		float blue,
		float alpha);
	
	virtual void fill_rect(
		int x1,
		int y1,
		int x2,
		int y2,
		float r,
		float g,
		float b,
		float a);
	
	virtual void draw_rect(
		int x1,
		int y1,
		int x2,
		int y2,
		float r,
		float g,
		float b,
		float a);
	
	virtual void draw_line(
		int x1,
		int y1,
		int x2,
		int y2,
		float r,
		float g,
		float b,
		float a);

	virtual void sync_buffers();

protected:
		
	virtual CL_Blitters create_hw_blitters(CL_SurfaceProvider *provider);
	virtual CL_Blitters create_hw_dynamic_blitters(CL_SurfaceProvider *provider);
	
	bool initialized;
	bool m_uses_gl;
	int uses_count;
	bool use_2d_projection;

	struct 
	{
		int use_gl;
		int buffer_size;
		int level;
		int rgba;
		int doublebuffer;
		int stereo;
		int aux_buffers;
		int red_size;
		int green_size;
		int blue_size;
		int alpha_size;
		int depth_size;
		int stencil_size;
		int accum_red_size;
		int accum_green_size;
		int accum_blue_size;
		int accum_alpha_size;
	} gl_config;
	

private:
	CL_Palette palette;
	std::stack< std::pair<int, int> > translate_stack;
};

#endif

#endif
