/*
	$Id: inputsource_zipped.cpp,v 1.3 2001/09/08 19:12:44 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Core/IOData/inputsource_zipped.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Zipped_Generic

class CL_InputSource_Zipped_Generic
{
public:
	CL_InputSource *input;
	bool delete_input;
};

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Zipped Construction:

CL_InputSource_Zipped::CL_InputSource_Zipped(
	CL_InputSource *input,
	bool delete_input)
:
	impl(new CL_InputSource_Zipped_Generic)
{
	impl->input = input;
	impl->delete_input = delete_input;
}

CL_InputSource_Zipped::~CL_InputSource_Zipped()
{
	if (impl->delete_input) delete impl->input;
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Zipped Attributes:

int CL_InputSource_Zipped::tell() const
{
	return impl->input->tell();
}

int CL_InputSource_Zipped::size() const
{
	return impl->input->size();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Zipped Operations:

int CL_InputSource_Zipped::read(void *data, int size)
{
	return impl->input->read(data, size);
}

void CL_InputSource_Zipped::open()
{
	impl->input->open();
}

void CL_InputSource_Zipped::close()
{
	impl->input->close();
}

CL_InputSource *CL_InputSource_Zipped::clone() const
{
	return new CL_InputSource_Zipped(impl->input->clone(), true);
}

void CL_InputSource_Zipped::seek(int pos, SeekEnum seek_type)
{
	impl->input->seek(pos, seek_type);
}

void CL_InputSource_Zipped::push_position()
{
	impl->input->push_position();
}

void CL_InputSource_Zipped::pop_position()
{
	impl->input->pop_position();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Zipped Implementation:
