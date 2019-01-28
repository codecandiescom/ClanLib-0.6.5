/*
	$Id: implementation.h,v 1.2 2001/09/08 19:12:45 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/


#ifndef __IMPLEMENTATION_H__
#define __IMPLEMENTATION_H__

#ifdef USE_DYN
#define DL_PREFIX
#else
#define DL_PREFIX static
#endif

typedef char* (*clan_id_proc)();
typedef void (*clan_init_proc)();

#endif





