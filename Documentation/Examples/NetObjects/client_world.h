
#ifndef header_client_world
#define header_client_world

#include "world.h"

class ServerWorld;
class ClientGameObject;
class CL_ResourceManager;

class ClientWorld : public World
{
// Construction:
public:
	ClientWorld(CL_NetSession *session, ServerWorld *server = NULL);
	~ClientWorld();

// Attributes:
public:
	CL_ResourceManager *get_resources();

// Operations:
public:
	void run();

// Implementation:
private:
	virtual void on_create_object(
		const CL_NetObject &netobj,
		int msgType,
		const std::string &message);

	ServerWorld *server;
	std::list<ClientGameObject *> client_gameobjects;

	CL_ResourceManager *resources;
	CL_Surface *background;

	float max_tick_elapsed;
	unsigned long last_time;
};

#endif
