/*
	$Id: netvariables_generic.h,v 1.2 2001/05/18 11:58:37 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_netvariables_generic
#define header_netvariables_generic

#include <list>
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/outputsource.h"

class CL_NetVariable
{
public:
	virtual ~CL_NetVariable() { return; }
	virtual CL_NetVariable *clone()=0;
	virtual void save(CL_OutputSource *output)=0;
	virtual void load(CL_InputSource *input)=0;
	virtual bool is_different()=0;
};

class CL_NetVariables_Generic
{
public:
	std::list<CL_NetVariable *> vars;
};

template<class T>
class CL_NetVariableT : public CL_NetVariable
{
public:
	CL_NetVariableT(T *var, int array, bool do_endian_swap = true)
	: var(var), array(array), do_endian_swap(do_endian_swap)
	{
		old = new T[array];
		memcpy(old, var, sizeof(T)*array);
	}

	virtual ~CL_NetVariableT()
	{
		delete[] old;
	}

	virtual CL_NetVariable *clone()
	{
		return new CL_NetVariableT<T>(var, array, do_endian_swap);
	}

	virtual void save(CL_OutputSource *output)
	{
		// todo: if (do_endian_swap)
		output->write(var, sizeof(T)*array);
		memcpy(old, var, sizeof(T)*array);
	}

	virtual void load(CL_InputSource *input)
	{
		input->read(var, sizeof(T)*array);
		// todo: if (do_endian_swap)
	}

	virtual bool is_different()
	{
		for (int i=0; i<array; i++) if (var[i] != old[i]) return true;
		return false;
	}

private:
	T *var, *old;
	int array;
	bool do_endian_swap;
};

#endif
