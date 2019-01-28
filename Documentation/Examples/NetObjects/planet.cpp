
#include "planet.h"

/////////////////////////////////////////////////////////////////////////////
// Fighter construction:

Planet::Planet(World *world)
:
	GameObject(world, GameObject::PLANET)
{
}

Planet::Planet(World *world, CL_NetObject netobj)
:
	GameObject(world, GameObject::PLANET, netobj)
{
}

Planet::~Planet()
{
}

/////////////////////////////////////////////////////////////////////////////
// Fighter attributes:

/////////////////////////////////////////////////////////////////////////////
// Fighter operations:

/////////////////////////////////////////////////////////////////////////////
// Fighter implementation:
