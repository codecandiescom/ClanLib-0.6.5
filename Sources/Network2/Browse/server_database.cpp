
#ifdef WIN32
#pragma warning(disable : 4786)
#endif

#include "server_database.h"
#include "API/Core/System/system.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ServerDatabase construction:

CL_ServerDatabase::CL_ServerDatabase() : ref_count(0)
{
}

CL_ServerDatabase::~CL_ServerDatabase()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ServerDatabase attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_ServerDatabase operations:

int CL_ServerDatabase::add_ref()
{
	return ++ref_count;
}

int CL_ServerDatabase::release_ref()
{
	--ref_count;
	if (ref_count == 0)
	{
		delete this;
		return 0;
	}
	return ref_count;
}

void CL_ServerDatabase::update_server(const CL_IPAddress &address, const std::string &desc)
{
	CL_MutexSection mutex_lock(&mutex);

	CL_ServerUpdate update;
	update.timestamp = CL_System::get_time();
	update.server = address;
	updates.push_front(update);

	std::map<CL_IPAddress, CL_ServerDescription *>::iterator it;

	it = servers.find(address);
	if (it != servers.end())
	{
		it->second->ref_count--;
		if (it->second->ref_count == 0) delete it->second;
	}

	CL_ServerDescription *server_description = new CL_ServerDescription;
	server_description->ref_count = 1;
	server_description->desc = desc;

	servers[address] = server_description;
}

void CL_ServerDatabase::remove_server(const CL_IPAddress &address)
{
	CL_MutexSection mutex_lock(&mutex);

	CL_ServerUpdate update;
	update.timestamp = CL_System::get_time();
	update.server = address;
	updates.push_front(update);

	std::map<CL_IPAddress, CL_ServerDescription *>::iterator it;

	it = servers.find(address);
	if (it != servers.end())
	{
		it->second->ref_count--;
		if (it->second->ref_count == 0) delete it->second;
		servers.erase(it);
	}
}

CL_ServerChanges CL_ServerDatabase::get_changes(unsigned int timestamp)
{
	CL_MutexSection mutex_lock(&mutex);

	CL_ServerChanges changes;
	changes.timestamp = CL_System::get_time();

	if (timestamp == 0) changes.servers = servers;
	else
	{
		std::deque<CL_ServerUpdate>::iterator it;

		for (it = updates.begin(); it != updates.end(); ++it)
		{
			if (it->timestamp < timestamp) break;

			std::map<CL_IPAddress, CL_ServerDescription *>::iterator server_it;

			server_it = servers.find(it->server);
			if (server_it != servers.end())
			{
				changes.servers[it->server] = server_it->second;
			}
			else
			{
				changes.removed_servers.push_back(it->server);
			}
		}
	}

	std::map<CL_IPAddress, CL_ServerDescription *>::iterator it;
	for (it = changes.servers.begin(); it != changes.servers.end(); ++it)
	{
		it->second->ref_count++;
	}

	return changes;
}

void CL_ServerDatabase::release_changes(CL_ServerChanges &changes)
{
	CL_MutexSection mutex_lock(&mutex);

	std::map<CL_IPAddress, CL_ServerDescription *>::iterator it;
	for (it = changes.servers.begin(); it != changes.servers.end(); ++it)
	{
		it->second->ref_count--;
		if (it->second->ref_count == 0) delete it->second;
	}

	changes.removed_servers.clear();
	changes.servers.clear();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ServerDatabase implementation:
