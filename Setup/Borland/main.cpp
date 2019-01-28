/*
	$Id: main.cpp,v 1.9 2002/03/18 06:31:24 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

//---------------------------------------------------------------------------

#include <iostream.h>
#include <string>
#include <conio.h>
#include "generator.h"
#pragma hdrstop

//---------------------------------------------------------------------------

#pragma argsused
int main(int argc, char* argv[])
{
	char tasm;

  Workspace work;

  std::list<std::string> libs_list_shared;
	std::list<std::string> libs_list_release;
	std::list<std::string> libs_list_debug;
	std::list<std::string> defines_list;
  
  defines_list.push_back("WIN32");
  defines_list.push_back("_WINDOWS");
  defines_list.push_back("USE_OPENGL");
  defines_list.push_back("USE_NETWORK");
  defines_list.push_back("BORLAND");
  defines_list.push_back("USE_CLANSOUND");
	defines_list.push_back("DIRECTINPUT_VERSION=0x700");

	cout << "Welcome to ClanLib Borland Configurer!" <<endl;
	cout<<"Do you have TASM?[y/n]";
	cin>>tasm;
	if(tasm == 'y' || tasm == 'Y')
	{
		defines_list.push_back("USE_I386_ASSEMBLER");
		defines_list.push_back("USE_TASM");
	}

  Project app("Application","libApp","application.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
  Project core("Core","libCore","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
  Project display("Display","libDisplay","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
  Project gl("GL","libGL","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
  Project gui("GUI","libGUI","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
  Project jpeg("JPEG","libJpeg","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
  Project javascript("JavaScript","libJavaScript","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);

  Project lua("Lua","libLua","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
  Project midi("Midi","libMidi","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
  Project mikmod("MikMod","libMikMod","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
  Project network("Network","libNetwork","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
  Project png("PNG","libPng","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
  Project sound("Sound","libSound","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
  Project smallJPEG("SmallJPEG","libSmallJPEG","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
  Project ttf("TTF","libTTF","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);
  Project vorbis("Vorbis","libVorbis","core.h",libs_list_shared,libs_list_release,libs_list_debug,defines_list);

  work.projects.push_back(app);
  work.projects.push_back(core);
  work.projects.push_back(display);
  work.projects.push_back(gl);
  work.projects.push_back(gui);
  work.projects.push_back(jpeg);
  work.projects.push_back(javascript);
  work.projects.push_back(lua);
  work.projects.push_back(midi);
  work.projects.push_back(mikmod);
  work.projects.push_back(network);
  work.projects.push_back(png);
  work.projects.push_back(smallJPEG);
  work.projects.push_back(sound);
  work.projects.push_back(ttf);
  work.projects.push_back(vorbis);

  create_release(work);
  create_makefile(work);



  cout <<"Project creation complete(press any key to quit)." <<endl;

  while(!kbhit()) ;
	return 0;
}
//---------------------------------------------------------------------------
 