/*
	$Id: vorbis_soundprovider_generic.h,v 1.7 2000/10/21 13:38:58 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

//! component="Vorbis"

#ifndef header_vorbis_soundprovider_generic
#define header_vorbis_soundprovider_generic

#include "API/Vorbis/vorbis_soundprovider.h"
#include "vorbis/codec.h"

class CL_VorbisSoundProvider_Generic
{
public:
	std::string name;
	CL_InputSourceProvider *provider;
	bool delete_provider;
};

class CL_VorbisStream : public CL_StreamSoundProvider_Session
{
public:
	CL_VorbisStream(CL_InputSource *input);
	virtual ~CL_VorbisStream();

	virtual bool eof() const;
	virtual void stop();
	virtual bool play();
	virtual bool set_position(int pos);
	virtual int get_data(void *data_ptr, int data_requested);
	virtual int get_frequency() const;
	virtual SoundFormat get_format() const;

private:
	void init();
	void deinit();
	void stream_data();
	
	bool inited;
	CL_InputSource *input;
	bool stream_eof;

	ogg_sync_state   oy; /* sync and verify incoming physical bitstream */
	ogg_stream_state os; /* take physical pages, weld into a logical
	                        stream of packets */
	ogg_page         og; /* one Ogg bitstream page.  Vorbis packets are inside */
	ogg_packet       op; /* one raw packet of data for decode */

	vorbis_info      vi; /* struct that stores all the static vorbis bitstream
	                        settings */
	vorbis_comment   vc; /* struct that stores all the bitstream user comments */
	vorbis_dsp_state vd; /* central working state for the packet->PCM decoder */
	vorbis_block     vb; /* local working space for packet->PCM decode */
};

#endif
