/*
	$Id: setupnetwork.h,v 1.11 2002/01/22 10:14:02 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanNetwork="System"
//! header=network.h

#ifndef header_setupnetwork
#define header_setupnetwork

//: Initialization class for clanNetwork.
class CL_SetupNetwork
{
public:
//! Operations:
	//: Initialize network.
	static void init(bool register_resources_only = false);

	//: Deinitialize network.
	static void deinit();
};

#endif
