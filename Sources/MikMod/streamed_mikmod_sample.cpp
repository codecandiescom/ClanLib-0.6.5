/*
	$Id: streamed_mikmod_sample.cpp,v 1.9 2001/12/11 20:44:23 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Streamed sample

*/

#include "API/MikMod/streamed_mikmod_sample.h"
#include "resourcetype_module.h"
#include "module_reader.h"



/*****************************
      CL_Streamed_MikModSample
*****************************/

CL_SoundBuffer *CL_Streamed_MikModSample::create(
	const std::string &filename,
	CL_InputSourceProvider *inputprovider, 
	bool looped)
{
	return CL_SoundBuffer::create(new CL_Streamed_MikModSample(filename, inputprovider, looped), true);
}

CL_SoundBuffer *CL_Streamed_MikModSample::load(
	const std::string &res_id,
	CL_ResourceManager *manager, 
	bool looped)
{
	return CL_SoundBuffer::create(new CL_Streamed_MikModSample(res_id, manager, looped), true);
}

CL_Streamed_MikModSample::CL_Streamed_MikModSample(
	const std::string &_filename, 
	CL_InputSourceProvider *_inputprovider, 
	bool _looped)
{
	loaded_from_resource=false;

	CL_InputSourceProvider *inputprovider;
	filename = _filename;
	looped = _looped;

	if (_inputprovider == NULL)
	{
		inputprovider = CL_InputSourceProvider::create_file_provider("");
	}
	else
	{
		inputprovider = _inputprovider->clone();
	}

	/*
	 * Here we use the same method than when loading the module
	 * from a .scr file. Still there's a difference, since here
	 * we don't use the resource mecanism and therefore will
	 * need to free the module manually later
	 */

	CL_InputSource *input=inputprovider->open_source(filename.c_str());
	cl_assert ( input != NULL );

	MREADER *reader=new_clanlib_reader((void *) input);
	cl_assert ( reader != NULL );

	module = Player_LoadGeneric(reader,CLANLIB_READER_CHANNELS,0);

	if (module == NULL)
	{
		throw CL_Error(MikMod_strerror(MikMod_errno));
	}

	delete_clanlib_reader(reader);
	delete input;
	delete inputprovider;
}

CL_Streamed_MikModSample::CL_Streamed_MikModSample(
	const std::string &_res_id, 
	CL_ResourceManager *_manager, 
	bool _looped)
{
	loaded_from_resource=true;

        manager=_manager;
	filename=_res_id;

	looped=_looped;
	CL_Resource res;
	CL_ResourceData_Module *resdata;
	res=manager->get_resource(filename);
	res.load();
	resdata=(CL_ResourceData_Module *) res.get_data("module");
	module=resdata->module;
}

CL_Streamed_MikModSample::~CL_Streamed_MikModSample()
{
	if (loaded_from_resource)
	{
		/*
		 * The sample has been loaded from a resource file therefore
		 * we call unload(), and if needed the resource manager
		 * will free the module
		 */
		CL_Resource res;
		res=manager->get_resource(filename);
		res.unload();
	}
	else
	{
		/*
		 * The sample has been loaded directly from a file,
		 * we free it right away.
		 */
		Player_Free(module);
	}
}

CL_StreamSoundProvider_Session *CL_Streamed_MikModSample::begin_session()
{
	return new CL_Streamed_MikModSample_Session(module, looped);
}

void CL_Streamed_MikModSample::end_session(CL_StreamSoundProvider_Session *session)
{
	delete session;
}


/*****************************
  CL_Streamed_MikModSample_Session
*****************************/

CL_Streamed_MikModSample_Session::CL_Streamed_MikModSample_Session(
	MODULE *_module,
	bool _looped)
{
	module=_module;
	looped=_looped;

	cl_assert (module != NULL);

	//	module->loop=looped;
	module->wrap=looped;

	Player_Start (module);

	sample_freq = md_mixfreq;

	int bytes_per_sample = ((md_mode&DMODE_STEREO)?2:1) * ((md_mode&DMODE_16BITS)?2:1);

	if (((md_mode&DMODE_STEREO)?2:1) == 2 && bytes_per_sample == 4) sample_format = sf_16bit_signed_stereo;
	else if (((md_mode&DMODE_STEREO)?2:1) == 2 && bytes_per_sample == 2) sample_format = sf_8bit_signed_stereo;
	else if (((md_mode&DMODE_STEREO)?2:1) == 1 && bytes_per_sample == 2) sample_format = sf_16bit_signed;
	else if (((md_mode&DMODE_STEREO)?2:1) == 1 && bytes_per_sample == 1) sample_format = sf_8bit_signed;
	else
	{
		std::cout << "    Invalid wave file format         " << std::endl;
		std::cout << "---------------------------------" << std::endl;
		std::cout << "Sample size: " << sample_size << std::endl;
		std::cout << "Sample frequency: " << sample_freq << std::endl;
		std::cout << "Number of channels: " << ((md_mode&DMODE_STEREO)?2:1) << std::endl;
		std::cout << "Number of bytes pr. sample: " << bytes_per_sample << std::endl;
		std::cout << "---------------------------------" << std::endl;

		throw CL_Error("Invalid wave file format");
	}
}

CL_Streamed_MikModSample_Session::~CL_Streamed_MikModSample_Session()
{
	Player_Stop();
}

bool CL_Streamed_MikModSample_Session::eof() const
{
	return !Player_Active();
}

bool CL_Streamed_MikModSample_Session::set_position(int pos)
{
	Player_SetPosition(pos);
	return true; // (Player_GetPosition() == pos); does not exist, what to do?
}

bool CL_Streamed_MikModSample_Session::play()
{
	if (Player_Active())
		return false;
		
	Player_Start(module);
	return Player_Active() != 0;
}

void CL_Streamed_MikModSample_Session::stop()
{
	Player_Stop();
}

int CL_Streamed_MikModSample_Session::get_data(void *data_ptr, int data_requested)
{
	MikMod_Update();
	int read = VC_WriteBytes((SBYTE*)data_ptr,data_requested);

	return read;
}

int CL_Streamed_MikModSample_Session::get_frequency() const
{
	return sample_freq;
}

SoundFormat CL_Streamed_MikModSample_Session::get_format() const
{
	return sample_format;
}

// EOF //
