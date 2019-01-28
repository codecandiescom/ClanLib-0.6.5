#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "client_world.h"
#include "client_gameobject.h"
#include "client_fighter.h"
#include "client_wormhole.h"
#include "server_world.h"
#include "fighter.h"
#include "wormhole.h"

/////////////////////////////////////////////////////////////////////////////
// ClientWorld construction:

ClientWorld::ClientWorld(CL_NetSession *session, ServerWorld *server)
:
	World(session),
	server(server),
	max_tick_elapsed(0.05f)
{
	// TODO: use the gui-resource-manager
	resources = new CL_ResourceManager("resources.scr", false);
	background = new CL_Surface("Backgrounds/nebula1", resources);
}

ClientWorld::~ClientWorld()
{
	delete resources;
}

/////////////////////////////////////////////////////////////////////////////
// ClientWorld attributes:

CL_ResourceManager *ClientWorld::get_resources()
{
	return resources;
}

/////////////////////////////////////////////////////////////////////////////
// ClientWorld operations:

void ClientWorld::run()
{
	float last_time = CL_System::get_time();

	while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
	{
		// Update world:
		float delta_time = (CL_System::get_time() - last_time) / 1000.0f;
		last_time = CL_System::get_time();

		for (; delta_time > 0; delta_time -= max_tick_elapsed)
		{
			float elapsed = (delta_time > max_tick_elapsed) ? max_tick_elapsed : delta_time;

			if (server)
				server->update(elapsed);

			players.update(get_session());

			World::update(elapsed);

			std::list<ClientGameObject *>::iterator it;
			std::list<ClientGameObject *> remove_list;
			for (it = client_gameobjects.begin(); it != client_gameobjects.end(); it++)
			{
				(*it)->update(elapsed);
//				if ((*it)->get_destroy_flag()) remove_list.push_back(*it);
			}
		
			for (it = remove_list.begin(); it != remove_list.end(); it++)
			{
				delete *it;
			}
		}

		// Show world:
		background->put_screen(0,0);
		std::list<ClientGameObject *>::iterator it;
		for (it = client_gameobjects.begin(); it != client_gameobjects.end(); it++)
		{
			ClientGameObject *cur = *it;
			cur->show();
		}

		CL_Display::flip_display();

		CL_System::keep_alive();
	}
}

/////////////////////////////////////////////////////////////////////////////
// ClientWorld implementation:

void ClientWorld::on_create_object(
	const CL_NetObject &netobj,
	int msgType,
	const std::string &message)
{
	// can only create object if the update is a full update.
	if (msgType != GameObject::msgtype_full_update) return;

	// Solve chicken'n'egg problem. We want to be sure that player descriptions have
	// arrived before constructing objects for the same players. I wish this could be
	// done a little bit more nicely, but this will do the trick. -- mbn
	players.update(get_session());

	// read header of a full update message:
	CL_InputSource_Memory input(message);
	int object_type = input.read_int32();

	// and use that information to find out what object to create:
	GameObject *obj = NULL;
	ClientGameObject *client_obj = NULL;

	switch (object_type)
	{
	case GameObject::FIGHTER:
		client_obj = new ClientFighter(this, new Fighter(this, netobj));
		break;
	default:
		cl_assert(false); // unknown object.
	};

	if (obj == NULL && client_obj != NULL) obj = client_obj->get_gameobject();

	// ask the object to serialize itself:
	CL_InputSource_Memory dummy(message);
	obj->recv_full(dummy);
 
	// add it to the world:
	add_object(obj);
	if (client_obj != NULL) client_gameobjects.push_back(client_obj);
}
