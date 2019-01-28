#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include "wormhole.h"

/////////////////////////////////////////////////////////////////////////////
// Wormhole construction:

Wormhole::Wormhole(World *world)
:
	GameObject(world, GameObject::WORMHOLE)
{
	init();
}

Wormhole::Wormhole(World *world, CL_NetObject netobj)
:
	GameObject(world, GameObject::WORMHOLE, netobj)
{
	init();
}

Wormhole::~Wormhole()
{
}

void Wormhole::init()
{
	angle = 0;
	speed = 20;
	position_x = 0;
	position_y = 0;
}

/////////////////////////////////////////////////////////////////////////////
// Wormhole attributes:

float Wormhole::get_position_x() const
{
	return position_x;
}

float Wormhole::get_position_y() const
{
	return position_y;
}

float Wormhole::get_direction() const
{
	return angle;
}

/////////////////////////////////////////////////////////////////////////////
// Wormhole operations:

void Wormhole::set_position_x(float pos)
{
	position_x = pos;
}

void Wormhole::set_position_y(float pos)
{
	position_y = pos;
}

void Wormhole::set_rotation_speed(float speed)
{
	this->speed = speed;
}

/////////////////////////////////////////////////////////////////////////////
// Wormhole overrideables:

void Wormhole::update(float time_elapsed)
{
	angle += speed * time_elapsed;
}

/////////////////////////////////////////////////////////////////////////////
// Wormhole implementation:
