/*
	$Id: vorbis_soundprovider.h,v 1.13 2002/01/11 02:48:23 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanVorbis="Sound Providers"
//! header=vorbis.h

#ifndef header_vorbis_soundprovider
#define header_vorbis_soundprovider

#include "../core.h"
#include "../Sound/stream_soundprovider.h"
#include "../Sound/soundbuffer.h"

class CL_VorbisSoundProvider_Generic;

//: Vorbis Sound Provider Class
class CL_VorbisSoundProvider : public CL_StreamSoundProvider
{
public:
//! Construction:
	//: Vorbis Sound Provider Constructor
	//- name - The name of the file, either it's filename or descriptor from 'provider'
	//- provider - the InputsourceProvider to use
	//- delete_provider - If true, 'provider' will be delete when the destructor is called
	CL_VorbisSoundProvider(
		const std::string &name,
		CL_InputSourceProvider *provider = NULL,
		bool delete_provider = false);

	//: Vorbis Sound Provider Constructor
	//- name - The name of the vorbis file in 'resources'
	//-	resources - The ResourceManager to use to access the resource identified by 'name'
	CL_VorbisSoundProvider(
		const std::string &name,
		CL_ResourceManager *resources);

	//: Vorbis Sound Provider Destructor
	virtual ~CL_VorbisSoundProvider();

//! Operations:
	//: Begin Session
	//- Returns - A CL_StreamSoundProvider_Session object for the resource named in the constructor
	virtual CL_StreamSoundProvider_Session *begin_session();

	//: End Session
	//: Deletes the object 'session'
	//- session - The CL_StreamSoundProvider_Session to be deleted
	virtual void end_session(CL_StreamSoundProvider_Session *session);

//! Implementation:
private:
	CL_VorbisSoundProvider(const CL_VorbisSoundProvider &copy) { return; } // disallow copy construction.
	CL_VorbisSoundProvider_Generic *impl;
};

#endif
