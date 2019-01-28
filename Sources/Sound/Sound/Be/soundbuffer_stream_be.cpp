/*
	$Id: soundbuffer_stream_be.cpp,v 1.1 2001/03/06 15:09:24 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Sound card impl. that uses the ClanSound library.

*/

#include "Core/precomp.h"

#include <API/Core/Sound/stream_soundprovider.h>
#include "API/Core/System/cl_assert.h"
#include <Core/Sound/Be/cardplayback_be.h>
#include <Core/Sound/Be/soundbuffer_stream_be.h>

CL_CardPlayback_Be_Stream::CL_CardPlayback_Be_Stream(CL_StreamSoundProvider *_provider)
: CL_CardPlayback_Be(NULL)
{
}

CL_CardPlayback_Be_Stream::~CL_CardPlayback_Be_Stream()
{
}

bool CL_CardPlayback_Be_Stream::keep_alive()
{
	return false;
}

bool CL_CardPlayback_Be_Stream::set_position(int new_pos)
{
	return false;
}

void CL_CardPlayback_Be_Stream::stop()
{
}

void CL_CardPlayback_Be_Stream::play()
{
}
