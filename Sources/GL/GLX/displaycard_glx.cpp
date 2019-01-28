/*
	$Id: displaycard_glx.cpp,v 1.9 2002/02/16 16:10:41 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifdef USE_OPENGL

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include "displaycard_glx.h"
#include "GL/Generic/blit_gl_generic.h"
#include "API/Core/System/cl_assert.h"
#include "API/Application/clanapp.h"
#include "../../Core/System/Unix/init_linux.h"
#include "Display/System/Unix/setupdisplay_unix.h"

CL_GLX_DisplayCard::CL_GLX_DisplayCard(
	Display *dpy,
	Window root,
	int card_no) :
CL_GL_DisplayCard_Generic(card_no), resolution(dpy, DefaultScreen(dpy))
{
	this->root = root;
	this->dpy = dpy;

	if (CL_Force_DispTarget::get_name()=="glx")
		m_uses_gl = true;
	else
		m_uses_gl = false;

	initialized = false;
	max_texture_size = 0;

	// set reasonable default values
	memset(&gl_config, 0, sizeof(gl_config));
	gl_config.buffer_size = 16;
	gl_config.depth_size = 16;
	gl_config.rgba = 1;
	gl_config.doublebuffer = 1;
}

CL_GLX_DisplayCard::~CL_GLX_DisplayCard()
{
	if (initialized)
	{
		glXDestroyContext(dpy, ctx);
		XDestroyWindow(dpy, win);
	}
}

void CL_GLX_DisplayCard::flip_display(bool sync)
{
	glXSwapBuffers(get_display(), get_window());
}

void CL_GLX_DisplayCard::put_display(const class CL_Rect &rect)
{
	cl_assert(false); // hmm... how do you do this under opengl? :)
}

void CL_GLX_DisplayCard::set_videomode(
	int width,
	int height,
	int bpp,
	bool fullscreen,
	bool allow_resize,
	bool video_memory)
{	
   	this->fullscreen = fullscreen;
	if (initialized) // clean up
	{
		//end_2d();
		glXDestroyContext(dpy, ctx);
		XDestroyWindow(dpy, win);
		initialized = false;
	}

	set_gfxmode(width, height, bpp, fullscreen, allow_resize);

	int dummy;
	if (!glXQueryExtension(dpy, &dummy, &dummy))
		cl_assert(false);

	static int gl_attribs[20];
	memset(&gl_attribs, None, 20*sizeof(int));

	// todo: figure out what GLX_LEVEL is

	int i = 0;
	if (gl_config.use_gl)
		gl_attribs[i++] = GLX_USE_GL;
	if (gl_config.rgba)
		gl_attribs[i++] = GLX_RGBA;
	if (gl_config.doublebuffer)
		gl_attribs[i++] = GLX_DOUBLEBUFFER;
	if (gl_config.stereo)
		gl_attribs[i++] = GLX_STEREO;
	if (gl_config.aux_buffers)
		gl_attribs[i++] = GLX_AUX_BUFFERS;

	if (gl_config.buffer_size)
	{
		gl_attribs[i++] = GLX_BUFFER_SIZE;
		gl_attribs[i++] = gl_config.buffer_size;
	}
	if (gl_config.red_size)
	{
		gl_attribs[i++] = GLX_RED_SIZE;
		gl_attribs[i++] = gl_config.red_size;
	}
	if (gl_config.green_size)
	{
		gl_attribs[i++] = GLX_GREEN_SIZE;
		gl_attribs[i++] = gl_config.green_size;
	}
	if (gl_config.blue_size)
	{
		gl_attribs[i++] = GLX_BLUE_SIZE;
		gl_attribs[i++] = gl_config.blue_size;
	}
	if (gl_config.alpha_size)
	{
		gl_attribs[i++] = GLX_ALPHA_SIZE;
		gl_attribs[i++] = gl_config.alpha_size;
	}
	if (gl_config.depth_size)
	{
		gl_attribs[i++] = GLX_DEPTH_SIZE;
		gl_attribs[i++] = gl_config.depth_size;
	}
	if (gl_config.stencil_size)
	{
		gl_attribs[i++] = GLX_STENCIL_SIZE;
		gl_attribs[i++] = gl_config.stencil_size;
	}
	if (gl_config.accum_red_size)
	{
		gl_attribs[i++] = GLX_ACCUM_RED_SIZE;
		gl_attribs[i++] = gl_config.accum_red_size;
	}
	if (gl_config.accum_green_size)
	{
		gl_attribs[i++] = GLX_ACCUM_GREEN_SIZE;
		gl_attribs[i++] = gl_config.accum_green_size;
	}
	if (gl_config.accum_blue_size)
	{
		gl_attribs[i++] = GLX_ACCUM_BLUE_SIZE;
		gl_attribs[i++] = gl_config.accum_blue_size;
	}
	if (gl_config.accum_alpha_size)
	{
		gl_attribs[i++] = GLX_ACCUM_ALPHA_SIZE;
		gl_attribs[i++] = gl_config.accum_alpha_size;
	}

	XVisualInfo *visual_info = glXChooseVisual(
		dpy,
		DefaultScreen(dpy),
		gl_attribs);

	cl_assert(visual_info != NULL);

	ctx = glXCreateContext(
		dpy,
		visual_info,
		None,
		GL_TRUE);

	cl_assert(ctx != NULL);

	Colormap cmap = XCreateColormap(
		dpy,
		RootWindow(dpy, visual_info->screen),
		visual_info->visual,
		AllocNone);

	XSetWindowAttributes attr;
	attr.colormap = cmap;
	attr.background_pixel = BlackPixel(dpy, DefaultScreen(dpy));
	attr.override_redirect = False;
	int attr_flags =
		CWOverrideRedirect |
		CWColormap |
		CWBackPixel;

	win_width = width;
	win_height = height;
	if (fullscreen)
	{
		resolution.set_mode(width, height);
		win_width = resolution.get_width();
		win_height = resolution.get_height();
		attr.override_redirect = True;
	}

	win = XCreateWindow(
		dpy,
		RootWindow(dpy, visual_info->screen),
		0,
		0,
		win_width,
		win_height,
		0,
		visual_info->depth,
		InputOutput,
		visual_info->visual,
		attr_flags,
		&attr);
	
	cl_assert(win);

	char *title = CL_ClanApplication::app->get_title();
	XTextProperty text_property;
	XStringListToTextProperty(&title, 1, &text_property);

	XSizeHints size_hints;
	size_hints.x = 0;
	size_hints.y = 0;
	size_hints.width = win_width;
	size_hints.height = win_height;
	size_hints.flags = PSize | PMinSize | PMaxSize;
	size_hints.min_width = win_width;
	size_hints.min_height = win_height;
	size_hints.max_width = win_width;
	size_hints.max_height = win_height;

	XSetWMProperties(
		dpy,
		win,
		&text_property,
		&text_property,
		0,
		0,
		&size_hints,
		0,
		0);

	glXMakeCurrent(dpy, win, ctx);
	XMapRaised(dpy, win);

	if (fullscreen)
	{
		XSetInputFocus(dpy, win, RevertToParent, CurrentTime);
		XGrabPointer(dpy, win, true, 0, GrabModeAsync, GrabModeAsync, win, None, CurrentTime);
	}

	// Listen for keystrokes...
	// ------------------------

	XSelectInput(dpy, win, KeyPressMask|KeyReleaseMask);

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
	
#ifdef DEBUG
	// Output OpenGL vendor & renderer
	// -------------------------------
	
	std::cout << std::endl;
	std::cout << "OpenGL status" << std::endl << std::endl;
	std::cout << "Vendor     : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer   : " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version    : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Extensions : " << glGetString(GL_EXTENSIONS) << std::endl << std::endl;

	int bufferSize, level, renderType, doubleBuffer, stereo, auxBuffers, redSize, greenSize;
	int blueSize, alphaSize, depthSize, stencilSize, acRedSize, acGreenSize, acBlueSize, acAlphaSize;

	glXGetConfig(dpy, visual_info, GLX_BUFFER_SIZE, &bufferSize);
	glXGetConfig(dpy, visual_info, GLX_LEVEL, &level);
	glXGetConfig(dpy, visual_info, GLX_RGBA, &renderType);
	glXGetConfig(dpy, visual_info, GLX_DOUBLEBUFFER, &doubleBuffer);
	glXGetConfig(dpy, visual_info, GLX_STEREO, &stereo);
	glXGetConfig(dpy, visual_info, GLX_AUX_BUFFERS, &auxBuffers);
	glXGetConfig(dpy, visual_info, GLX_RED_SIZE, &redSize);
	glXGetConfig(dpy, visual_info, GLX_GREEN_SIZE, &greenSize);
	glXGetConfig(dpy, visual_info, GLX_BLUE_SIZE, &blueSize);
	glXGetConfig(dpy, visual_info, GLX_ALPHA_SIZE, &alphaSize);
	glXGetConfig(dpy, visual_info, GLX_DEPTH_SIZE, &depthSize);
	glXGetConfig(dpy, visual_info, GLX_STENCIL_SIZE, &stencilSize);
	glXGetConfig(dpy, visual_info, GLX_ACCUM_RED_SIZE, &acRedSize);	
	glXGetConfig(dpy, visual_info, GLX_ACCUM_GREEN_SIZE, &acGreenSize);
	glXGetConfig(dpy, visual_info, GLX_ACCUM_BLUE_SIZE, &acBlueSize);
	glXGetConfig(dpy, visual_info, GLX_ACCUM_ALPHA_SIZE, &acAlphaSize);

	std::cout << "GLX status" << std::endl << std::endl;

	std::cout << " visual class: ";
	switch (visual_info->c_class)
	{
		case StaticGray	 : { std::cout << "StaticGray" << std::endl; break; }
		case GrayScale 	 : { std::cout << "GrayScale" << std::endl; break; }
		case StaticColor : { std::cout << "StaticColor" << std::endl; break; }
		case PseudoColor : { std::cout << "PseudoColor" << std::endl; break; }
		case TrueColor   : { std::cout << "TrueColor" << std::endl; break; }
		case DirectColor : { std::cout << "DirectColor" << std::endl; break; }
		default          : { std::cout << "unknown " << std::endl; }
	}

	std::cout << "visual depth: " << visual_info->depth << std::endl << std::endl;
	if (glXIsDirect(dpy, ctx))
		std::cout << "direct rendering" << std::endl;
	if (renderType)
		std::cout << "RGBA" << std::endl;
	if (doubleBuffer)
		std::cout << "double buffer" << std::endl;
	if (stereo)
		std::cout << "stereo buffer" << std::endl;
	std::cout << std::endl;

	std::cout << "GLX_BUFFER_SIZE      : " << bufferSize << std::endl;
	std::cout << "GLX_LEVEL            : " <<  level << std::endl;
//	std::cout << "GLX_RGBA             : " << renderType << std::endl;
//	std::cout << "GLX_DOUBELBUFFER     : " << doubleBuffer << std::endl;
//	std::cout << "GLX_STEREO           : " << stereo << std::endl;
	std::cout << "GLX_AUX_BUFFERS      : " << auxBuffers << std::endl;
	std::cout << "GLX_RED_SIZE         : " << redSize << std::endl;
	std::cout << "GLX_GREEN_SIZE       : " << greenSize << std::endl;
	std::cout << "GLX_BLUE_SIZE        : " << blueSize << std::endl;
	std::cout << "GLX_ALPHA_SIZE       : " << alphaSize << std::endl;
	std::cout << "GLX_DEPTH_SIZE       : " << depthSize << std::endl;
	std::cout << "GLX_STENCIL_SIZE     : " << stencilSize << std::endl;
	std::cout << "GLX_ACCUM_RED_SIZE   : " << acRedSize << std::endl;
	std::cout << "GLX_ACCUM_GREEN_SIZE : " << acGreenSize << std::endl;
	std::cout << "GLX_ACCUM_BLUE_SIZE  : " << acBlueSize << std::endl;
	std::cout << "GLX_ACCUM_ALPHA_SIZE : " << acAlphaSize << std::endl;
	std::cout << std::endl << std::endl;
	std::cout << "GL_MAX_TEXTURE_SIZE  : " << max_texture_size << std::endl;
	std::cout << std::endl << std::endl;

#endif

	// Setup OpenGL sanity:
	// --------------------
	if (!m_uses_gl) 
		begin_2d();

	initialized = true;
	clear_display(0.0, 0.0, 0.0, 1.0);
}

void CL_GLX_DisplayCard::keep_alive()
{
	XEvent event;
	
	for (int i=XPending(dpy); i>0; i--)
	{
		XNextEvent(dpy, &event);
		sig_xevent(event);
	}
}

void (*CL_GLX_DisplayCard::get_proc_address(const std::string& function_name))()
{
	return glXGetProcAddressARB((GLubyte*)function_name.c_str());
}



#endif
