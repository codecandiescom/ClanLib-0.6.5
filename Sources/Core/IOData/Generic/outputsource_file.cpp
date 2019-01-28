/*
	$Id: outputsource_file.cpp,v 1.3 2001/12/11 20:44:21 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Core/IOData/outputsource_file.h"
#include "API/Core/System/cl_assert.h"
//#include "API/Core/System/error.h"

CL_OutputSource_File::CL_OutputSource_File(const std::string &_filename)
{
	filename = _filename;
	pos = 0;
	file = NULL;
	open();
}

CL_OutputSource_File::CL_OutputSource_File()
{
	pos = 0;
	file = NULL;
}

CL_OutputSource_File::~CL_OutputSource_File()
{
	close();
}

std::string CL_OutputSource_File::get_data() const
{
	cl_assert(false);
	return NULL;
}

int CL_OutputSource_File::write(const void *data, int size)
{
	cl_assert(file != NULL);
	fwrite(data, size, 1, file);
	pos += size;
	return size;
}

void CL_OutputSource_File::open()
{
	if(file != NULL)
		return;

	file = fopen(filename.c_str(), "w+b");
	if(file == NULL)
		throw CL_Error("could not create file");
}

void CL_OutputSource_File::close()
{
	if(file == NULL)
		return;

	fclose(file);
	file = NULL;
}

CL_OutputSource *CL_OutputSource_File::clone()
{
	cl_assert(false); // not implemented yet.
	return NULL;
}

int CL_OutputSource_File::tell() const
{
	return pos;
}

int CL_OutputSource_File::size() const
{
	return pos;
}
