#include <ClanLib/display.h>

#include "sprite.h"
#include "spritedata.h"
#include "client_wormhole.h"
#include "wormhole.h"
#include "client_world.h"

/////////////////////////////////////////////////////////////////////////////
// ClientWormhole construction:

ClientWormhole::ClientWormhole(
	ClientWorld *world,
	Wormhole *wormhole)
:
	ClientGameObject(world, wormhole)
{
	sprite_data1 = new SpriteData("Sprites/Wormhole1", world->get_resources());
	sprite1 = new Sprite(sprite_data1);
	sprite_data2 = new SpriteData("Sprites/Wormhole2", world->get_resources());
	sprite2 = new Sprite(sprite_data2);

	// Debug:
	get_wormhole()->set_position_x(100);
	get_wormhole()->set_position_y(100);
}

ClientWormhole::~ClientWormhole()
{
}

/////////////////////////////////////////////////////////////////////////////
// ClientWormhole attributes:

Wormhole *ClientWormhole::get_wormhole()
{
	return static_cast<Wormhole *>(get_gameobject());
}

/////////////////////////////////////////////////////////////////////////////
// ClientWormhole operations:

/////////////////////////////////////////////////////////////////////////////
// ClientWormhole overridables:

void ClientWormhole::show()
{
	Wormhole *wormhole = get_wormhole();
	float x = wormhole->get_position_x();
	float y = wormhole->get_position_y();
	float angle = wormhole->get_direction();

	sprite1->draw(x, y, angle);
	sprite2->draw(x, y, 360 - angle);
}

void ClientWormhole::update(float time_elapsed)
{
	Wormhole *wormhole = get_wormhole();

	sprite1->update(time_elapsed);
	sprite2->update(time_elapsed);
	wormhole->update(time_elapsed);
}

/////////////////////////////////////////////////////////////////////////////
// ClientWormhole implementation:
