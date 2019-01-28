
#include "gameobject.h"
#include "world.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject construction:

GameObject::GameObject(
	World *world,
	GameObjectType object_type)
:
	CL_GameObject<World>(world, object_type)
{
}

GameObject::GameObject(
	World *world,
	GameObjectType object_type,
	CL_NetObject netobj)
:
	CL_GameObject<World>(world, object_type, netobj)
{
}

GameObject::~GameObject()
{
}
