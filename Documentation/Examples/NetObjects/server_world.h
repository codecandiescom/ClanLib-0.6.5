#ifndef header_server_world
#define header_server_world

#include "world.h"

class LobbyPlayers;

class ServerWorld : public World
{
// Construction:
public:
	ServerWorld(CL_NetSession *session, const LobbyPlayers &players);

	~ServerWorld();

// Attributes:
public:

// Operations:
public:
	void dedicated();

	virtual void update(float delta_time);

// Implementation:
private:
	float max_tick_elapsed;
};

#endif
