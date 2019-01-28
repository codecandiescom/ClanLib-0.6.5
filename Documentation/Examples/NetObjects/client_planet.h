
#ifndef header_client_planet
#define header_client_planet

#include "client_gameobject.h"

class Planet;

class ClientPlanet : public ClientGameObject
{
// Construction:
public:
	ClientPlanet(
		ClientWorld *world,
		Planet *planet);

	virtual ~ClientPlanet();

// Attributes:
public:
	Planet *get_planet();

// Operations:
public:

// Overridables:
public:
	virtual void show();

// Implementation:
private:
};

#endif
