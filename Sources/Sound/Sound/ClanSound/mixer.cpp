/*
	$Id: mixer.cpp,v 1.7 2001/11/21 23:18:51 mbn Exp $
	
	ClanSound, sound mixing library written for ClanLib.
	Copyright (c)1998 Magnus Norddahl / ClanSoft.
*/

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#ifdef USE_CLANSOUND

#include <Sound/Sound/ClanSound/mixer.h>
#include <Sound/Sound/ClanSound/cardplayback_clan.h>

#ifdef USE_I386_ASSEMBLER
#if defined(__BORLANDC__) && !defined(USE_TASM)
  #undef USE_I386_ASSEMBLER
#else
  #define MIXER_CLIP_ASM
#endif
#endif

#ifdef MIXER_CLIP_ASM
extern "C"
{
	void mixer_clip_asm(
		int *mix_buffer2,
		short *mix_buffer,
		int size);
}
#endif

CL_CSMixer::CL_CSMixer()
{
	mix_buffer_size = output.get_frag_size()*2;
	mix_buffer = new short[mix_buffer_size];
	mix_buffer2 = new int[mix_buffer_size];
}

CL_CSMixer::~CL_CSMixer()
{
	delete[] mix_buffer;
	delete[] mix_buffer2;
  mix_buffer = NULL;
  mix_buffer2 = NULL;
}

void CL_CSMixer::add(CL_CardPlayback_ClanSound *playback)
{
	playbacks.push_back(playback);
}

void CL_CSMixer::remove(CL_CardPlayback_ClanSound *playback)
{
	playbacks.remove(playback);
}

void CL_CSMixer::wait()
{
	output.wait();
}

void CL_CSMixer::mix(CL_Mutex *mutex)
{
	
	memset(mix_buffer2, 0, sizeof(int)*mix_buffer_size);

//	int num_sessions = playbacks.size();
//	for (int i=0; i<num_sessions; i++)

	mutex->enter();
	for (
		std::list<CL_CardPlayback_ClanSound*>::iterator it = playbacks.begin();
		it != playbacks.end();
		it++)
	{
		if ((*it)->is_playing() == false) continue;

		(*it)->mix_to(mix_buffer2, mix_buffer_size >> 1);
	}
	mutex->leave();

#ifdef MIXER_CLIP_ASM
	mixer_clip_asm(mix_buffer2, mix_buffer, mix_buffer_size);
#else
	for (int k=0; k<mix_buffer_size; k++)
	{
		if      (mix_buffer2[k] > 32767)  mix_buffer[k] = 32767;
		else if (mix_buffer2[k] < -32767) mix_buffer[k] = -32767;
		else                              mix_buffer[k] = mix_buffer2[k];
	}
#endif

	output.write_fragment(mix_buffer);
}

#endif


