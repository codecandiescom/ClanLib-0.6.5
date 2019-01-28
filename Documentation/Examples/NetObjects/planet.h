
#ifndef header_planet
#define header_planet

#include "gameobject.h"

class Planet : public GameObject
{
// Construction:
public:
	Planet(World *world);
	// Server constructor.

	Planet(World *world, CL_NetObject netobj);
	// Client constructor.

	virtual ~Planet();

// Attributes:
public:

// Operations:
public:

// Implementation:
private:
};

#endif
