
#ifndef header_client_wormhole
#define header_client_wormhole

#include "client_gameobject.h"

class Wormhole;
class SpriteData;
class Sprite;

class ClientWormhole : public ClientGameObject
{
// Construction:
public:
	ClientWormhole(
		ClientWorld *world,
		Wormhole *wormhole);

	virtual ~ClientWormhole();

// Attributes:
public:
	Wormhole *get_wormhole();

// Operations:
public:

// Overridables:
public:
	virtual void show();
	virtual void update(float time_elapsed);

// Implementation:
private:
	Sprite *sprite1;
	SpriteData *sprite_data1;
	Sprite *sprite2;
	SpriteData *sprite_data2;
};

#endif
