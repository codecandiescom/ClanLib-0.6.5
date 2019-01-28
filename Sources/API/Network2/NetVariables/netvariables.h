/*
	$Id: netvariables.h,v 1.1 2002/11/02 19:46:02 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanNetwork="NetObjects"
//! header=network.h

#ifndef header_netvariables
#define header_netvariables

class CL_NetVariables_Generic;
class CL_InputSource;
class CL_OutputSource;

//: Easy serialization of a data structure.
//- <p>The netvariables class is used to read a set of variables from an object,
//- and write them structured, in network byte order, to an output source.</p>
//-
//- <p>The Netvariables interface can also read the data again, and produce 'diff'
//- packages where it only write variables that have changed since last write.</p>
class CL_NetVariables
{
//! Construction:
public:
	//: Construct a netvariables object.
	CL_NetVariables();

	//: Net Variables Destructor
	virtual ~CL_NetVariables();

//! Attributes:

	//: Returns true if the variables have changed since last save.
	bool is_different();

//! Operations:

	//: Add booleans to the data structure.
	void add_bool(bool *var, int array = 1);

	//: Add integers to the data structure.
	void add_int(int *var, int array = 1);

	//: Add shorts to the data structure.
	void add_short(short *var, int array = 1);

	//: Add floats to the data structure.
	void add_float(float *var, int array = 1);

	//: Add doubles to the data structure.
	void add_double(double *var, int array = 1);

	//: Add structures to the data structure.
	void add_vars(CL_NetVariables *variables, int array = 1);

	//: Write all variables in the data structure to the output source.
	void save_all(class CL_OutputSource *msg);

	//: Read all variables in the data structure from the input source.
	void load_all(class CL_InputSource *msg);

	//: Write all variables that have changed since last save.
	void save_diff(class CL_OutputSource *msg);

	//: Read a diff produced with save_diff from the input source.
	void load_diff(class CL_InputSource *msg);

//! Implementation:
public:
	CL_NetVariables_Generic *impl;
};

#endif
