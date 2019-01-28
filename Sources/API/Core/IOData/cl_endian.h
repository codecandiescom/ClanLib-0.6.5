/*
	$Id: cl_endian.h,v 1.9 2001/09/22 15:52:10 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanCore="I/O Data"
//! header=core.h

#ifndef header_endian
#define header_endian

//: Endianess management class.
//- <p>The CL_Endian class is used to convert between different endianess. There
//- are also a set of defines to the lazy developer:</p>
//- #define SWAP_IF_BIG(i)
//- #define SWAP_IF_LITTLE(i)
//- #define SWAP_IF_BIG_ALOT(i, times)
//- #define SWAP_IF_LITTLE_ALOT(i, times)
//- #define IS_SYSTEM_64BIT()
//- Each of these defines call CL_Endian::is_system_big() and CL_Endian::swap.
class CL_Endian
{
public:
//! Operations:
	//: Swaps larger amounts of data between little and big endian.
	//- data -Data to be swapped.
	//- type_size - Size of datatype to be swapped.
	//- total_times - Number of 'type_size' size data chunks to be swapped.
	static void swap(void *data, int type_size, int total_times=1);

//! Attributes:
	//: Returns true if big endian system.
	//- Returns - True if big endian system, false otherwise.
	static bool is_system_big();

	//: Returns true if 64 bit system.
	//- Returns - True if 64 bit, false otherwise.
	static bool is_system_64bit();
};

// Defines to the lazy developer:
#define SWAP_IF_BIG(i) if (CL_Endian::is_system_big()) CL_Endian::swap(&i, sizeof(i));
#define SWAP_IF_LITTLE(i) if (!CL_Endian::is_system_big()) CL_Endian::swap(&i, sizeof(i));

#define SWAP_IF_BIG_ALOT(i, times) if (CL_Endian::is_system_big()) CL_Endian::swap(&i, sizeof(i), times);
#define SWAP_IF_LITTLE_ALOT(i, times) if (!CL_Endian::is_system_big()) CL_Endian::swap(&i, sizeof(i), times);

#define IS_SYSTEM_64BIT() CL_Endian::is_system_64bit();

#endif
