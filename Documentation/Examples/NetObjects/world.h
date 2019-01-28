#ifndef header_world
#define header_world

#include "gameobject.h"
#include "players.h"

class World : public CL_World<GameObject>
{
// Enums:
public:
	enum netchannels
	{
		NETCHANNEL_WORLD = NETCHANNELS_WORLD,
	};

// Construction: (don't construct this object directly. Use client or server world.
protected:
	World(CL_NetSession *session);

// Attributes:
public:
	Players players;
};

#endif
