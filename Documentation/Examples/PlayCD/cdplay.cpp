/*
	Example of playing a cd.
*/

#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/sound.h>

class CDPlayApp : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "CD Play Application"; }

	virtual int main(int, char **)
	{
		// make sure there is some "visible" output
		CL_ConsoleWindow console("My console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore::init();
		
			// Setup Sound
			CL_SetupSound::init();

			// Get the first drive in the system
			CL_CDDrive* mycd = CL_CDAudio::cd_drives.front();

			// open the cd
			mycd->init();

			std::cout << "Playing tracks 7 and 8" << std::endl;

			//play tracks 7 and 8
			mycd->play_tracks(7,8);

			// you can exit right now.. the system will continue
			// playing the cd.. If not, you need the following while
			
			// if you stop the CD by hand, this loop will exit (because the CD
			// won't be playing anymore
			while(mycd->is_playing())
			{
				CL_System::keep_alive();
				CL_System::sleep(10);
			} 

			// stop the cd
			mycd->stop();

			CL_SetupSound::deinit();			
			CL_SetupCore::deinit();			
		}
		catch(CL_Error ce)
		{
			std::cout << ce.message << std::endl;
		}

		// Display console close message and wait for a key
		console.display_close_message();

		return 0;
	}
} app;
