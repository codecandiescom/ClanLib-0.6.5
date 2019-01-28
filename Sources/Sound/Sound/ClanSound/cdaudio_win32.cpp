/*
	$Id: cdaudio_win32.cpp,v 1.5 2001/10/16 16:53:04 japj Exp $

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

#include <windows.h>
#include "API/Sound/cd_audio.h"
#include <cstdio>
#include "cdaudio_win32.h"

using namespace std;

void CL_CDDrive_Win32::init_cdaudio()
{
	int i;
	char drive[4];

	for (i='A'; i<='Z'; i++) 
	{
		sprintf(drive, "%c:\\", i);
		if (GetDriveType(drive) == DRIVE_CDROM ) 
		{
			char name[256];
			GetVolumeInformation(drive, name, 256, NULL, NULL, NULL, NULL, 0);

			CL_CDAudio::cd_drives.push_back(new CL_CDDrive_Win32(drive, name));
		}
	}
}

/***************************************************
              class CL_CDDrive_Win32
****************************************************/

CL_CDDrive_Win32::CL_CDDrive_Win32(std::string path, std::string name)
{
	this->path = path;
	this->name = name;
	id = 0;
}

CL_CDDrive_Win32::~CL_CDDrive_Win32()
{
}

bool CL_CDDrive_Win32::send_command(unsigned int msg, unsigned int flags, void *arg)
{
	MCIERROR mci_error;
	mci_error = mciSendCommand(id, msg, flags, (DWORD) arg);

	return mci_error == 0;
}

bool CL_CDDrive_Win32::init()
{
	MCI_OPEN_PARMS mci_open;
	memset(&mci_open, 0, sizeof(MCI_OPEN_PARMS));
	MCI_SET_PARMS mci_set;
	memset(&mci_set, 0, sizeof(MCI_SET_PARMS));
	DWORD flags;

	mci_open.lpstrDeviceType = (LPCSTR) MCI_DEVTYPE_CD_AUDIO;
	mci_open.lpstrElementName = path.c_str();
	
	flags =	MCI_OPEN_TYPE |
			MCI_OPEN_SHAREABLE |
			MCI_OPEN_TYPE_ID |
			MCI_OPEN_ELEMENT;

	if (!send_command(MCI_OPEN, flags, &mci_open))
	{
		flags &= ~MCI_OPEN_SHAREABLE;
		if (!send_command(MCI_OPEN, flags, &mci_open)) 
		{
			return false;
		}
	}
	id = mci_open.wDeviceID;

	/* Set the minute-second-frame time format */
	mci_set.dwTimeFormat = MCI_FORMAT_MSF;
	send_command(MCI_SET, MCI_SET_TIME_FORMAT, &mci_set);

	return read_toc();
}

bool CL_CDDrive_Win32::read_toc()
{
	MCI_STATUS_PARMS mci_status;
	int i;
	DWORD flags;
	bool retval = false;

	mci_status.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
	flags = MCI_STATUS_ITEM | MCI_WAIT;

	if (send_command(MCI_STATUS, flags, &mci_status)) 
	{
		num_tracks = mci_status.dwReturn;
		tracks.reserve(num_tracks+1);

		flags = MCI_STATUS_ITEM | MCI_TRACK | MCI_WAIT;
		for (i=0; i<num_tracks;i++) 
		{
			STrack track;
			track.id = i+1;

			mci_status.dwTrack = track.id;
			mci_status.dwItem = MCI_CDA_STATUS_TYPE_TRACK;
			if (!send_command(MCI_STATUS, flags, &mci_status))
			{
				break;
			}

			if (mci_status.dwReturn == MCI_CDA_TRACK_AUDIO ) 
			{
				track.is_audio = true;
			} else 
			{
				track.is_audio = false;
			}

			mci_status.dwItem = MCI_STATUS_POSITION;
			if (!send_command(MCI_STATUS, flags, &mci_status))
			{
				break;
			}
			
			track.start_frame = MSF_TO_FRAMES(
					MCI_MSF_MINUTE(mci_status.dwReturn),
					MCI_MSF_SECOND(mci_status.dwReturn),
					MCI_MSF_FRAME(mci_status.dwReturn));
			track.length_frames = 0;

			if (i > 0) 
			{
				tracks[i-1].length_frames =	track.start_frame - tracks[i-1].start_frame;
			}
			tracks.push_back(track);
		}
		
		if (i == num_tracks) 
		{
			flags &= ~MCI_TRACK;
			mci_status.dwItem = MCI_STATUS_LENGTH;
			if (send_command(MCI_STATUS, flags,	&mci_status)) 
			{
				int offset = MSF_TO_FRAMES(
					MCI_MSF_MINUTE(mci_status.dwReturn),
					MCI_MSF_SECOND(mci_status.dwReturn),
					MCI_MSF_FRAME(mci_status.dwReturn));
				
				STrack track;
				track.is_audio = false;
				track.start_frame = offset;
				track.length_frames = 0;
				tracks.push_back(track);

				tracks[i-1].length_frames = offset - tracks[i-1].start_frame;
				retval = true;
			}
		}
	}

	return retval;
}

void CL_CDDrive_Win32::update_status()
{
	MCI_STATUS_PARMS mci_status;
	DWORD flags;

	flags = MCI_STATUS_ITEM | MCI_WAIT;
	mci_status.dwItem = MCI_STATUS_MODE;

	bool update_position = false;
	
	cd_playing = false;
	cur_track = -1;
	cur_frame = -1;

	if (!send_command(MCI_STATUS, flags, &mci_status)) 
	{
		throw "Failed to get CD status";
	}
	else
	{

		switch (mci_status.dwReturn) 
		{
			case MCI_MODE_PLAY:
				cd_playing = true;
				update_position = true;
				break;
			case MCI_MODE_PAUSE:
				update_position = true;
				break;
		}
	}

	if (update_position) 
	{
		mci_status.dwItem = MCI_STATUS_POSITION;
		if (send_command(MCI_STATUS, flags,	&mci_status)) 
		{
			cur_frame = MSF_TO_FRAMES(
				MCI_MSF_MINUTE(mci_status.dwReturn),
				MCI_MSF_SECOND(mci_status.dwReturn),
				MCI_MSF_FRAME(mci_status.dwReturn));

			cur_track = 0;
			int t = 1;
			for (std::vector<STrack>::iterator it = tracks.begin();it!=tracks.end();it++)
			{
				if ((*it).start_frame < cur_frame && ((*it).start_frame+(*it).length_frames) > cur_frame)
				{
					cur_track = t;
					break;
				}
			}
		} 
	}
}

std::string CL_CDDrive_Win32::get_drive_path()
{
	return path;
}

std::string CL_CDDrive_Win32::get_cd_name()
{
	return name;
}

int CL_CDDrive_Win32::get_num_tracks()
{
	return num_tracks;
}

bool CL_CDDrive_Win32::is_playing()
{
	update_status();
	return cd_playing;
}

int CL_CDDrive_Win32::get_cur_track()
{
	update_status();
	return cur_track;
}

int CL_CDDrive_Win32::get_cur_frame()
{
	update_status();
	return cur_frame;
}

bool CL_CDDrive_Win32::play_tracks(int track, int end_track)
{
	if (track < 1 || track > num_tracks) return false;
	if (end_track != 0 && end_track < track) return false;
	if (end_track == 0) end_track = num_tracks;
	if (end_track >= tracks.size()) end_track = tracks.size()-1;

	return play_frames(
		tracks[track-1].start_frame,
		tracks[end_track-1].start_frame+tracks[end_track-1].length_frames);
}

bool CL_CDDrive_Win32::play_frames(int frame, int end_frame)
{
	MCI_PLAY_PARMS mci_play;
	int m, s, f;
	DWORD flags;

	flags = MCI_FROM | MCI_TO | MCI_NOTIFY;
	mci_play.dwCallback = 0;
	FRAMES_TO_MSF(frame, &m, &s, &f);
	mci_play.dwFrom = MCI_MAKE_MSF(m, s, f);
	FRAMES_TO_MSF(end_frame, &m, &s, &f);
	mci_play.dwTo = MCI_MAKE_MSF(m, s, f);
	
	return send_command(MCI_PLAY, flags, &mci_play);

}

bool CL_CDDrive_Win32::play_track(int track)
{
	if (track < 1 || track > num_tracks) return false;
	return play_frames(
		tracks[track-1].start_frame,
		tracks[track-1].start_frame+tracks[track-1].length_frames);
}

void CL_CDDrive_Win32::stop()
{
	send_command(MCI_STOP, MCI_WAIT, NULL);
}

void CL_CDDrive_Win32::pause()
{
	send_command(MCI_PAUSE, MCI_WAIT, NULL);
}

void CL_CDDrive_Win32::resume()
{
	send_command(MCI_RESUME, MCI_WAIT, NULL);
}

