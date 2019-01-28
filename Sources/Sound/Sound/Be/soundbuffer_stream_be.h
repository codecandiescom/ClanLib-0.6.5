/*
	$Id: soundbuffer_stream_be.h,v 1.1 2001/03/06 15:09:24 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Sound card impl. that uses the ClanSound library.

*/

#ifndef header_soundbuffer_stream_be
#define header_soundbuffer_stream_be

#include "API/Core/Sound/stream_soundprovider.h"
#include "soundcard_be.h"
#include "cardplayback_be.h"
#include "Core/System/Unix/init_linux.h"

class CL_CardPlayback_Be_Stream
: public CL_CardPlayback_Be, CL_System_KeepAlive
{
protected:
	CL_StreamSoundProvider *provider;
	CL_StreamSoundProvider_Session *provider_session;

public:
	CL_CardPlayback_Be_Stream(CL_StreamSoundProvider *provider);
	virtual ~CL_CardPlayback_Be_Stream();

	virtual bool set_position(int new_pos);
	virtual void play();
	virtual void stop();
	
	virtual bool keep_alive();
};

#endif
