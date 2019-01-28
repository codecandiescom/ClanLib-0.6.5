/*
	$Id: vorbis_soundprovider_generic.cpp,v 1.14 2002/03/18 06:31:57 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

//! component="Vorbis"

#include "vorbis_soundprovider_generic.h"


CL_VorbisSoundProvider::CL_VorbisSoundProvider(
	const std::string &name,
	CL_InputSourceProvider *provider,
	bool delete_provider)
{
	impl = new CL_VorbisSoundProvider_Generic;
	impl->name = name;
	impl->provider = provider;
	impl->delete_provider = delete_provider;
	
	if (provider == NULL)
	{
		impl->provider = CL_InputSourceProvider::create_file_provider("");
		impl->delete_provider = true;
	}
}

CL_VorbisSoundProvider::CL_VorbisSoundProvider(
	const std::string &name,
	CL_ResourceManager *resources)
{
	impl = new CL_VorbisSoundProvider_Generic;
	impl->name = name;
	impl->provider = resources->get_resource_provider();
}

CL_VorbisSoundProvider::~CL_VorbisSoundProvider()
{
	if (impl->delete_provider) delete impl->provider;
	delete impl;
}

CL_StreamSoundProvider_Session *CL_VorbisSoundProvider::begin_session()
{
	return new CL_VorbisStream(impl->provider->open_source(impl->name.c_str()));
}

void CL_VorbisSoundProvider::end_session(CL_StreamSoundProvider_Session *session)
{
	delete session;
}

// -----------------------------------------------------------------------------

CL_VorbisStream::CL_VorbisStream(CL_InputSource *_input)
: inited(false), input(_input), stream_eof(false)
{
	init();
}

CL_VorbisStream::~CL_VorbisStream()
{
	deinit();
	delete input;
}

void CL_VorbisStream::init()
{
	if (inited) return;

	input->seek(0, CL_InputSource::seek_set);

	ogg_sync_init(&oy); /* Now we can read pages */

	char *buffer;
	int bytes;
	
	buffer = ogg_sync_buffer(&oy,4096);
	bytes = input->read(buffer, 4096);
	ogg_sync_wrote(&oy,bytes);

	/* Get the first page. */
	if (ogg_sync_pageout(&oy,&og) !=1)
	{
		/* error case.  Must not be Vorbis data */
		throw CL_Error("Input does not appear to be an Ogg bitstream.");
	}
	
	/* Get the serial number and set up the rest of decode. */
	/* serialno first; use it to set up a logical stream */
	ogg_stream_init(&os,ogg_page_serialno(&og));

	vorbis_info_init(&vi);
	vorbis_comment_init(&vc);
	if (ogg_stream_pagein(&os,&og)<0)
	{ 
		/* error; stream version mismatch perhaps */
		throw CL_Error("Error reading first page of Ogg bitstream data.");
	}
	
	if (ogg_stream_packetout(&os,&op)!=1)
	{ 
		/* no page? must not be vorbis */
		throw CL_Error("Error reading initial header packet.");
	}
    
	if (vorbis_synthesis_headerin(&vi,&vc,&op)<0)
	{ 
		/* error case; not a vorbis header */
		throw CL_Error("This Ogg bitstream does not contain Vorbis "
		      "audio data.");
	}
    
	/* At this point, we're sure we're Vorbis.  We've set up the logical
	   (Ogg) bitstream decoder.  Get the comment and codebook headers and
	   set up the Vorbis decoder */
    
	/* The next two packets in order are the comment and codebook headers.
	   They're likely large and may span multiple pages.  Thus we reead
	   and submit data until we get our two pacakets, watching that no
	   pages are missing.  If a page is missing, error out; losing a
	   header page is the only place where missing data is fatal. */
    
	int i=0;
	while (i<2)
	{
		while (i<2)
		{
			int result=ogg_sync_pageout(&oy,&og);
			if(result==0) break; /* Need more data */
			/* Don't complain about missing or corrupt data yet.  We'll
			   catch it at the packet output phase */
			if(result==1)
			{
				ogg_stream_pagein(&os,&og);
				/* we can ignore any errors here
				   as they'll also become apparent
				   at packetout */

				while(i<2)
				{
					result=ogg_stream_packetout(&os,&op);
					if(result==0)break;
					if(result==-1)
					{
						/* Uh oh; data at some point was corrupted or missing!
						We can't tolerate that in a header.  Die. */
						throw CL_Error("Corrupt secondary header.");
					}
					vorbis_synthesis_headerin(&vi,&vc,&op);
					i++;
				}
			}
		}
		/* no harm in not checking before adding more */
		buffer=ogg_sync_buffer(&oy,4096);
		bytes=input->read(buffer,4096);
		if(bytes==0 && i<2)
		{
			throw CL_Error("End of file before finding all Vorbis headers!");
		}
		ogg_sync_wrote(&oy,bytes);
	}
    
	/* Throw the comments plus a few lines about the bitstream we're
	decoding */
/*	{
		char **ptr=vc.user_comments;
		while(*ptr)
		{
			fprintf(stderr,"%s\n",*ptr);
			++ptr;
		}
		fprintf(stderr,"\nBitstream is %d channel, %ldHz\n",vi.channels,vi.rate);
		fprintf(stderr,"Encoded by: %s\n\n",vc.vendor);
	}
    
	convsize=4096/vi.channels;
*/
//	std::cout << "vc.vendor: " << vc.vendor << std::endl;
	
	/* OK, got and parsed all three headers. Initialize the Vorbis
	packet->PCM decoder. */
	vorbis_synthesis_init(&vd,&vi); /* central decode state */
	vorbis_block_init(&vd,&vb);     /* local state for most of the decode
				       so multiple block decodes can
				       proceed in parallel.  We could init
				       multiple vorbis_block structures
				       for vd here */
}

void CL_VorbisStream::deinit()
{
	if (!inited) return;

	/* clean up this logical bitstream; before exit we see if we're
	followed by another [chained] */
	
	ogg_stream_clear(&os);
	
	/* ogg_page and ogg_packet structs always point to storage in
	libvorbis.  They're never freed or manipulated directly */
	
	vorbis_block_clear(&vb);
	vorbis_dsp_clear(&vd);
	vorbis_info_clear(&vi);  /* must be called last */

	/* OK, clean up the framer */
	ogg_sync_clear(&oy);

	delete input;
}

bool CL_VorbisStream::eof() const
{
	return stream_eof;
}

void CL_VorbisStream::stop()
{
}

bool CL_VorbisStream::play()
{
	if (stream_eof)
	{
		deinit();
		stream_eof = false;
		init();
	}
	return true;
}

bool CL_VorbisStream::set_position(int pos)
{
	return false;
}

int CL_VorbisStream::get_data(void *_data_ptr, int data_requested)
{
	data_requested /= sizeof(short) * vi.channels;
	
	short *data_ptr = (short *) _data_ptr;
	int data_left = data_requested;

	while (!eof() && data_left > 0)
	{
		float **pcm;

		int samples = vorbis_synthesis_pcmout(&vd, &pcm);
		if (samples <= 0)
		{
			stream_data();
			continue;
		}
		
		if (samples > data_left) samples = data_left;

		for (int i=0; i<samples; i++)
		{
			for (int j=0; j<vi.channels; j++)
			{
				int s = (int) (pcm[j][i] * 32767);
				if (s < -32767) s = -32767;
				else if (s > 32767) s = 32767;
				*(data_ptr++) = (short) s;
			}
		}
		
		vorbis_synthesis_read(&vd, samples);
		data_left -= samples;
	}
	
	return (data_requested - data_left) * sizeof(short) * vi.channels;
}

void CL_VorbisStream::stream_data()
{
	while (!eof())
	{
		int result=ogg_stream_packetout(&os,&op);
		if (result > 0)
		{
			if(vorbis_synthesis(&vb,&op)==0) /* test for success! */
				vorbis_synthesis_blockin(&vd,&vb);
			break;
		}
		
		while (!eof())
		{
			int result = ogg_sync_pageout(&oy,&og);
			if (result == -1) continue; // corrupt data at this page position. Read next.
			if (result == 0) // need more data
			{
				char *buffer=ogg_sync_buffer(&oy,4096);
				int bytes=input->read(buffer,4096);
				ogg_sync_wrote(&oy,bytes);
				if (bytes==0)
				{
					stream_eof = true;
					return;
				}
			}
			else
			{
				ogg_stream_pagein(&os,&og);
				break;
			}
		}
	}
}
	
int CL_VorbisStream::get_frequency() const
{
	return vi.rate;
}

SoundFormat CL_VorbisStream::get_format() const
{
	return (vi.channels == 2) ? sf_16bit_signed_stereo : sf_16bit_signed;
}
