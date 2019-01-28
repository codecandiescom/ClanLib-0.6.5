#include "texture.h"

Texture *Texture::load(const char *id, CL_ResourceManager *manager)
{
	return (Texture *)CL_Texture::load(id, manager);
}

Texture::~Texture()
{
}

void Texture::put_screen(
	int x,
	int y,
	float rotation,
	float scale,
	RGBA *rgba,
	int spr_no)
{
	bind();

	int width = get_width();
	int height = get_height();

	int texture_width = 1;
	int texture_height = 1;
	while (texture_width < width) texture_width *= 2;
	while (texture_height < height * get_num_frames()) texture_height *= 2;

	float h1 = ((float) height * spr_no) / texture_height;
	float h2 = ((float) height * (spr_no + 1)) / texture_height;
	float w = (float)width / texture_width;

	glPushMatrix();
	glTranslatef((float)x, (float)y, 0.0f);
	glRotatef(rotation, 0, 0, 1);
	glScalef(scale, scale, 1.0f);
	glTranslatef(-width / 2.0f, -height / 2.0f, 0.0f);

	if(rgba) {
		glBegin(GL_TRIANGLE_STRIP);
			glColor4fv((float *)&(rgba[0])); glTexCoord2f(0.0, h1); glVertex2i(0, 0);
			glColor4fv((float *)&(rgba[1])); glTexCoord2f(  w, h1); glVertex2i(width, 0);
			glColor4fv((float *)&(rgba[2])); glTexCoord2f(0.0, h2); glVertex2i(0, height);
			glColor4fv((float *)&(rgba[3])); glTexCoord2f(  w, h2); glVertex2i(width, height);
		glEnd();
	}
	else {
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(0.0, h1); glVertex2i(0, 0);
			glTexCoord2f(  w, h1); glVertex2i(width, 0);
			glTexCoord2f(0.0, h2); glVertex2i(0, height);
			glTexCoord2f(  w, h2); glVertex2i(width, height);
		glEnd();
	}
	glPopMatrix();
}