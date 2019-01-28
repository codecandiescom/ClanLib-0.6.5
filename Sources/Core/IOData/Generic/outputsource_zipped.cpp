/*
	$Id: outputsource_zipped.cpp,v 1.3 2001/09/08 19:12:44 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Core/IOData/outputsource_zipped.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OutputSource_Zipped_Generic

class CL_OutputSource_Zipped_Generic
{
public:
	CL_OutputSource *output;
	bool delete_output;
};

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_OutputSource_Zipped::CL_OutputSource_Zipped(
	CL_OutputSource *output,
	bool delete_output)
:
	impl(new CL_OutputSource_Zipped_Generic)
{
	impl->output = output;
	impl->delete_output = delete_output;
}

CL_OutputSource_Zipped::CL_OutputSource_Zipped(
	const CL_OutputSource_Zipped &copy)
:
	impl(new CL_OutputSource_Zipped_Generic)
{
	impl->output = copy.impl->output->clone();
	impl->delete_output = true;
}

CL_OutputSource_Zipped::~CL_OutputSource_Zipped()
{
	if (impl->delete_output) delete impl->output;
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CL_OutputSource_Zipped::tell() const
{
	return impl->output->tell();
}

int CL_OutputSource_Zipped::size() const
{
	return impl->output->size();
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

int CL_OutputSource_Zipped::write(const void *data, int size)
{
	return impl->output->write(data, size);
}

void CL_OutputSource_Zipped::open()
{
	impl->output->open();
}

void CL_OutputSource_Zipped::close()
{
	impl->output->close();
}

CL_OutputSource *CL_OutputSource_Zipped::clone()
{
	return new CL_OutputSource_Zipped(*this);
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:
