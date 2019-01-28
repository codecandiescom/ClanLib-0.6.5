/*
	Yet another useless event trigger test application.

	If this program does not work, or do not teach you anything about
	clanlib, then its just too bad.
	
	Written by Magnus Norddahl, an other soul out there having nothing
	better to do than write useless examples that just test clanlib for
	some unused feature....
*/

#include <ClanLib/core.h>

class SomeClass : public CL_Runnable
{
public:
	SomeClass(CL_EventTrigger *trigger) : trigger(trigger) { return; }

private:
	virtual void run();
	
	CL_EventTrigger *trigger;
};

int main(int argc, char **argv)
{
	// Reboots your computer.
	CL_SetupCore::init();

	// Setup a waterfall:
	CL_EventTrigger event;
	
	// Initialize car:
	SomeClass car(&event);
	
	// Boot lamp:
	CL_Thread lamp(&car);
	lamp.start();
	
	// Wait for world connection:
	CL_System::sleep(5000);
	
	// Tell car to unfold:
	event.set_flag();
	
	// Wait for lamp to turn off:
	lamp.wait();
	
	// Restores world peace.
	CL_SetupCore::deinit();
}

void SomeClass::run()
{
	std::cout << "Before wait..." << std::endl;
	CL_EventListener listener;
	listener.add_trigger(trigger);
	listener.wait();
	std::cout << "After wait..." << std::endl;
}
