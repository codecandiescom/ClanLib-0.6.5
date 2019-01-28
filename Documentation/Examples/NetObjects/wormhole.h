#ifndef header_wormhole
#define header_wormhole

#include <ClanLib/core.h>

#include "gameobject.h"

class Wormhole : public GameObject
{
// Construction:
public:
	Wormhole(World *world);
	// Server constructor.

	Wormhole(World *world, CL_NetObject netobj);
	// Client constructor.

	virtual ~Wormhole();

// Attributes:
public:
	float get_position_x() const;
	float get_position_y() const;
	float get_direction() const;

// Operations:
public:
	void set_position_x(float pos);
	void set_position_y(float pos);

	void set_rotation_speed(float speed);

//!Overrideables:
public:
	virtual void update(float time_elapsed);

// Implementation:
private:
	void init();

	float position_x;
	float position_y;
	float angle;
	float speed;
};

#endif
