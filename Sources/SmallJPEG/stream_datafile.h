/*
	$Id: stream_datafile.h,v 1.3 2001/09/08 19:24:21 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#ifndef header_stream_datafile
#define header_stream_datafile

#include "jpgd/jpegdecoder.h"
#include "API/Core/IOData/inputsource.h"

class jpeg_decoder_datafile_stream : public jpeg_decoder_stream
{
public:
	jpeg_decoder_datafile_stream(CL_InputSource *_input, bool delete_after_usage)
	{
		if (delete_after_usage) input = _input;
		else input = _input->clone();
	}

	~jpeg_decoder_datafile_stream()
	{
		delete input;
	}

	virtual int read(uchar *Pbuf, int max_bytes_to_read, bool *Peof_flag)
	{
		int r = input->read(Pbuf, max_bytes_to_read);
		if (r < max_bytes_to_read) *Peof_flag = true;
		return r;
	}

private:
	CL_InputSource *input;
};

#endif
