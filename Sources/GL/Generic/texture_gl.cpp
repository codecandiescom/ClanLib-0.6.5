/*
	$Id: texture_gl.cpp,v 1.8 2002/02/26 22:41:41 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "texture_gl.h"
#include "API/Display/Display/pixeldata.h"

CL_Texture *CL_Texture::create(CL_SurfaceProvider *prov, bool delete_prov)
{
	return new CL_Texture_OpenGL(prov, delete_prov);
}

CL_Texture *CL_Texture::load(const std::string &id, CL_ResourceManager *manager)
{
	return new CL_Texture_OpenGL(
		CL_SurfaceProvider::load(id, manager),
		false); // this should be true, but...
}

CL_Texture_OpenGL::CL_Texture_OpenGL(CL_SurfaceProvider *provider, bool delete_provider)
{
	this->provider = provider;
	this->delete_provider = delete_provider;

	handles = NULL;
	num_textures = 0;
	reload();
}

CL_Texture_OpenGL::~CL_Texture_OpenGL()
{
	if (delete_provider) delete provider;

	flush();
}

void CL_Texture_OpenGL::bind(int texture_no)
{
	if (texture_no < 0 || texture_no >= num_textures)
	{
		throw CL_Error("Illegal subtexture selected");
	}

	if (handles == NULL) reload();

	glBindTexture(GL_TEXTURE_2D, handles[texture_no]);
}

CL_SurfaceProvider *CL_Texture_OpenGL::get_provider() const
{
	return provider;
}

void CL_Texture_OpenGL::reload()
{
	provider->lock();

	if (handles != NULL && provider->get_num_frames() != (unsigned int) num_textures)
	{
		glDeleteTextures(num_textures, handles);
		delete[] handles;
		handles = NULL;
	}

	num_textures = provider->get_num_frames();

	if (handles == NULL)
	{
		handles = new GLuint[num_textures];
		glGenTextures(num_textures, handles);
	}

	int width = provider->get_width();
	int height = provider->get_height();

	int texture_width = 1;
	int texture_height = 1;
	while (texture_width < width) texture_width *= 2;
	while (texture_height < height * num_textures) texture_height *= 2;

	cl_assert(width <= texture_width);
	cl_assert(height <= texture_height);

	CL_PixelData pixeldata(
		255,
		255 << 8,
		255 << 16,
		255 << 24,
		provider,
		4);

	unsigned int *texture_data = new unsigned int[texture_width * texture_height];
	memset(texture_data, 0, texture_width * texture_height * sizeof(int));

	// Convert texture to correct pixelformat before passing it to OpenGL:	
	for (int i = 0; i < num_textures; i++)
	{
		if (provider->get_alpha_mask() == 0)
		{
			for (int y = 0; y < height * num_textures; y++)
			{
				unsigned int* src = (unsigned int*) pixeldata.get_line_pixel(y + height * i);
				unsigned int* dst = (unsigned int*) &texture_data[texture_width * y];
				for (int x = 0; x < width; x++)
				{
					*(dst++) = (*src & 0x00FFFFFF) | (~(*src) & 0xFF000000);
					src++;
				}
			}
		}
		else
		{
			for (int y = 0; y < height * num_textures; y++)
			{	
				memcpy(
					&texture_data[texture_width * y],
					pixeldata.get_line_pixel(y + height * i),
					width * sizeof(int));
			}
		}
		
		glBindTexture(GL_TEXTURE_2D, handles[i]);
	
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			texture_width,
			texture_height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			texture_data);

/*		gluBuild2DMipmaps(
			GL_TEXTURE_2D,
			GL_RGBA,
			texture_width,
			texture_height,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			texture_data);
*/	}

	delete[] texture_data;

	provider->unlock();
}

/*void CL_Texture_OpenGL::reload()
{
	provider->lock();

	if (handles != NULL && provider->get_num_frames() != (unsigned int) num_textures)
	{
		glDeleteTextures(num_textures, handles);
		delete[] handles;
		handles = NULL;
	}

	num_textures = provider->get_num_frames();

	if (handles == NULL)
	{
		handles = new GLuint[num_textures];
		glGenTextures(num_textures, handles);
	}

	int width = provider->get_width();
	int height = provider->get_height();

	int texture_width = 1;
	int texture_height = 1;
	while (texture_width < width) texture_width *= 2;
	while (texture_height < height * no_sprs) texture_height *= 2;

	cl_assert(width <= texture_width);
	cl_assert(height <= texture_height);

	CL_PixelData pixeldata(
		255,
		255 << 8,
		255 << 16,
		255 << 24,
		provider,
		4);

	unsigned int *texture_data = new unsigned int[width*height];

	// Convert texture to correct pixelformat before passing it to OpenGL:	
	for (int i=0;i<num_textures;i++)
	{
		if (provider->get_alpha_mask() == 0)
		{
			for (int y=0; y<height; y++)
			{
				unsigned int* src = (unsigned int*) pixeldata.get_line_pixel(y+height*i);
				unsigned int* dst = (unsigned int*) &texture_data[width*y];
				for (int x=0; x< width; x++)
				{
					*(dst++) = (*src & 0x00FFFFFF) | (~(*src) & 0xFF000000);
					src++;
				}
			}
		}
		else
		{
			for (int y=0; y<height; y++)
			{	
				memcpy(
					&texture_data[width*y],
					pixeldata.get_line_pixel(y+height*i),
					width * sizeof(int));
			}
		}
		
		glBindTexture(GL_TEXTURE_2D, handles[i]);

		gluBuild2DMipmaps(
			GL_TEXTURE_2D,
			4,
			width,
			height,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			texture_data);

	}

	delete[] texture_data;

	provider->unlock();
}
*/
int CL_Texture_OpenGL::get_width() const
{
	return provider->get_width();
}

int CL_Texture_OpenGL::get_height() const
{
	return provider->get_height();
}

int CL_Texture_OpenGL::get_num_frames() const
{
	return provider->get_num_frames();
}

int CL_Texture_OpenGL::get_num_textures() const
{
	return num_textures; 
}

void CL_Texture_OpenGL::flush(CL_DisplayCard *card)
{
	if (handles != NULL)
	{
		glDeleteTextures(num_textures, handles);
		delete[] handles;
		handles = NULL;
	}
}
