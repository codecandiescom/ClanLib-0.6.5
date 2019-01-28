/*
	$Id: setupmikmod.cpp,v 1.7 2002/01/28 11:08:03 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#include "API/MikMod/streamed_mikmod_sample.h"
#include "resourcetype_module.h"
#include "API/MikMod/setupmikmod.h"

static CL_RegisterResourceType<CL_ResourceData_Module> *restype_module = NULL;

static int ref_count = 0;

/*
 * This structure is the ClanLib driver for MikMod. It's a "standard" driver
 * since it re-uses lots of functions from libMikMod, indeed the big
 * ClanLib-specific stuff is the update function, which is mapped to
 * CL_Streamed_MikModSample_Session::clanMikMod_Update
 */
MDRIVER drv_clanlib = {
	NULL,
	"clanMikMod driver",
	"0.1",
	255,
	255, 
	"clanmikmod",
	NULL, // Command line
	CL_Streamed_MikModSample_Session::clanMikMod_IsPresent, // IsPresent
	VC_SampleLoad,
	VC_SampleUnload,
	VC_SampleSpace,
	VC_SampleLength,
	VC_Init,
	VC_Exit,
	NULL, // Reset
	VC_SetNumVoices,
	VC_PlayStart,
	VC_PlayStop,
	CL_Streamed_MikModSample_Session::clanMikMod_Update,
	NULL, // Pause
	VC_VoiceSetVolume,
	VC_VoiceGetVolume,
	VC_VoiceSetFrequency,
	VC_VoiceGetFrequency,
	VC_VoiceSetPanning,
	VC_VoiceGetPanning,
	VC_VoicePlay,
	VC_VoiceStop,
	VC_VoiceStopped,
	VC_VoiceGetPosition,
	VC_VoiceRealVolume
};


void CL_SetupMikMod::init(bool register_resources_only)
{
	ref_count++;
	if (ref_count > 1) return;

	restype_module = new CL_RegisterResourceType<CL_ResourceData_Module>("module");

	/* Register the loaders we want to use:  */
	MikMod_RegisterAllLoaders();
	
	/* Register the drivers we want to use: */
	MikMod_RegisterDriver(&drv_clanlib);

#ifdef WIN32
	MikMod_Init("");
#else
#if LIBMIKMOD_VERSION >= ((3 << 16) | (1 << 8) | (7))
	MikMod_Init("");
#else
	MikMod_Init();
#endif
#endif

	md_mode |= DMODE_INTERP;
	
	// disable the reverb
	md_reverb=0;

	/*
	 * OK, now we kind of ignore the "register_resources_only" 
	 * parameter. Indeed, MikMod_Init seems to be required for
	 * the module loader to be able to work. However, it's not
	 * a big problem since MikMod_Init() does not require any
	 * peculiar hardware and/or settings, the hardware being
	 * accessed through clanSound anyway. In fact, MikMod is
	 * only used to read modules and transform them into raw
	 * sound. Then clanSound handles everything else. So
	 * register_resources_only is of no use in this function,
	 * but is kept for API consistency.
	 */
	// if (register_resources_only) return;
}

void CL_SetupMikMod::deinit()
{
	ref_count--;
	if (ref_count > 0) return;

	MikMod_Exit();

	if (restype_module) delete restype_module;
	
	restype_module = NULL;
}
