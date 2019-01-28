/*
	$Id: inputsource_file.cpp,v 1.5 2002/06/20 10:22:43 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <API/Core/IOData/inputsource_file.h>

CL_InputSource_File::CL_InputSource_File(const std::string &_filename)
{
	filename = _filename;
	filehandle = NULL;
	open();
}

CL_InputSource_File::CL_InputSource_File(const CL_InputSource_File *source)
{
	filename = source->filename;
	filehandle = NULL;

	open();
	fseek(filehandle, ftell(source->filehandle), SEEK_SET);
}

CL_InputSource_File::~CL_InputSource_File()
{
	close();
}

int CL_InputSource_File::read(void *data, int size)
{
	return fread(data, 1, size, filehandle);
}

void CL_InputSource_File::open()
{
	if (filehandle != NULL) return;

#ifndef WIN32 // hate win32 non posix conform
	if (filename[0] == '!')
	{
		filehandle = popen(std::string(filename, 1).c_str(), "rb");
		if (filehandle == NULL)
		{
			std::string err;
			err += "Could not open pipe: " + std::string(filename,1 );
			throw CL_Error(err);
		}
		filesize = 99999999;
	}
	else
#endif
	{
		filehandle = fopen(filename.c_str(), "rb");
		if (filehandle == NULL)
		{
			std::string err;
			err += "Could not open file: " + filename;
			throw CL_Error(err);
		}
		fseek(filehandle, 0, SEEK_END);
		filesize = ftell(filehandle);
		fseek(filehandle, 0, SEEK_SET);
	}
//	cl_assert(filehandle != NULL);
}

void CL_InputSource_File::close()
{
	if (filehandle == NULL) return;
	fclose(filehandle);

	filehandle = NULL;
}

CL_InputSource *CL_InputSource_File::clone() const
{
	return new CL_InputSource_File(this);
}

int CL_InputSource_File::tell() const
{
	return ftell(filehandle);
}

void CL_InputSource_File::seek(int pos, SeekEnum seek_type)
{
	switch (seek_type)
	{
		case seek_cur:
			fseek(filehandle, pos, SEEK_CUR);
			break;

		case seek_set:
			fseek(filehandle, pos, SEEK_SET);
			break;

		case seek_end:
			fseek(filehandle, pos, SEEK_END);
			break;
	}
}

int CL_InputSource_File::size() const
{
	return filesize;
}

void CL_InputSource_File::push_position()
{
	int a = ftell(filehandle);

	stack.push(a);
}

void CL_InputSource_File::pop_position()
{
	int a = stack.top();
	stack.pop();

	fseek(filehandle, a, SEEK_SET);
}
