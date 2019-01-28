/*
	$Id: implementation_clansound.h,v 1.1 2001/04/14 19:10:56 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Initialize sound support using ClanSound.

*/

#ifndef header_implementation_clansound
#define header_implementation_clansound

#ifdef USE_CLANSOUND

char *clansound_identify();
char *clansound_abbreviation();
void clansound_init();

class CL_Implementation_ClanSound
{
public:
	void add_sound();
};

#endif /* USE_CLANSOUND */

#endif
