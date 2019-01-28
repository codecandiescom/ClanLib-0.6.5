/*
	$Id: texture.h,v 1.14 2001/12/11 21:12:06 japj Exp $

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

#ifndef header_texture
#define header_texture

class CL_SurfaceProvider;
class CL_ResourceManager;
class CL_DisplayCard;

#include <string>

//: Texture loading class for OpenGL.
//- <p>This class can load the image(s) from surface provider into OpenGL
//- textures.</p>
//-
//- <p>This class works just like a CL_Surface, except that it
//- requires the image width to be 2^n.</p>
class CL_Texture
{
public:
//! Construction:
	//: Creates a texture from a surface provider.
	//- provider - Surface provider to use as image source.
	//- delete_provider - If true, surface provider will be deleted
	//-                   when the texture is deleted.
	static CL_Texture *create(CL_SurfaceProvider *provider, bool delete_provider=false);

	//: Load the texture from a surface resource.
	//- id - Resource ID of the surface resource.
	//- manager - Resource manager used to load the resource from.
	static CL_Texture *load(const std::string &id, CL_ResourceManager *manager);

	//: Texture Destructor
	virtual ~CL_Texture() { ; }

//! Attributes:
	//: Returns the surface provider.
	virtual CL_SurfaceProvider *get_provider() const = 0;

	//: Forces surface to reload texture data from provider.
	virtual void reload() = 0;

	//: Returns the width of the surface
	//- Returns - Width of the texture.
	virtual int get_width() const = 0;

	//: Returns the height of the texture
	//- Returns - Height of the texture.
	virtual int get_height() const = 0;

	//: Returns the number of subtextures in the texture
	//- Returns - Number of subtextures in texture.
	virtual int get_num_frames() const = 0;

//! Operations:
	//: Bind the texture in OpenGL.
	//- texture_no - Subframe in the provider to bind.
	virtual void bind(int texture_no=0) = 0;

	//: completely flushes textures (removes from video/system-memory)
	virtual void flush(CL_DisplayCard *card = NULL) = 0;
};

#endif
