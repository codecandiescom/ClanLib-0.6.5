#ifndef header_fighter
#define header_fighter

#include <ClanLib/core.h>

#include "gameobject.h"

class Player;

class Fighter : public GameObject
{
// Construction:
public:
	Fighter(World *world, Player *owner);
	// Server constructor.

	Fighter(World *world, CL_NetObject netobj);
	// Client constructor.

	virtual ~Fighter();

// Attributes:
public:
	float get_position_x() const;
	float get_position_y() const;
	float get_direction() const;
	float get_speed() const;
	bool local_owner() const;

// Operations:
public:
	void set_position_x(float pos);
	void set_position_y(float pos);
	void set_direction(float dir);
	void rotate(float degrees);
	void set_speed(float speed);
	void set_keys(bool keyAccel, bool keyTurnLeft, bool keyTurnRight);

//!Overrideables:
public:
	virtual void update(float time_elapsed);

	virtual void write_full_update(CL_OutputSource &message);
	virtual void read_full_update(CL_InputSource &message);
	virtual void write_tick_update(CL_OutputSource &message);
	virtual void read_tick_update(CL_InputSource &message);

// Implementation:
protected:
	void init();
	void read_talkback_position(const CL_NetComputer &computer, CL_InputSource &message);
	void write_talkback_position(CL_OutputSource &message);
	void send_talkback_position();

	enum
	{
		talkback_position = 0
	};

	float position_x;
	float position_y;
	float angle;
	float speed;

	bool key_accel;
	bool key_turn_left;
	bool key_turn_right;

	int last_update;

	Player *owner;

	CL_Slot slot_read_talkback_position;
};

#endif
