#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/network.h>
#include <ClanLib/Network/socket.h>
#include <ClanLib/Core/System/error.h>

#include <iostream>
#include <list>

#include "ftp.h"
#include "main.h"

char welcome[] = {'2','2','0','h','i','\r','\n'};

std::list<FTPD*> kill_daemon;
bool server_kill;

ClanFTP app;

	char* ClanFTP::get_title() { return "ClanFTP"; }

	int ClanFTP::main(int, char**)
	{
		try
		{
			CL_SetupCore::init();
			CL_SetupNetwork::init();
			CL_ConsoleWindow console("Console");
			console.redirect_stdio();

			CL_IPAddress ip;
			ip.set_port(2021);

			//unsigned short foo[8];
			//	ip.set_address(56<<8|23);
			//	memcpy(foo,ip.get_address6(),16);
			//	for(int i=0;i<16;i++)
			//		cout << (unsigned int)ip[i] << endl;

			ftp = new CL_Socket(CL_Socket::tcp);

			ftp->bind(ip);
			ftp->sig_read_triggered().connect(this,&ClanFTP::conn);
			ftp->sig_write_triggered().connect(this,&ClanFTP::conn);
			ftp->sig_exception_triggered().connect(this,&ClanFTP::conn);
			ftp->listen(4);

			std::cout << "FTP server running." << std::endl;

			while(1)
			{
				if(server_kill)
					for(std::list<FTPD*>::iterator it = kill_daemon.begin();it != kill_daemon.end();it++)
					{
									daemon_list.remove(*it);
									delete *it;
									server_kill = false;
					}
				
						
	 			CL_System::sleep(10);
				CL_System::keep_alive();
			}

			CL_SetupNetwork::deinit();
			CL_SetupCore::deinit();
		}

		catch(CL_Error err) 
		{
			std::cout << err.message << std::endl;
		}

		catch(...)
		{
			std::cout << "A non-ClanLib based error occured.  Please provide clanlib-devel@dark.x.dtu.dk with a back-trace/stack-trace." << std::endl;
		}

		return 0;
	}

void ClanFTP::pop_server(FTPD* server)
{
	kill_daemon.push_back(server);
	server_kill = true;
}

void ClanFTP::conn()
{
	CL_Socket accept = ftp->accept();
	accept.send(welcome,sizeof(welcome));
	FTPD *temp = new FTPD(accept);
	//delete accept;
//	delete ftp;
	daemon_list.push_back(temp);
//	CL_IPAddress ip;
//	ip.set_port(2021);

//	ftp = new CL_Socket(CL_Socket::tcp);

//	ftp->bind(ip);
//	ftp->sig_read_triggered().connect(this,&ClanFTP::conn);
//	ftp->sig_write_triggered().connect(this,&ClanFTP::conn);
//	ftp->sig_exception_triggered().connect(this,&ClanFTP::conn);
//	ftp->listen(4);
}
