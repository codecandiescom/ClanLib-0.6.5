
#ifndef header_client_fighter
#define header_client_fighter

#include "client_gameobject.h"

class Fighter;
class SpriteData;
class Sprite;

class ClientFighter : public ClientGameObject
{
// Construction:
public:
	ClientFighter(
		ClientWorld *world,
		Fighter *fighter);

	virtual ~ClientFighter();

// Attributes:
public:
	Fighter *get_fighter();

// Operations:
public:

// Overridables:
public:
	virtual void show();
	virtual void update(float time_elapsed);

// Implementation:
private:
	Sprite *ship;
	SpriteData *ship_data;
};

#endif
