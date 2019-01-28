#ifndef header_gameobject
#define header_gameobject

#include <ClanLib/network.h>

class GameObject : public CL_GameObject<class World>
{
// Enums:
public:
	enum GameObjectType
	{
		FIGHTER = 0,
		PLANET = 1,
		WORMHOLE = 2
	};

// Construction:
public:
	GameObject(World *world, GameObjectType object_type);
	// Server constructor.

	GameObject(World *world, GameObjectType object_type, CL_NetObject netobj);
	// Client constructor.

	virtual ~GameObject();
};

#endif
