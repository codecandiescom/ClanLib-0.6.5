/*
	$Id: texture_gl.h,v 1.4 2001/02/17 17:12:10 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_texture_opengl
#define header_texture_opengl

#ifdef WIN32
	#include <windows.h>
#endif
#include "API/GL/texture.h"
#include <GL/gl.h>
#include <GL/glu.h>

class CL_Texture_OpenGL : public CL_Texture
{
public:
	CL_Texture_OpenGL(CL_SurfaceProvider *provider, bool delete_provider);
	virtual ~CL_Texture_OpenGL();

	// CL_Texture implementation
	virtual void bind(int texture_no=0);
	virtual CL_SurfaceProvider *get_provider() const;
	virtual void reload(); 
	virtual int get_width() const;
	virtual int get_height() const;
	virtual int get_num_textures() const;
	virtual int get_num_frames() const;
	virtual void flush(CL_DisplayCard *card = NULL);

private:
	CL_SurfaceProvider *provider;
	bool delete_provider;

	GLuint *handles;
	int num_textures;
};

#endif
