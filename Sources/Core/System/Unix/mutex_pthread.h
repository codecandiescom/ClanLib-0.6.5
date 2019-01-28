/*
	$Id: mutex_pthread.h,v 1.2 2001/05/07 17:10:12 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_mutex_pthread
#define header_mutex_pthread

#include "API/Core/System/mutex.h"

class CL_Mutex_Generic
{
public:
	pthread_mutex_t mutex;
	pthread_cond_t cond;
};

#endif
