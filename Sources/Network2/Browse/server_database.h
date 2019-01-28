
#ifndef header_server_database
#define header_server_database

#include "API/Core/System/mutex.h"
#include "API/Network2/Socket/ip_address.h"

#include <string>
#include <deque>
#include <list>
#include <map>

class CL_ServerDescription
{
public:
	std::string desc;
	int ref_count;
};

class CL_ServerUpdate
{
public:
	unsigned int timestamp;
	CL_IPAddress server;
};

class CL_ServerChanges
{
public:
	unsigned int timestamp;
	std::map<CL_IPAddress, CL_ServerDescription *> servers;
	std::list<CL_IPAddress> removed_servers;
};

class CL_ServerDatabase
{
//! Construction:
public:
	//: Constructs a server database.
	CL_ServerDatabase();

	~CL_ServerDatabase();

//! Attributes:
public:

//! Operations:
public:
	//: Increments reference count.
	int add_ref();

	//: Decrements reference count.
	//- <p>When the reference count reaches zero, release_ref will delete the instance.</p>
	int release_ref();

	//: Adds/updates a server in the database.
	void update_server(const CL_IPAddress &address, const std::string &desc);

	//: Removes a server from the database.
	void remove_server(const CL_IPAddress &address);

	//: Returns a list of server changes that has happened since last timestamp.
	CL_ServerChanges get_changes(unsigned int timestamp = 0);

	//: Releases reference counts caused by a call to get_changes.
	void release_changes(CL_ServerChanges &changes);

//! Implementation:
private:
	int ref_count;

	std::map<CL_IPAddress, CL_ServerDescription *> servers;

	std::deque<CL_ServerUpdate> updates;

	CL_Mutex mutex;
};

#endif
