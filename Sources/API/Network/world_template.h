/*
	$Id: world_template.h,v 1.22 2001/12/18 15:55:33 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanNetwork="World Template"
//! header=network.h

#ifndef header_world_template
#define header_world_template

#include "../Core/System/cl_assert.h"
#include "../Core/System/system.h"
#include "../Core/IOData/inputsource_memory.h"
#include "../Core/IOData/outputsource_memory.h"
#include "netobject_channel.h"
#include "netobject.h"

template<class World>
//: Game object template.
class CL_GameObject
{
public:
	//: Predefined message types:
	enum EGameObjectMessageTypes
	{
		msgtype_full_update = 0,
		msgtype_tick_update = 1,
		msgtype_destroy     = 2,
		msgtype_user        = 1000 // reserve first 1000 entries for clanlib.
	};

//! Construction:
public:
	//: Server constructor.
	CL_GameObject(World *world, int object_type)
	: world(world), object_type(object_type), netobject(&world->get_netobject_channel())
	{
		common_init();
	}

	//: Client construtor.
	CL_GameObject(World *world, int object_type, const CL_NetObject &netobject)
	: world(world), object_type(object_type), netobject(netobject)
	{
		common_init();
	}

	//: Game Object Destructor
	virtual ~CL_GameObject() { return; }

//! Attributes:
public:
	//: Get World
	World *get_world() { return world; }

	//: Get destroy flag
	bool get_destroy_flag() { return destroy_flag; }

	//: Get netobject
	CL_NetObject &get_netobject() { return netobject; }

	//: Get object type
	int get_object_type() { return object_type; }

//! Operations:
public:
	//: Set destroy flag
	void set_destroy_flag(bool new_value = true) { destroy_flag = new_value; }

	//: Set tick rate
	void set_tick_rate(float time_per_tick) { tick_rate = time_per_tick; }

	//: Send full
	void send_full(const CL_NetComputer *dest = NULL)
	{
		CL_OutputSource_Memory output;
		output.write_int32(object_type);
		write_full_update(output);
		netobject.send(/*dest,*/ msgtype_full_update, output.get_data());
	}

	//: Send tick
	void send_tick(const CL_NetComputer *dest = NULL)
	{
		CL_OutputSource_Memory output;
		write_tick_update(output);
		netobject.send(/*dest,*/ msgtype_tick_update, output.get_data());
	}

	//: Send destroy
	void send_destroy(const CL_NetComputer *dest = NULL)
	{
		CL_OutputSource_Memory output;
		write_destroy(output);
		netobject.send(/*dest,*/ msgtype_destroy, output.get_data());
	}

	//: Recv full
	void recv_full(CL_InputSource &message)
	{
		int type = message.read_int32();
		cl_assert(type == object_type);
		read_full_update(message);
	}

	//: Recv tick
	void recv_tick(CL_InputSource &message)
	{
		read_tick_update(message);
	}

	//: Recv destroy
	void recv_destroy(CL_InputSource &message)
	{
		read_destroy(message);
	}

//! Overrideables:
public:
	//: Update
	virtual void update(float time_elapsed)
	{
		if (tick_rate > 0.0f)
		{
			tick_time += time_elapsed;

			while (tick_time >= tick_rate)
			{
				tick_time -= tick_rate;
				if (netobject.is_server()) send_tick();
			}
		}
	}

	//: Read full update
	virtual void read_full_update(CL_InputSource &message) { return; }

	//: Read tick update
	virtual void read_tick_update(CL_InputSource &message) { return; }

	//: Read destroy
	virtual void read_destroy(CL_InputSource &message) { set_destroy_flag(); }

	//: Write full update
	virtual void write_full_update(CL_OutputSource &message) { return; }

	//: Write tick update
	virtual void write_tick_update(CL_OutputSource &message) { return; }

	//: Write destroy
	virtual void write_destroy(CL_InputSource &message) { return; }

//! Implementation:
public:
	//: Common init
	void common_init()
	{
		destroy_flag = false;
		tick_rate = 0.0f; // disable tick updates per default.
		tick_time = 0.0f;

		slot_full = netobject.connect(
			msgtype_full_update,
			this, &CL_GameObject<World>::recv_full);

		slot_tick = netobject.connect(
			msgtype_tick_update,
			this, &CL_GameObject<World>::recv_tick);

		slot_destroy = netobject.connect(
			msgtype_destroy,
			this, &CL_GameObject<World>::recv_destroy);
	}

	//: The world
	World *world;

	//: Object Type
	int object_type;

	//: Destroy_flag
	bool destroy_flag;

	//: Netobject
	CL_NetObject netobject;

	//: Tick_rate
	float tick_rate;

	//: Tick_time
	float tick_time;

	//: Slot_full
	CL_Slot slot_full;

	//: Slot_tick
	CL_Slot slot_tick;

	//: Slot_destroy
	CL_Slot slot_destroy;
};

template<class GameObject>
//: World template.
class CL_World
{
//! Construction:
public:
	//: World Constructor
	CL_World(CL_NetSession *session, int object_netchannel)
	:
		session(session),
		netobject_channel(session, object_netchannel)
	{
		slot_create_object = netobject_channel.sig_create_object().connect(
			this, &CL_World<GameObject>::sig_create_object);
	}

	//: World Destructor
	virtual ~CL_World()
	{
	}

//! Attributes:
public:
	//: Get netobject channel
	CL_NetObjectChannel &get_netobject_channel() { return netobject_channel; }

	//: Get session
	CL_NetSession *get_session() { return session; }

	//: Get objects
	std::list<GameObject *> &get_objects() { return objects; }

//! Operations:
public:
	//: Add object
	void add_object(GameObject *object) { objects.push_back(object); }

	//: Remove object
	void remove_object(GameObject *object) { objects.remove(object); }

	//: Update
	void update(float delta_time)
	{
		std::list<GameObject *>::iterator it;
		std::list<GameObject *> remove_list;
		for (it = objects.begin(); it != objects.end(); it++)
		{
			(*it)->update(delta_time);
			if ((*it)->get_destroy_flag()) remove_list.push_back(*it);
		}
			
		for (it = remove_list.begin(); it != remove_list.end(); it++)
		{
			delete *it;
		}
	}

//! Overrideables:
protected:
	virtual void on_create_object(
		const CL_NetObject &netobj,
		int msgType,
		const std::string &message) { return; }

//! Implementation:
private:
	void sig_create_object(
		const CL_NetObject &netobj,
		int msgType,
		const std::string &message)
	{
		// this function is needed because I'm not sure how to connect a signal to a virtual
		// table function.
		on_create_object(netobj, msgType, message);
	}

	std::list<GameObject *> objects;
	CL_NetSession *session;
	CL_NetObjectChannel netobject_channel;
	CL_Slot slot_create_object;
};

#endif
