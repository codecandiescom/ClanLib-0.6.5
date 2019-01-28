/*
	$Id: soundbuffer_generic.cpp,v 1.3 2001/06/12 21:44:56 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include "soundbuffer_generic.h"
#include "API/Core/System/cl_assert.h"

CL_SoundBuffer_Generic::CL_SoundBuffer_Generic()
:
	ref(0)
{
}

CL_SoundBuffer_Generic::~CL_SoundBuffer_Generic()
{
	cl_assert(ref == 0);
}

int CL_SoundBuffer_Generic::add_reference()
{
	return ++ref;
}

int CL_SoundBuffer_Generic::release_reference()
{
	int retval = --ref;
	if (retval == 0) delete this;
	return retval;
}
