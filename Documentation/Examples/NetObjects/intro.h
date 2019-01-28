/* $Id: intro.h,v 1.3 2001/10/27 16:22:33 sphair Exp $ */

#ifndef header_intro_included
#define header_intro_included

class CL_ResourceManager;
class CL_Font;
class Texture;

class Intro
{
public:
	Intro(CL_ResourceManager *resources);
	~Intro();

	void run();

private:
	CL_Font *font;
	Texture *background;
	CL_SoundBuffer *sample_write;
	CL_SoundBuffer_Session session_write;
	CL_SoundBuffer *sample_background;
	CL_SoundBuffer_Session session_background;

	bool update(float time_elapsed);
	void draw();

	float back_x;
	float back_y;
	float alpha;

	std::string line[10];
	int current_line;
	int current_pos;

	int state;

	float current_time;
};

#endif