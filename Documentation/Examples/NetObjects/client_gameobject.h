
#ifndef header_clientgameobject
#define header_clientgameobject

class ClientWorld;
class GameObject;

class ClientGameObject
{
// Construction:
public:
	ClientGameObject(
		ClientWorld *world,
		GameObject *gameobject);

	virtual ~ClientGameObject();

// Attributes:
public:
	ClientWorld *get_world();

	GameObject *get_gameobject();

// Operations:
public:

// Overrideables:
public:
	virtual void show() = 0;
	virtual void update(float delta_time) {};

// Implementation:
private:
	ClientWorld *world;
	GameObject *gameobject;
};

#endif
