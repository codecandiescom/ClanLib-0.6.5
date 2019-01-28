/*
	$Id: cdaudio_linux.h,v 1.6 2001/12/11 20:44:23 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------



*/
#ifndef header_cdaudio_linux
#define header_cdaudio_linux

#include "API/Sound/cd_audio.h"

struct cdrom_tocentry;

class CL_CDDrive_Linux : public CL_CDDrive
{
public:
	static void init_cdaudio();

	CL_CDDrive_Linux(const std::string &filename);
	virtual ~CL_CDDrive_Linux();

	virtual bool init();

	virtual std::string get_drive_path();
	virtual std::string get_cd_name();

	virtual int get_num_tracks();

	virtual bool is_playing();
	virtual int get_cur_track();
	virtual int get_cur_frame();

	virtual bool play_tracks(int track, int end_track);
	virtual bool play_frames(int frame, int end_frame);
	virtual bool play_track(int track);

	virtual void stop();
	virtual void pause();
	virtual void resume();

private:
	std::string filename;
	int file_handle;
	
	std::string cd_name;
	struct STrack
	{
		int start_lba;
		int track_length;
		bool is_audio;
	};
	
	std::vector<STrack> tracks;
	int num_tracks;
	
	int first_track;
	int last_track;
	
	bool readtoc();
	bool readtocentry(int track_no,struct cdrom_tocentry* track_info);
	bool readtocentry(int track_no,struct ioc_read_toc_single_entry* track_info);
};

#endif
