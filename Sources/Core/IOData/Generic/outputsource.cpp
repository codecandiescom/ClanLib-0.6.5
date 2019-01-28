/*
	$Id: outputsource.cpp,v 1.3 2001/09/08 19:12:44 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Core/IOData/outputsource.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OutputSource operations:

void CL_OutputSource::set_system_mode()
{
	little_endian_mode = true;
}

void CL_OutputSource::set_big_endian_mode()
{
	little_endian_mode = false;
}

void CL_OutputSource::set_little_endian_mode()
{
	little_endian_mode = true;
}

void CL_OutputSource::write_int32(int data)
{
	write(&data, sizeof(int));
}

void CL_OutputSource::write_uint32(unsigned int data)
{
	write(&data, sizeof(unsigned int));
}

void CL_OutputSource::write_short16(short data)
{
	write(&data, sizeof(short));
}

void CL_OutputSource::write_ushort16(unsigned short data)
{
	write(&data, sizeof(unsigned short));
}

void CL_OutputSource::write_char8(char data)
{
	write(&data, sizeof(char));
}

void CL_OutputSource::write_uchar8(unsigned char data)
{
	write(&data, sizeof(unsigned char));
}

void CL_OutputSource::write_float32(float data)
{
	write(&data, sizeof(float));
}

void CL_OutputSource::write_bool8(bool data)
{
	write(&data, sizeof(bool));
}

void CL_OutputSource::write_string(const std::string &str)
{
	int size = str.length();
	write(&size, sizeof(int));
	write(str.data(), size);
}
