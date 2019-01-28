/* $Id: texture.h,v 1.2 2001/05/09 21:33:11 sphair Exp $ */

#ifndef header_texture_included
#define header_texture_included

#include <ClanLib/gl.h>

struct RGBA
{
	float rgba[4];
};

class Texture : public CL_Texture
{
public:
	Texture() {};
	~Texture();

	static Texture *load(const char *id, CL_ResourceManager *manager);

	void put_screen(
		int x,
		int y,
		float rotation = 0.0f,
		float scale = 1.0f,
		RGBA *rgba = NULL,
		int spr_no = 0);
};

#endif