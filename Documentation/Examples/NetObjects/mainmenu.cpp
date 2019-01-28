#ifdef WIN32
#pragma warning (disable:4355)
#endif

#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/network.h>

#include "mainmenu.h"
#include "optionsmenu.h"
#include "server_world.h"
#include "client_world.h"
#include "about_dialog.h"
#include "Lobby/lobby_dialog.h"
#include "Lobby/client_lobby.h"
#include "Lobby/server_lobby.h"

/////////////////////////////////////////////////////////////////////////////
// MainMenu construction:

MainMenu::MainMenu(CL_Component *parent, CL_StyleManager *style)
:
	CL_Frame(
		CL_Rect(0, 0, CL_Display::get_width(), CL_Display::get_height()),
		parent,
		style),
	button_create(CL_Rect(10, 10 + 75, 190, 45 + 75), "Create new game", this, style),
	button_join(CL_Rect(10, 50 + 75, 190, 85 + 75), "Join game", this, style),
	button_options(CL_Rect(10, 90 + 75, 190, 125 + 75), "Options", this, style),
	button_about(CL_Rect(10, 400, 190, 435), "About", this, style),
	button_quit(CL_Rect(10, 440, 190, 475), "Quit", this, style),
	server_name(CL_Rect(200, 50 + 75, 630, 85 + 75), "localhost", this, style)
{
	slot_paint = sig_paint().connect(this, &MainMenu::on_paint);
	slot_resize = CL_Display::sig_resized().connect(this, &MainMenu::on_resize);
	slot_create = button_create.sig_clicked().connect(this, &MainMenu::on_create);
	slot_join = button_join.sig_clicked().connect(this, &MainMenu::on_join);
	slot_options = button_options.sig_clicked().connect(this, &MainMenu::on_options);
	slot_about = button_about.sig_clicked().connect(this, &MainMenu::on_about);
	slot_quit = button_quit.sig_clicked().connect(this, &MainMenu::on_quit);

	title = new CL_Surface("Titles/mainmenu", style->get_resources());
//	ship = new CL_Surface("Titles/mainmenu_ship", style->get_resources());
}

MainMenu::~MainMenu()
{
}

/////////////////////////////////////////////////////////////////////////////
// MainMenu attributes:

/////////////////////////////////////////////////////////////////////////////
// MainMenu operations:

/////////////////////////////////////////////////////////////////////////////
// MainMenu implementation:

void MainMenu::on_paint()
{
	title->put_screen(20, 20);
//	ship->put_screen(286, 222);

	// todo: move this to header file, and dont make it dependant of the framerate!!
/*	static float cube_rotate = 78.5f;
	cube_rotate+=4;
	while (cube_rotate >= 360) cube_rotate -= 360;

	// Save opengl state:
	GLint old_viewport[4];
	int old_matrix_mode;
	glGetIntegerv(GL_VIEWPORT, old_viewport);
	glGetIntegerv(GL_MATRIX_MODE, &old_matrix_mode);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Change opengl rendering to be relative to inside our frame component:
	CL_Rect rect = get_screen_rect();
	glViewport(rect.x1, rect.y1, rect.get_width(), rect.get_height());

	// Setup projection matrix:
	float fov = 90.0f;
	float nearZ = 1.0f;
	float farZ = 20*100.0f;
	float ratio = rect.get_width() / (float) rect.get_height();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glTranslatef(0.7f, -0.7f, 0); // move center to the lower right.
	gluPerspective(
		fov,                   // field of view
		ratio,                 // aspect ratio
		nearZ,                 // near clipping plane
		farZ);                 // far clipping plane

	// Setup model view matrix:
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		0, 0, -17, // eye
		0, 0, 0,   // center
		0, 1, 0);  // up

	GLfloat light_position[] = { 1.0, 1.0, -1.0, 0.0 };
	GLfloat white_light[] = { 1.0, 1.0, -1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

	glRotatef(cube_rotate, -1, 1, 0.2f);

	// Draw cube:
	float vertices[3*8] =
	{
		-1,  1,  1, // vertex 0, top upper left
		 1,  1,  1, // vertex 1: top upper right
		 1,  1, -1, // vertex 2: top lower right
		-1,  1, -1, // vertex 3: top lower left
		-1, -1,  1, // vertex 4: bottom upper left
		 1, -1,  1, // vertex 5: bottom upper right
		 1, -1, -1, // vertex 6: bottom lower right
		-1, -1, -1, // vertex 7: bottom lower left
	};

	glBegin(GL_QUADS);

	glColor4f( 1, 1, 1, 0.5f);

	// draw top of cube:
	glNormal3f(0, 1, 0);
	glVertex3fv(&vertices[0*3]);
	glVertex3fv(&vertices[1*3]);
	glVertex3fv(&vertices[2*3]);
	glVertex3fv(&vertices[3*3]);

	// draw bottom of cube:
	glNormal3f(0, -1, 0);
	glVertex3fv(&vertices[7*3]);
	glVertex3fv(&vertices[6*3]);
	glVertex3fv(&vertices[5*3]);
	glVertex3fv(&vertices[4*3]);

	// draw left of cube:
	glNormal3f(-1, 0, 0);
	glVertex3fv(&vertices[3*3]);
	glVertex3fv(&vertices[7*3]);
	glVertex3fv(&vertices[4*3]);
	glVertex3fv(&vertices[0*3]);

	// draw right of cube:
	glNormal3f(1, 0, 0);
	glVertex3fv(&vertices[1*3]);
	glVertex3fv(&vertices[5*3]);
	glVertex3fv(&vertices[6*3]);
	glVertex3fv(&vertices[2*3]);

	// draw front of cube:
	glNormal3f(0, 0, 1);
	glVertex3fv(&vertices[4*3]);
	glVertex3fv(&vertices[5*3]);
	glVertex3fv(&vertices[1*3]);
	glVertex3fv(&vertices[0*3]);

	// draw back of cube:
	glNormal3f(0, 0, -1);
	glVertex3fv(&vertices[3*3]);
	glVertex3fv(&vertices[2*3]);
	glVertex3fv(&vertices[6*3]);
	glVertex3fv(&vertices[7*3]);

	glEnd();

	// Restore opengl state:
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(old_matrix_mode);
	glViewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);
*/
}

void MainMenu::on_resize(int width, int height)
{
}

void MainMenu::on_create()
{
	if(options.get_player_name() == "")
	{
		OptionsMenu optionsmenu(NULL, get_style_manager(), &options);
		optionsmenu.run(get_gui_manager());
		if(optionsmenu.get_result() == false)
			return;
	}
			
	CL_NetSession server(NETWORK_ID, NETWORK_PORT);
	CL_NetSession client(NETWORK_ID, server_name.get_text(), NETWORK_PORT);

	ServerLobby serverlobby(server);
	ClientLobby clientlobby(client, options);

	LobbyDialog lobby(NULL, get_style_manager());
	lobby.run(get_gui_manager(), &clientlobby, &serverlobby);

	while (lobby.get_result() == LobbyDialog::LOBBY_GO)
	{
		get_gui_manager()->disable_input();

		//CL_NetSession server(NETWORK_ID, NETWORK_PORT);
		//CL_NetSession client(NETWORK_ID, server_name.get_text(), NETWORK_PORT);

		ServerWorld server_world(&server, serverlobby.players);

		ClientWorld client_world(&client, &server_world);
		client_world.run();

		get_gui_manager()->enable_input();

		// back to the lobby
		lobby.run(get_gui_manager(), &clientlobby, &serverlobby);
	}
}

void MainMenu::on_join()
{
	if(options.get_player_name() == "")
	{
		OptionsMenu optionsmenu(NULL, get_style_manager(), &options);
		optionsmenu.run(get_gui_manager());
		if(optionsmenu.get_result() == false)
			return;
	}

	try
	{
		CL_NetSession client(NETWORK_ID, server_name.get_text(), NETWORK_PORT);
		ClientLobby clientlobby(client, options);

		LobbyDialog lobby(NULL, get_style_manager());
		lobby.run(get_gui_manager(), &clientlobby);

		while (lobby.get_result() == LobbyDialog::LOBBY_GO)
		{
			get_gui_manager()->disable_input();

			ClientWorld world(&client);
			world.run();
		
			get_gui_manager()->enable_input();
			
			lobby.run(get_gui_manager(), &clientlobby);
		}
	}
	catch(CL_Error err)
	{
		get_gui_manager()->enable_input();
		CL_MessageBox::info("Error", err.message, this);
	}
}

void MainMenu::on_options()
{
	OptionsMenu optionsmenu(NULL, get_style_manager(), &options);
	optionsmenu.run(get_gui_manager());
}

void MainMenu::on_about()
{
	AboutDialog dlg(get_style_manager());
	dlg.run(get_gui_manager());
}

void MainMenu::on_quit()
{
	int result = CL_MessageBox::info(
		"Quit ?",
		"Are you sure you want to quit ?",
		"Yes",
		"No",
		"",
		get_gui_manager()
	);

	if(result == 0)
		quit();
}
