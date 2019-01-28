/*
         $Id: 

      ------------------------------------------------------------------------
        ClanLib, the platform independent game SDK.

        This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
        version 2. See COPYING for details.

        For a total list of contributers see CREDITS.

        See http://www.clanlib.org
      ------------------------------------------------------------------------
*/

//! clanVorbis="System"
//! header=vorbis.h

#ifndef header_setupvorbis
#define header_setupvorbis

//: ClanVorbis initialization functions
class CL_SetupVorbis
{
public:
//! Operations:
	//: Initialize the Vorbis library
	static void init(bool register_resources_only = false);

	//: Deinitialize the Vorbis library
	static void deinit();
};

#endif
