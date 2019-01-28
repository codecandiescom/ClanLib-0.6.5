/*
	$Id: streamed_mikmod_sample.h,v 1.15 2001/12/11 20:44:20 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

	File purpose:
		Streamed sample header file
*/

//! clanMikMod="Sound Providers"
//! header=mikmod.h

#ifndef header_streamed_mikmod_sample
#define header_streamed_mikmod_sample

#include <stdlib.h>
#include "../core.h"
#include "../Sound/stream_soundprovider.h"
#include "../Sound/soundbuffer.h"

#include <mikmod.h>

//: CL_Streamed_MikModSample is used to store MikMod modules in memory.
//: <p>This class stores the ClanLib sample the way the MikMod MODULE struct
//: does. It's only intended to store the module in memory, not to play
//: it directly. To play the module, use CL_Streamed_MikModSample_Session
//: instead.</p>
class CL_Streamed_MikModSample : public CL_StreamSoundProvider
{
protected:
	std::string filename;
	bool looped;
	bool loaded_from_resource;
	CL_ResourceManager *manager;

	MODULE *module;
public:
//! Construction:
	//: Use this function to create a module from a file for instance
	static CL_SoundBuffer *create(
		const std::string &filename, 
		CL_InputSourceProvider *inputprovider = NULL, 
		bool looped = false);

	//: Loads a module from a resource file (.scr or .dat)
	static CL_SoundBuffer *load(
		const std::string &res_id, 
		CL_ResourceManager *manager, 
		bool looped = false);

	//: Use this function to create a module from a file for instance
	CL_Streamed_MikModSample(
		const std::string &filename, 
		CL_InputSourceProvider *inputprovider = NULL, 
		bool looped = false);

	//: Loads a module from a resource file (.scr or .dat)
	CL_Streamed_MikModSample(
		const std::string &res_id, 
		CL_ResourceManager *manager, 
		bool looped = false);

	//: Streamed MikModSample Destructor
	virtual ~CL_Streamed_MikModSample();

//! Operations:
	//: Starts playing the module
	virtual CL_StreamSoundProvider_Session *begin_session();

	//: Ends the session
	virtual void end_session(CL_StreamSoundProvider_Session *session);

};

//: CL_Streamed_MikModSample_Session is used to play and control a module.
class CL_Streamed_MikModSample_Session : public CL_StreamSoundProvider_Session
{
protected:
	friend class CL_Streamed_MikModSample;

	CL_Streamed_MikModSample_Session(MODULE *_module, bool _looped);

	MODULE *module;
	bool looped;

	SoundFormat sample_format;
	int sample_size;
	int sample_freq;
	int sample_left;

public:
//! Construction:
	//: ~CL Streamed MikModSample Session
	virtual ~CL_Streamed_MikModSample_Session();

//! Operations:
	//: Stop
	virtual void stop();

	//: Play
	virtual bool play();

	//: Set position
	virtual bool set_position(int pos);

	//: ClanMikMod Update
	static void clanMikMod_Update() { /* do nothing */ }

//! Attributes:
	//: Eof
	virtual bool eof() const;

	//: Get data
	virtual int get_data(void *data_ptr, int data_requested);

	//: Get frequency
	virtual int get_frequency() const;

	//: Get format
	virtual SoundFormat get_format() const;

	//: ClanMikMod IsPresent
	static BOOL clanMikMod_IsPresent() { return 1; }


};

#endif

















