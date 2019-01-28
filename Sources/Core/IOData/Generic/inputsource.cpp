/*
	$Id: inputsource.cpp,v 1.4 2001/09/10 03:40:32 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/cl_endian.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource operations:

void CL_InputSource::set_system_mode()
{
	little_endian_mode = true; // fix me: should use whatever endianess the system uses.
}

void CL_InputSource::set_big_endian_mode()
{
	little_endian_mode = false;
}

void CL_InputSource::set_little_endian_mode()
{
	little_endian_mode = true;
}

int CL_InputSource::read_int32()
{
	int answer;
	if (read(&answer, sizeof(int)) != sizeof(int)) throw CL_Error("CL_InputSource_Datafile::read_int32() failed");

	SWAP_IF_BIG(answer);

	return answer;
}

unsigned int CL_InputSource::read_uint32()
{
	unsigned int answer;
	if (read(&answer, sizeof(unsigned int)) != sizeof(unsigned int)) throw CL_Error("CL_InputSource_Datafile::read_uint32() failed");

	SWAP_IF_BIG(answer);

	return answer;
}

short CL_InputSource::read_short16()
{
	short answer;
	if (read(&answer, sizeof(short)) != sizeof(short)) throw CL_Error("CL_InputSource_Datafile::read_short16() failed");

	SWAP_IF_BIG(answer);
	// todo: do endianess converting here, if needed.
	return answer;
}

unsigned short CL_InputSource::read_ushort16()
{
	unsigned short answer;
	if (read(&answer, sizeof(unsigned short)) != sizeof(unsigned short)) throw CL_Error("CL_InputSource_Datafile::read_ushort16() failed");

	SWAP_IF_BIG(answer);
	// todo: do endianess converting here, if needed.
	return answer;
}

char CL_InputSource::read_char8()
{
	char answer;
	if (read(&answer, sizeof(char)) != sizeof(char)) throw CL_Error("CL_InputSource_Datafile::read_char8() failed");
	// todo: do endianess converting here, if needed.
	return answer;
}

unsigned char CL_InputSource::read_uchar8()
{
	unsigned char answer;
	if (read(&answer, sizeof(unsigned char)) != sizeof(unsigned char)) throw CL_Error("CL_InputSource_Datafile::read_uchar8() failed");
	// todo: do endianess converting here, if needed.
	return answer;
}

float CL_InputSource::read_float32()
{
	float answer;
	if (read(&answer, sizeof(float)) != sizeof(float)) throw CL_Error("CL_InputSource_Datafile::read_float32() failed");


	SWAP_IF_BIG(answer);
	
	return answer;
}

bool CL_InputSource::read_bool8()
{
	bool answer;
	if (read(&answer, sizeof(bool)) != sizeof(bool)) throw CL_Error("CL_InputSource_Datafile::read_bool8() failed");
	// todo: do endianess converting here, if needed.
	return answer;
}

std::string CL_InputSource::read_string()
{
	int size = read_int32();

	char *str = new char[size];
	try
	{
		read(str, size);
		
		std::string ret(str, size);
		delete[] str;

		return ret;
	}
	catch (...)
	{
		delete[] str;
		throw;
	}
}
