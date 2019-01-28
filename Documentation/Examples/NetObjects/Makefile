EXAMPLE_BIN=netobjects

include ../Makefile.conf

VPATH = ./:Lobby:GUI

OBJF =	Intermediate/about_dialog.o \
	Intermediate/button_opengl.o \
	Intermediate/checkbox_opengl.o \
	Intermediate/client_fighter.o \
	Intermediate/client_gameobject.o \
	Intermediate/client_planet.o \
	Intermediate/client_world.o \
	Intermediate/client_wormhole.o \
	Intermediate/fighter.o \
	Intermediate/frame_opengl.o \
	Intermediate/gameobject.o \
	Intermediate/inputbox_opengl.o \
	Intermediate/intro.o \
	Intermediate/listbox_opengl.o \
	Intermediate/client_lobby.o \
	Intermediate/lobby_dialog.o \
	Intermediate/lobby_players.o \
	Intermediate/server_lobby.o \
	Intermediate/mainmenu.o \
	Intermediate/options.o \
	Intermediate/optionsmenu.o \
	Intermediate/netobjects.o \
	Intermediate/planet.o \
	Intermediate/player.o \
	Intermediate/players.o \
	Intermediate/scrollbar_opengl.o \
	Intermediate/server_world.o \
	Intermediate/sprite.o \
	Intermediate/spritedata.o \
	Intermediate/stylemanager_opengl.o \
	Intermediate/texture.o \
	Intermediate/window_opengl.o \
	Intermediate/world.o \
	Intermediate/wormhole.o


netobjects: $(OBJF)
	$(CXX) ${CXXFLAGS} $(OBJF) -o netobjects_demo -lclanCore -lclanApp -lclanDisplay -lclanNetwork -lclanSound -lclanGL -lclanGUI -lclanPNG -lclanJPEG -ldl
