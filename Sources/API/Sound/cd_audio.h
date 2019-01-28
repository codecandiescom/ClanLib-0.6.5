/*
	$Id: cd_audio.h,v 1.6 2001/09/22 15:52:18 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanSound="CD Audio"
//! header=sound.h

#ifndef header_cd_audio
#define header_cd_audio

#include <string>
#include <vector>

//: CD drive audio interface.
//- Use this class to control the CDROM drive in your computer.
class CL_CDDrive
{
public:
//! Construction:
	//: CD Drive Destructor
	virtual ~CL_CDDrive() { ; }

	//: Call this one to initialize the cd drive.
	virtual bool init()=0;

//! Attributes:
	//: Get the path of the CD drive. (what is this, starch?)
	virtual std::string get_drive_path()=0;

	//: Get the name of the CD.
	virtual std::string get_cd_name()=0;

	//: Get the number of tracks available on the cd.
	virtual int get_num_tracks()=0;

	//: Returns true if the CD drive is playing.
	virtual bool is_playing()=0;
	
	//: Returns the current track.
	virtual int get_cur_track()=0;

	//: Returns the current frame on the track.
	virtual int get_cur_frame()=0;

//! Operations:
	//: Play from track 'track' til track 'end_track'.
	//- track - Begin track.
	//- end_track - End track.
	virtual bool play_tracks(int track, int end_track=0)=0;

	//: Play from frame 'frame' to 'end_frame'.
	//- frame - Begin frame.
	//- end_frame - End frame.
	virtual bool play_frames(int frame, int end_frame=0)=0;

	//: Play track 'track'. Returns true if successful.
	virtual bool play_track(int track)=0;

	//: Stop playing.
	virtual void stop()=0;

	//: Pause the playback.
	virtual void pause()=0;

	//: Resume the playback.
	virtual void resume()=0;
};

//: CD Audio interface.
class CL_CDAudio
{
public:
//! Attributes:
	//: List of CD drives available.
	static std::vector<CL_CDDrive *> cd_drives;
};

#endif
