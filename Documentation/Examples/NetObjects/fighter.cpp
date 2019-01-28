
#include "fighter.h"
#include "player.h"
#include "world.h"

/////////////////////////////////////////////////////////////////////////////
// Fighter construction:

Fighter::Fighter(World *world, Player *player)
:
	GameObject(world, GameObject::FIGHTER)
{
	init();
	owner = player;
}

Fighter::Fighter(World *world, CL_NetObject netobj)
:
	GameObject(world, GameObject::FIGHTER, netobj)
{
	init();
}

Fighter::~Fighter()
{
}

void Fighter::init()
{
	owner = 0;
	speed = 0;
	angle = 0;
	position_x = rand()%100;
	position_y = rand()%100;
	key_accel = false;
	key_turn_left = false;
	key_turn_right = false;

	set_tick_rate(0.05f); // 20 tick updates per second. (only does anything on server side)
	slot_read_talkback_position = netobject.connect(talkback_position, this, &Fighter::read_talkback_position);
}

/////////////////////////////////////////////////////////////////////////////
// Fighter attributes:

float Fighter::get_position_x() const
{
	return position_x;
}

float Fighter::get_position_y() const
{
	return position_y;
}

float Fighter::get_direction() const
{
	return angle;
}

float Fighter::get_speed() const
{
	return speed;
}

bool Fighter::local_owner() const
{
	if (owner == 0) return false;
	return owner->owner;
}

/////////////////////////////////////////////////////////////////////////////
// Fighter operations:

void Fighter::set_position_x(float pos)
{
	position_x = pos;
}

void Fighter::set_position_y(float pos)
{
	position_y = pos;
}

void Fighter::set_direction(float dir)
{
	angle = dir;
}

void Fighter::rotate(float delta_angle)
{
	angle += delta_angle;
}

void Fighter::set_speed(float speed)
{
	this->speed = speed;
}

void Fighter::set_keys(bool new_key_accel, bool new_key_turn_left, bool new_key_turn_right)
{
	bool keys_changed = 
		(key_accel != new_key_accel) ||
		(key_turn_left != new_key_turn_left) ||
		(key_turn_right != new_key_turn_right);

	key_accel = new_key_accel;
	key_turn_left = new_key_turn_left;
	key_turn_right = new_key_turn_right;

	if (keys_changed) send_talkback_position();
}

/////////////////////////////////////////////////////////////////////////////
// Fighter overrideables:

void Fighter::write_full_update(CL_OutputSource &message)
{
	message.write_int32(owner->ID);
	write_tick_update(message);
}

void Fighter::read_full_update(CL_InputSource &message)
{
	owner = get_world()->players.get_id(message.read_int32());
	read_tick_update(message);
}

void Fighter::write_tick_update(CL_OutputSource &message)
{
	message.write_float32(position_x);
	message.write_float32(position_y);
	message.write_float32(angle);
	message.write_float32(speed);
	message.write_bool8(key_accel);
	message.write_bool8(key_turn_left);
	message.write_bool8(key_turn_right);
}

void Fighter::read_tick_update(CL_InputSource &message)
{
	position_x = message.read_float32();
	position_y = message.read_float32();
	angle = message.read_float32();
	speed = message.read_float32();
	key_accel = message.read_bool8();
	key_turn_left = message.read_bool8();
	key_turn_right = message.read_bool8();
}

void Fighter::update(float time_elapsed)
{
	if (key_accel)
	{
		// mbn: this way of calculating acceleration will cause problems with different update cycle speeds.
		speed += 300.0f * time_elapsed;
	}
	if (key_turn_left)
	{
		rotate(-100.0f * time_elapsed);
	}
	if (key_turn_right)
	{
		rotate( 100.0f * time_elapsed);
	}

	#define ARAD 57.29577951308232

	float rad_angle = (90.0f - angle) / ARAD;
	float dx = cos(rad_angle);
	float dy = sin(rad_angle);

	float x_delta = speed * time_elapsed * dx;
	float y_delta = -speed * time_elapsed * dy;

	position_x += x_delta;
	position_y += y_delta;

	// HACK during development
	if(position_x > 660)
		position_x = -20;
	if(position_x < -20)
		position_x = 660;
	if(position_y > 500)
		position_y = -20;
	if(position_y < -20)
		position_y = 500;

	if(speed)
	{
		speed -= time_elapsed * 100.0f;
		if(speed < 0)
			speed = 0.0f;
	}

	// Call general update functionality (tickrate updates)
	GameObject::update(time_elapsed);
}

/////////////////////////////////////////////////////////////////////////////
// Fighter implementation:

void Fighter::read_talkback_position(const CL_NetComputer &computer, CL_InputSource &message)
{
	// Currently a position talkback from client controlling object is the
	// same as a tick update, just going from client to server.
	//
	// Therefore we just need to check if sender is really the controller (no hostile takeovers :)).

	if (computer != owner->computer) // Someone that wasnt allowed to control us sent a message!!
	{
		std::cout << "Someone attempted hostile takeover of Fighter object!!" << std::endl;
		return;
	}

	read_tick_update(message);
}

void Fighter::write_talkback_position(CL_OutputSource &message)
{
	// Current a position talkback is the same as a tick update, just going from client to server.
	write_tick_update(message);
}

void Fighter::send_talkback_position()
{
	CL_OutputSource_Memory output;
	write_talkback_position(output);
	netobject.talkback(talkback_position, output.get_data());
}

float calc_force(float mass1, float mass2, float distance)
{
	float constant = 6.6726f;
	return (constant * mass1 * mass2) / (distance * distance);
}

//http://www.angelfire.com/geek/alphabeta/

// mass of earth: 5.97 * 10^24 kg
// mass of spaceship = 20 ton ?

/*
for (split time_elapsed into 5 ms chunks)
{
  pos += speed;
  speed += calc_grav(pos, planet);
}
*/

// chris hecker, 6th dimension, 6d, game developer magazine
// 