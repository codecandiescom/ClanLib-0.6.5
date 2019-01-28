/*
	$Id: setupnetwork.h,v 1.1 2002/11/02 19:45:58 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanNetwork="System"

#ifndef header_setupnetwork
#define header_setupnetwork

//: Initialization class for clanNetwork.
class CL_SetupNetwork
{
public:
	//: Initialize network.
	static void init(bool register_resources_only = false);

	//: Deinitialize network.
	static void deinit();
};

#endif
