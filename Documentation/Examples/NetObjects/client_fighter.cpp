#include <ClanLib/display.h>

#include "sprite.h"
#include "spritedata.h"
#include "client_fighter.h"
#include "fighter.h"
#include "client_world.h"

/////////////////////////////////////////////////////////////////////////////
// ClientFighter construction:

ClientFighter::ClientFighter(
	ClientWorld *world,
	Fighter *fighter)
:
	ClientGameObject(world, fighter)
{
	ship_data = new SpriteData("Sprites/Ship", world->get_resources());
	ship = new Sprite(ship_data);
}

ClientFighter::~ClientFighter()
{
}

/////////////////////////////////////////////////////////////////////////////
// ClientFighter attributes:

Fighter *ClientFighter::get_fighter()
{
	return static_cast<Fighter *>(get_gameobject());
}

/////////////////////////////////////////////////////////////////////////////
// ClientFighter operations:

/////////////////////////////////////////////////////////////////////////////
// ClientFighter overridables:

void ClientFighter::show()
{
	Fighter *fighter = get_fighter();
	float x = fighter->get_position_x();
	float y = fighter->get_position_y();
	float angle = fighter->get_direction();
	ship->draw(x, y, angle);
}

void ClientFighter::update(float time_elapsed)
{
	Fighter *fighter = get_fighter();
	if (fighter->local_owner())
	{
		bool key_accel = CL_Keyboard::get_keycode(CL_KEY_UP);
		bool key_turn_left = CL_Keyboard::get_keycode(CL_KEY_LEFT);
		bool key_turn_right = CL_Keyboard::get_keycode(CL_KEY_RIGHT);

		fighter->set_keys(key_accel, key_turn_left, key_turn_right);
	}

	ship->update(time_elapsed);
}

/////////////////////////////////////////////////////////////////////////////
// ClientFighter implementation:
