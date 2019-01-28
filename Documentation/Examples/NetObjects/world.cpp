
#include "world.h"
#include "netobjects.h"

World::World(CL_NetSession *session)
: CL_World<GameObject>(session, NETCHANNEL_WORLD)
{
}
