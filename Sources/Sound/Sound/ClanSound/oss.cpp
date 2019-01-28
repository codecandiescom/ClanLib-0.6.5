/*
	$Id: oss.cpp,v 1.7 2002/07/02 20:56:33 mbn Exp $
	
	ClanSound, sound mixing library written for ClanLib.
	Copyright (c)1998 Magnus Norddahl / ClanSoft.

	OSS Documentation can be found at http://www.opensound.com
*/

#include "Core/precomp.h"

#ifdef USE_CLANSOUND

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#ifndef Solaris
#include <sys/soundcard.h>
#define DSP_DEVICE "/dev/dsp"
#else
#include "soundcard.h" // we don't have any soundcard.h on Solaris by default
#define DSP_DEVICE "/dev/audio"
#endif		       
#include <iostream>

#include <API/Core/System/error.h>
#include "API/Core/System/cl_assert.h"
#include "API/Core/System/system.h"
#include <Sound/Sound/ClanSound/oss.h>

// If USE_DRIVER_FRAGSIZE is defined, we use the fragsize (and number of them)
// used by the soundcard driver. This is not as efficient as using our own
// settings - mainly provided for debugging.
// #define USE_DRIVER_FRAGSIZE

bool has_sound = true;

CL_CSOutput::CL_CSOutput()
{
	dev_dsp_fd = open(DSP_DEVICE, O_WRONLY|O_NONBLOCK);
	if (dev_dsp_fd == -1)
	{
		has_sound = false;
		frag_size = 22050/2;
		return;
//		throw CL_Error("Could not open audio device. No sound will be available.");
	}
	fcntl(dev_dsp_fd, F_SETFL, fcntl(dev_dsp_fd, F_GETFL) &~ O_NONBLOCK);
	
#ifndef USE_DRIVER_FRAGSIZE
	int frag_settings = 0x0003000b; // 0xMMMMSSSS
		// (where MMMM = num fragments, SSSS = fragment size)

	if (ioctl(dev_dsp_fd, SNDCTL_DSP_SETFRAGMENT, &frag_settings))
	{
		std::cout << "ClanSound: Failed to set soundcard fragment size. Sound may have a long latency." << std::endl;
	}

#endif
	
	int format = AFMT_S16_NE;
	ioctl(dev_dsp_fd, SNDCTL_DSP_SETFMT, &format);
	if (format != AFMT_S16_NE)
	{
		close(dev_dsp_fd);
		throw CL_Error("Requires 16 bit soundcard. No sound will be available.");
	}
	
	int stereo = 1;
	ioctl(dev_dsp_fd, SNDCTL_DSP_STEREO, &stereo);
	if (stereo != 1)
	{
		close(dev_dsp_fd);
		throw CL_Error("ClanSound: Requires 16 bit stereo capable soundcard. No sound will be available.");
	}
	
	int speed = 22050;
	ioctl(dev_dsp_fd, SNDCTL_DSP_SPEED, &speed);

	float percent_wrong = speed / (float) 22050;
	if (percent_wrong < 0.90 || percent_wrong > 1.10)
	{
		close(dev_dsp_fd);
		throw CL_Error("ClanSound: Mixing rate (22.05 kHz) not supported by soundcard.");
	}
	
	// Try to improve mixing performance by using the same mixing buffer size
	// as the sound device does:
	int err = ioctl(dev_dsp_fd, SNDCTL_DSP_GETBLKSIZE, &frag_size);
	if (err == -1)
	{
		std::cout << "ClanSound: Warning, Couldn't get sound device blocksize. Using 0.25 sec mixing buffer." << std::endl;
		frag_size = 22050/2; // 0.25 sec mixing buffer used.
	}

	audio_buf_info info;
	ioctl(dev_dsp_fd, SNDCTL_DSP_GETOSPACE, &info);
}

CL_CSOutput::~CL_CSOutput()
{
	if (dev_dsp_fd != -1) close(dev_dsp_fd);
}

void CL_CSOutput::silence()
// Called when we have no samples to play - and wants to tell the soundcard
// about this possible event.
{
	ioctl(dev_dsp_fd, SNDCTL_DSP_POST, 0);
}

bool CL_CSOutput::is_full()
// Returns true if all fragments are filled with data.
{
	if (!has_sound) return false;
	audio_buf_info info;
	int err = ioctl(dev_dsp_fd, SNDCTL_DSP_GETOSPACE, &info);
	if (err == -1)
	{
		std::cout << "ClanSound: fragments free not supported by device!?" << std::endl;
		return false; // not supported by device!?
	}

	if (info.fragments == 0) return true;
	return false;
}

int CL_CSOutput::get_frag_size()
// Returns the buffer size used by device (returned as num [stereo] samples).
{
	return frag_size/4;
}

void CL_CSOutput::write_fragment(short *data)
// Writes a fragment to the soundcard.
{
	write(dev_dsp_fd, data, frag_size);
}

void CL_CSOutput::wait()
{
	if(!has_sound)
	{
		CL_System::sleep(100);
		return;
	}

	fd_set wfds;

	FD_ZERO(&wfds);
	FD_SET(dev_dsp_fd, &wfds);

	/*int retval =*/ select(dev_dsp_fd+1, NULL, &wfds, NULL, NULL);
//	cl_assert(retval != -1);
}

#endif
