
#include "client_gameobject.h"

/////////////////////////////////////////////////////////////////////////////
// ClientGameObject construction:

ClientGameObject::ClientGameObject(
	ClientWorld *world,
	GameObject *gameobject)
:
	world(world),
	gameobject(gameobject)
{
}

ClientGameObject::~ClientGameObject()
{
}

/////////////////////////////////////////////////////////////////////////////
// ClientGameObject attributes:

ClientWorld *ClientGameObject::get_world()
{
	return world;
}

GameObject *ClientGameObject::get_gameobject()
{
	return gameobject;
}

/////////////////////////////////////////////////////////////////////////////
// ClientGameObject operations:

/////////////////////////////////////////////////////////////////////////////
// ClientGameObject implementation:

