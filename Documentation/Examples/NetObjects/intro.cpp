#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/sound.h>
#include "intro.h"
#include "texture.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

Intro::Intro(CL_ResourceManager *resources)
{
	font = new CL_Font("Intro/font", resources);

	background = Texture::load("Intro/background", resources);

	sample_write = new CL_SoundBuffer("Intro/Sounds/write", resources);
	sample_background = new CL_SoundBuffer("Intro/Sounds/background", resources);

	session_write = sample_write->prepare();
	session_background = sample_background->prepare(true);

	line[0] = "In the year 2045...";
	line[1] = "Technology has reached";
	line[2] = "a point where we fight wars";
	line[3] = "deep in space.";
	line[4] = "They call the fight...ehrm...";
	line[5] = "NetObjects Example.";

	current_time = 0;
	current_line = 0;
	current_pos = 0;

	state = 0;

	back_x = -512.0f;
	back_y = -384.0f;
	alpha = -0.2f;
}

Intro::~Intro()
{
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void Intro::run()
{
	bool quit = false;
	float last_time = CL_System::get_time();

	do 
	{
		if(CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
			quit = true;

		float delta_time = (CL_System::get_time() - last_time) / 1000.0f;
		last_time = CL_System::get_time();

		if(update(delta_time) == false)
			quit = true;
		else
			draw();

		CL_Display::flip_display();

		CL_System::keep_alive();
		CL_System::sleep(10);
	} while (quit == false);
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

bool Intro::update(float time_elapsed)
{
	current_time += time_elapsed;

	back_x += time_elapsed * 9;
	back_y += time_elapsed * 8; 

	alpha += 0.05f * time_elapsed;

	switch(state)
	{
	case 0:		// Waiting for a new line to start
		if(current_time > 1.0f)
		{
			current_time = 0.0f;
			state++;
		}
		break;
	case 1:		// Writing text
		while(current_time > 0.05f)
		{
			current_time -= 0.05f;
			current_pos++;

			if(session_write.is_playing() == false)
				if(current_pos < line[current_line].size() - 3)
					session_write.play();

			if(current_pos > line[current_line].size())
			{
				current_time = 0.0f;
				state++;
				break;
			}
		}
		break;
	case 2:		// Waiting for last line to show a while
		if(current_time > 3.0f)
		{
			current_time = 0.0f;
			current_pos = 0;
			current_line++;
			state = 0;
			if(current_line == 6)
				return false;
		}
		break;
	}

	return true;
}

void Intro::draw()
{
	session_background.play();

	RGBA col[4];
	for(int j=0; j<4; j++)
	{
		col[j].rgba[0] = 1.0f;
		col[j].rgba[1] = 1.0f;
		col[j].rgba[2] = 1.0f;
		col[j].rgba[3] = alpha;
	}

	CL_Display::clear_display(0.0f, 0.0f, 0.0f);
	background->put_screen(-back_x, -back_y, 0.0f, 1.0f, col);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	char cur[2] = {0};
	int x = 0;
	for(int i=0; i < current_pos; i++)
	{
		cur[0] = line[current_line][i];

		font->print_left(10 + x, 440, cur);
		x += font->get_char_width(cur[0]);
	}
}
