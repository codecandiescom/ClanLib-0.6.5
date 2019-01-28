#ifndef _main_h
#define _main_h
#include <ClanLib/application.h>

class ClanFTP : public CL_ClanApplication
{
public:
	char* get_title();

	CL_Socket *ftp;
	std::list<FTPD*> daemon_list;

	int main(int, char**);

	void conn();

	void pop_server(FTPD*);

} extern app;
#endif
