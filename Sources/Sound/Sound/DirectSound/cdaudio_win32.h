/*
	$Id: cdaudio_win32.h,v 1.2 2001/09/08 19:24:21 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#ifndef header_cdaudio_win32
#define header_cdaudio_win32

#include "API/Core/Sound/cd_audio.h"

/* Conversion functions from frames to Minute/Second/Frames and vice versa */
#define CD_FPS	75
#define FRAMES_TO_MSF(f, M,S,F)	{					\
	int value = f;							\
	*(F) = value%CD_FPS;						\
	value /= CD_FPS;						\
	*(S) = value%60;						\
	value /= 60;							\
	*(M) = value;							\
}
#define MSF_TO_FRAMES(M, S, F)	((M)*60*CD_FPS+(S)*CD_FPS+(F))

class CL_CDDrive_Win32 : public CL_CDDrive
{
public:
	CL_CDDrive_Win32(std::string path, std::string name);
	virtual ~CL_CDDrive_Win32();

	virtual bool init();

	virtual std::string get_drive_path();
	virtual std::string get_cd_name();

	virtual int get_num_tracks();

	virtual bool is_playing();
	virtual int get_cur_track();
	virtual int get_cur_frame();

	virtual bool play_tracks(int track, int end_track=0);
	virtual bool play_frames(int frame, int end_frame=0);
	virtual bool play_track(int track);

	virtual void stop();
	virtual void pause();
	virtual void resume();
private:
	std::string path, name;
	MCIDEVICEID id;
	
	struct STrack
	{
		unsigned int id;
		unsigned int start_frame;
		unsigned int length_frames;
		bool is_audio;
	};
	std::vector<STrack> tracks;
	unsigned int num_tracks;

	int cur_track;
	int cur_frame;
	bool cd_playing;

	bool send_command(unsigned int msg, unsigned int flags, void *arg);
	bool read_toc();
	void update_status();
};

#endif