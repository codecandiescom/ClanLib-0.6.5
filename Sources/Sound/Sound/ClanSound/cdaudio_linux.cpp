/*
	$Id: cdaudio_linux.cpp,v 1.20 2002/06/22 12:58:11 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#include "Core/precomp.h"

#include <API/Core/System/clanstring.h>
#include <API/Core/System/error.h>
#include <API/Core/System/cl_assert.h>
#include <fcntl.h>

#ifndef Solaris // we'll need to define Solaris
#include <fstab.h> // there's no fstab.h on Solaris...
#else
#include <sys/vfstab.h> //...but vfstab.h
#endif

//#include <mntent.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <cstdio>

#ifdef linux
#include <linux/cdrom.h>
#else
#include <sys/cdio.h> // cdio.h is in Solaris and FreeBSD
#endif

#define LBA(msf) ((msf.minute * 60 + msf.second) * 75 + msf.frame)

#ifdef __FreeBSD__
#define CDROM_LBA CD_LBA_FORMAT
#define CDROMSUBCHNL CDREADSUBQ
#define CDROMPLAYMSF CDIOCPLAYMSF
#define CDROMSTOP CDIOCSTOP
#define CDROMPAUSE CDIOCPAUSE
#define CDROMRESUME CDIOCRESUME
#define CDROMREADTOCHDR CDIOREADTOCHEADER
#define CDROMREADTOCENTRY CDIOREADTOCENTRY 
#define CDROM_LEADOUT 0xAA;
#define CD_FRAMES 75
#define CD_SECS 60
///#define cdrom_subchnl cd_sub_channel_info
#endif

#ifdef Solaris 
#define CD_SECS 60    // needed on Solaris
#define CD_FRAMES 75  // needed on Solaris
#endif

#ifdef Solaris
#define CDROM_LINK "/dev/dsk/c0t6d0s0"
#else
#define CDROM_LINK "/mnt/cdrom"
#endif

#include "cdaudio_linux.h"

void CL_CDDrive_Linux::init_cdaudio()
{
	// Make sure init only happen once:

	static bool already_inited = false;
	if (already_inited) return;
	already_inited = true;
	bool found_in_mount_file=false;
	
	// Locate all CDROM drives available on the system.
	
	// We start by looking the mounttab:
	
#ifdef Solaris
	FILE *f_mnttab = fopen("/etc/mnttab", "r");
	vfstab *mount_file;
	getvfsent(f_mnttab, mount_file);
#else
	fstab *mount_file = getfsent();
#endif
	while(mount_file != NULL)
	{
#ifndef Solaris
		if(strncmp(mount_file->fs_vfstype,"iso9660",7)==0) 
#else
		if(strncmp(mount_file->vfs_fstype,"iso9660",7)==0) // ugly code... suggestions ?
#endif
		{
			try
		 	{
#ifndef Solaris
				CL_CDAudio::cd_drives.push_back(new CL_CDDrive_Linux(mount_file->fs_spec));
#else
				CL_CDAudio::cd_drives.push_back(new CL_CDDrive_Linux(mount_file->vfs_special));
#endif
				found_in_mount_file = true;
			}
			catch (CL_Error err)
	 		{
//				cl_info(info_sound, err.message.c_str());
			}
		}
#ifndef Solaris
		mount_file = getfsent();
#else
		getvfsent(f_mnttab, mount_file);
#endif
		//		cout << mount_file->fs_vfstype << endl;
	}
#ifndef Solaris
	endfsent();
#else
	fclose(f_mnttab);
#endif
	if (found_in_mount_file) return; // success! :)
	
	
	try
	{
		CL_CDAudio::cd_drives.push_back(new CL_CDDrive_Linux(CDROM_LINK));

	}
	catch (CL_Error)
	{
//		std::cout << "No CDROM drives found." << std::endl;
//		std::cout << "Either symlink it from /dev/cdrom, or add it to /etc/fstab." << std::endl;
	}
	
// setmntent("/etc/fstab", "r");
/*
	if (mount_file != NULL)
	{
		bool found_in_mount_file = false;
		while (true)
		{
			mntent *entry = getmntent(mount_file);
			if (entry == NULL) break; // no more entries.
			
			if (strcmp(entry->mnt_type, "iso9660") == 0)
			{
				try
				{
					CL_CDAudio::cd_drives.push_back(
						new CL_CDDrive_Linux(entry->mnt_fsname));
					
					found_in_mount_file = true;
				}
				catch (CL_Error err)
				{
					// Damn.
//					cl_info(info_sound, err.message.c_str());
				}
			}
		}

		endmntent(mount_file);
		if (found_in_mount_file) return; // success! :)
	}
	
	// Ok. None there - assume he linked it from /dev/cdrom.
	
	try
	{
		CL_CDAudio::cd_drives.push_back(new CL_CDDrive_Linux("/dev/cdrom"));
	}
	catch (CL_Error)
	{
//		std::cout << "Now CDROM drives found." << std::endl;
//		std::cout << "Either symlink it from /dev/cdrom, or add it to /etc/fstab." << std::endl;
	}
*/
}

/**************************************************************/

CL_CDDrive_Linux::CL_CDDrive_Linux(const std::string &filename)
{
	this->filename = filename;
	
	// Is this really a cdrom drive??
	
	struct stat status;
	int err = stat(filename.c_str(), &status);
	if (err <= -1)
	{
		// File doesn't exist! (or something else failed.)
		std::string err;
		err += "Device " + filename + " not found!";
		throw CL_Error(err);
	}
	
	if (!S_ISCHR(status.st_mode) && !S_ISBLK(status.st_mode))
	{
		std::string err;
		err += "Device " + filename + " is not a block device!";
		throw CL_Error(err);
	}
	
	file_handle = open(filename.c_str(),O_RDONLY);//|O_NONBLOCK);
	int error = errno;
	if (file_handle <= 0)
	{
//		cout << "File handle: " << file_handle << endl;
//		cout << "Error is: " << error << endl;
		std::string err;
		err += "Device " + filename + " could not be opened!";
		throw CL_Error(err);
	}
	
	#ifdef __FreeBSD__
	ioc_read_subchannel info;
	#else
	cdrom_subchnl info;
	#endif

	if (ioctl(file_handle, CDROMSUBCHNL, &info) == 0 ||
		/* is the disk not present?: */
		errno == EIO || 
		errno == ENOENT ||
		errno == EINVAL)
	{
		// Hurray. Found a cdrom drive.
//		cl_info_debug(info_sound, "HURRAY! We found a cdrom drive.");
	}
	else
	{
		error = errno;
//		cout << "Error Number: " << error << endl;
		close(file_handle);
	
		std::string err;
		err += "Device " + filename + " is not a cdrom device!";
		throw CL_Error(err);
		file_handle = -1;
	}
	close(file_handle);
	file_handle = -1;
}

CL_CDDrive_Linux::~CL_CDDrive_Linux()
{
	if (file_handle != -1) close(file_handle);
}

bool CL_CDDrive_Linux::init()
{
	file_handle = open(filename.c_str(),O_RDONLY|O_NONBLOCK);
	if (file_handle < 0)
	{
		file_handle = -1;
		return false;
	}
	
    if(!readtoc())
    {
		close(file_handle);
	
		std::string err;
		err += "TOC Read error on Device " + filename;
		throw CL_Error(err);
		file_handle = -1;
    }
	return true;
}

std::string CL_CDDrive_Linux::get_drive_path()
{
	return filename;
}

std::string CL_CDDrive_Linux::get_cd_name()
{
	return std::string("UnKnown");
}

int CL_CDDrive_Linux::get_num_tracks()
{
	return num_tracks;
}

bool CL_CDDrive_Linux::is_playing()
{
	#ifdef __FreeBSD__
	ioc_read_subchannel sub_channel; 
	cd_sub_channel_header subinfo;

	sub_channel.data_len = sizeof(subinfo);
	sub_channel.data = &subinfo;	
	#else
	struct cdrom_subchnl sub_channel;
	#endif

	#ifdef __FreeBSD__
	sub_channel.address_format = CD_LBA_FORMAT;
	#else
	sub_channel.cdsc_format = CDROM_LBA;
 	#endif   

#ifdef __FreeBSD__
	if(!ioctl(file_handle,CDIOCREADSUBCHANNEL, &sub_channel))
	{
		if(subinfo.audio_status == CD_AS_PLAY_IN_PROGRESS)
			return true;
		else
			return false;
	}
	else
		return false;	
#else
    if(!ioctl(file_handle,CDROMSUBCHNL,&sub_channel))
    {
        if(sub_channel.cdsc_audiostatus == CDROM_AUDIO_PLAY)
	    {
            return true;
	    }
	    else
	    {
	        return false;
		}
	}
	else
	{
	    return false;
	}
#endif


		//	return false;
}

int CL_CDDrive_Linux::get_cur_track()
{
#ifdef __FreeBSD__
	ioc_read_subchannel sub_channel;
	sub_channel.address_format = CD_LBA_FORMAT;
	ioctl(file_handle,CDROMSUBCHNL, &sub_channel);
	return sub_channel.track;
#else
    struct cdrom_subchnl sub_channel;

    sub_channel.cdsc_format=CDROM_LBA;
    ioctl(file_handle,CDROMSUBCHNL,&sub_channel);
    return sub_channel.cdsc_trk;
#endif
}

int CL_CDDrive_Linux::get_cur_frame()
{
	#ifdef __FreeBSD__
	ioc_read_subchannel sub_channel;
	cd_sub_channel_info sub_info;
	sub_channel.data = &sub_info;
	sub_channel.address_format = CD_MSF_FORMAT;
	sub_channel.data_format = CD_CURRENT_POSITION;
	sub_channel.data_len = sizeof(sub_info);
	ioctl(file_handle,CDIOCREADSUBCHANNEL,&sub_channel);
	return sub_channel.data->what.position.absaddr.msf.frame;
	#else
    struct cdrom_subchnl sub_channel;

    sub_channel.cdsc_format=CDROM_LBA;
    ioctl(file_handle,CDROMSUBCHNL,&sub_channel);
    return sub_channel.cdsc_reladdr.lba;
	#endif
}

bool CL_CDDrive_Linux::play_tracks(int track, int end_track)
{
    if((track < first_track) || (track > last_track))
        return false;
	if((end_track < track) || (end_track > last_track))
	    return false;
	  #ifdef __FreeBSD__
		ioc_play_msf msf;
	#else  
    struct cdrom_msf msf;
	#endif

	#ifdef __FreeBSD__
    msf.start_m = tracks[track-1].start_lba/CD_FRAMES/CD_SECS;
    msf.start_s = (tracks[track-1].start_lba/CD_FRAMES%CD_SECS)+2;
    msf.start_f = tracks[track-1].start_lba%CD_FRAMES;
    
    msf.end_m
=(tracks[end_track-1].start_lba+tracks[end_track-1].track_length)/75/60; 
   
msf.end_s=((tracks[end_track-1].start_lba+tracks[end_track-1].track_length)/75%60)+2;    

msf.end_f=(tracks[end_track-1].start_lba+tracks[end_track-1].track_length)%75;
	 	 #else	

    msf.cdmsf_min0 = tracks[track-1].start_lba/CD_FRAMES/CD_SECS;
    msf.cdmsf_sec0 = (tracks[track-1].start_lba/CD_FRAMES%CD_SECS)+2;
    msf.cdmsf_frame0 = tracks[track-1].start_lba%CD_FRAMES;
    
    msf.cdmsf_min1 = (tracks[end_track-1].start_lba+tracks[end_track-1].track_length)/CD_FRAMES/CD_SECS;
    msf.cdmsf_sec1 = ((tracks[end_track-1].start_lba+tracks[end_track-1].track_length)/CD_FRAMES%CD_SECS)+2;
    msf.cdmsf_frame1 = (tracks[end_track-1].start_lba+tracks[end_track-1].track_length)%CD_FRAMES;
	#endif	
	if(ioctl(file_handle,CDROMPLAYMSF,&msf))
	{
		std::string err;
		err += "Could not play track " + track;
		
		cl_info(info_sound, err.c_str());
		return false;
	}
	    
	return true;
}

bool CL_CDDrive_Linux::play_frames(int frame, int end_frame)
{
#ifdef __FreeBSD__
	ioc_play_msf msf;
#else
    struct cdrom_msf msf;
#endif

#ifdef __FreeBSD__
 
    msf.start_m = frame/CD_FRAMES/CD_SECS;
    msf.start_s = (frame/CD_FRAMES%CD_SECS)+2;
    msf.start_f  = frame%CD_FRAMES;
    
    msf.end_m = end_frame/CD_FRAMES/CD_SECS;
    msf.end_s = (end_frame/CD_FRAMES%CD_SECS)+2;
    msf.end_f = end_frame%CD_FRAMES;
#else
    
    msf.cdmsf_min0 = frame/CD_FRAMES/CD_SECS;
    msf.cdmsf_sec0 = (frame/CD_FRAMES%CD_SECS)+2;
    msf.cdmsf_frame0 = frame%CD_FRAMES;
    
    msf.cdmsf_min1 = end_frame/CD_FRAMES/CD_SECS;
    msf.cdmsf_sec1 = (end_frame/CD_FRAMES%CD_SECS)+2;
    msf.cdmsf_frame1 = end_frame%CD_FRAMES;
	
#endif
	if(ioctl(file_handle,CDROMPLAYMSF,&msf))
	{
		CL_String err;

		err << "Could not play from frame " << frame << " to frame " << end_frame << '\0';
		
		cl_info(info_sound, static_cast<const char*>(err));
        return false;
	}
	return true;
}

bool CL_CDDrive_Linux::play_track(int track)
{
    if((track< first_track)||(track > last_track))
        return false;
#ifdef __FreeBSD__
    ioc_play_msf msf;
    msf.start_m = tracks[track-1].start_lba/CD_FRAMES/CD_SECS;
    msf.start_s = (tracks[track-1].start_lba/CD_FRAMES%CD_SECS)+2;
    msf.start_f = tracks[track-1].start_lba%CD_FRAMES;
    
    msf.end_m =
(tracks[track-1].start_lba+tracks[track-1].track_length)/CD_FRAMES/CD_SECS;   
 msf.end_s =
((tracks[track-1].start_lba+tracks[track-1].track_length)/CD_FRAMES%CD_SECS)+2;
   msf.start_f =
(tracks[track-1].start_lba+tracks[track-1].track_length)%CD_FRAMES;
#else	     
struct cdrom_msf msf;     msf.cdmsf_min0 =
tracks[track-1].start_lba/CD_FRAMES/CD_SECS;     msf.cdmsf_sec0 =
(tracks[track-1].start_lba/CD_FRAMES%CD_SECS)+2;     msf.cdmsf_frame0 =
tracks[track-1].start_lba%CD_FRAMES;
    msf.cdmsf_min1 = (tracks[track-1].start_lba+tracks[track-1].track_length)/CD_FRAMES/CD_SECS;
    msf.cdmsf_sec1 = ((tracks[track-1].start_lba+tracks[track-1].track_length)/CD_FRAMES%CD_SECS)+2;
    msf.cdmsf_frame1 = (tracks[track-1].start_lba+tracks[track-1].track_length)%CD_FRAMES;
	
#endif
	if(ioctl(file_handle,CDROMPLAYMSF,&msf))
	{
		CL_String err;
		err << "Could not play track " << track << '\0';
		
		cl_info(info_sound, static_cast<const char*>(err));
		return false;
	}
	return true;
}

void CL_CDDrive_Linux::stop()
{
    ioctl(file_handle,CDROMSTOP);
}

void CL_CDDrive_Linux::pause()
{
    ioctl(file_handle,CDROMPAUSE);
}

void CL_CDDrive_Linux::resume()
{
    ioctl(file_handle,CDROMRESUME);
}

bool CL_CDDrive_Linux::readtoc()
{
#ifdef __FreeBSD__
	ioc_toc_header tochdr;
#else
    struct cdrom_tochdr tochdr;
#endif
    // Read the TOC header
    if(ioctl(file_handle, CDROMREADTOCHDR, &tochdr))
    {
		close(file_handle);
		
		CL_String err;
		err << "Could not read table of contents in Device " << filename << '\0';
		throw CL_Error(static_cast<const char*>(err));
		file_handle = -1;
    }
    
#ifdef __FreeBSD__
	first_track = tochdr.starting_track;
	last_track = tochdr.ending_track;
#else
    // get the first and last tracks
    first_track = tochdr.cdth_trk0;
    last_track = tochdr.cdth_trk1;
#endif

    num_tracks = last_track - first_track +1;

#ifdef __FreeBSD__
	ioc_read_toc_single_entry cur_track_info;
	ioc_read_toc_single_entry next_track_info;
#else
    struct cdrom_tocentry cur_track_info;
    struct cdrom_tocentry next_track_info;
#endif
	// read TOC for first track
	if(!readtocentry(first_track,&cur_track_info))
	    return false;
	
    // read the TOC Entry for each track
    for(int i = first_track+1;i<=last_track+1;i++)
    {
        STrack track;
	
	    // read for next track
	    if(!readtocentry(i,&next_track_info))
	        return false;
	    
#ifdef __FreeBSD__
			track.start_lba = cur_track_info.entry.addr.lba
			track.track_length = next_track_info.entry.data_len;
#else
	    track.start_lba = cur_track_info.cdte_addr.lba;
	    track.track_length = next_track_info.cdte_addr.lba - cur_track_info.cdte_addr.lba;
#endif
	    track.is_audio = true;
#ifdef __FreeBSD__
			tracks.push_back(track);

			cur_track_info.entry.addr.lba = next_track_info.entry.addr.lba;
#else
	    if(cur_track_info.cdte_ctrl == CDROM_DATA_TRACK)
	        track.is_audio = false;
			
		
		tracks.push_back(track);
		cur_track_info.cdte_addr.lba = next_track_info.cdte_addr.lba;
		cur_track_info.cdte_ctrl     = next_track_info.cdte_ctrl;
#endif
    }
    return true;
}

#ifdef __FreeBSD__
bool CL_CDDrive_Linux::readtocentry(int track_no, struct
ioc_read_toc_single_entry* track_info) { 	
	track_info->track = track_no;
	track_info->address_format = CD_LBA_FORMAT;
	if(ioctl(file_handle,CDIOREADTOCENTRY,track_info) )
	{ 
		std::string err;
		err += "Could not read table of contents in Device " + filename;
		cl_info(info_sound, err.c_str ());
		return false;
	}
	return true;
}

#else
bool CL_CDDrive_Linux::readtocentry(int track_no, struct cdrom_tocentry* track_info)
{
    track_info->cdte_track = track_no;
    
    // check for leadout
    if (track_no==last_track+1)
	    track_info->cdte_track = CDROM_LEADOUT;
    
    // We want the LBA format so that we can switch our play format
    track_info->cdte_format = CDROM_LBA;
    
    if(ioctl(file_handle, CDROMREADTOCENTRY, track_info))
    {
	    std::string err;
	    err += "Could not read table of contents in Device " + filename;
	    cl_info(info_sound,err.c_str());
        return false;
    }
    
    return true;
} 
#endif
