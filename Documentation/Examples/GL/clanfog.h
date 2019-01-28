//------------------------------------------------------
//
// FOG By Johan 'Mace' GLysing		-		mace-x@usa.net
//
// Converted to ClanLib by Kenneth Gangstoe
//
//------------------------------------------------------

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

class FogApplication : public CL_ClanApplication
{
public:
	virtual char *get_title();

	virtual int main(int argc, char** argv);

private:
	void init();
	void init_display();
	void init_view(int width, int height);
	void init_gl();
	void init_lightpos();
	void init_fog();
	void init_textures();
	void init_display_lists();

	void run();
	void draw();
	void set_lights();
	void check_timers();

private:
	CL_ResourceManager *manager;

	CL_Texture *texture[3];
};
