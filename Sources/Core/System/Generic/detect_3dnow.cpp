/*
	$Id: detect_3dnow.cpp,v 1.7 2001/09/08 19:12:44 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Core/System/system.h"
#include "API/Core/System/cl_assert.h"

static bool do_3dnow_test();
static bool do_ext_3dnow_test();

bool CL_System::detect_3dnow()
{
	static bool has_3dnow = false;
	static bool first_time = true;
	
	if (first_time)
	{
		first_time = false;
		has_3dnow = do_3dnow_test();
	}
	
	return has_3dnow;
}

bool CL_System::detect_ext_3dnow()
{
	static bool has_ext_3dnow = false;
	static bool first_time = true;
	
	if (first_time)
	{
		first_time = false;
		has_ext_3dnow = do_ext_3dnow_test();
	}
	
	return has_ext_3dnow;
}

static bool do_3dnow_test()
{
#ifdef USE_I386_ASSEMBLER
#ifdef __MSC__
	unsigned long RegEDX = 0;
	unsigned long RegEAX = 0;

	try 
	{
		_asm
		{
			mov		eax, 0x80000000		// set up CPUID to return processor extended functions
			CPUID						// code bytes = 0fh,  0a2h
			mov		RegEAX, eax
		}

		if (!(RegEAX & 0x80000000))		// no functions > 0x80000000 ?
			return false;				//we don't have any extended features, quit
		else
			_asm
			{
				mov     eax, 0x80000001		// CPUID ext. function 0x80000001
				CPUID                       // EDX contains extended feature flags
				mov		RegEDX, edx			// extended features returned in edx
			}

   	}
	catch(...)					// catch everything
	{
		return false;
	}

	if (RegEDX & 0x80000000) 	// bit 31 indicates presence of 3DNow! technology
	{
		try
		{
			//DB 0Fh, 0Eh			// try executing the 3DNow! instruction "femms"
		     	_asm _emit 0x0f;
		     	_asm _emit 0x0e;
		}
		catch(...) { return false; }
	}
   	else
		return false;

	return true;
#elif __BORLANDC__

//This is for detecting assembly when using the Borland compilers
//If you do not have TASM then this will not compile
//If you do have TASM then uncomment the code below, otherwise ignore it

	#ifdef USE_TASM
	int RegEDX=0,RegEAX=0;
  try
  {
  	asm
    {
    	mov		eax, 0x80000000		// set up CPUID to return processor extended functions
    	CPUID						// code bytes = 0fh,  0a2h
    	mov		RegEAX, eax
    }
  }
  catch(...) { return false; }

  if(RegEAX > 0x80000000)
  {
  	asm
    {
			mov     eax, 0x80000001		// CPUID ext. function 0x80000001
    	CPUID                       // EDX contains extended feature flags
    	mov		RegEDX, edx			// extended features returned in edx
    }
  }
  else
  	return false;

  if( RegEDX & 0x80000000)
  {
  	try
		{
			//DB 0Fh, 0Eh			// try executing the 3DNow! instruction "femms"
      __emit__(0x0f,0x0e);
		}
		catch(...) { return false; }
  }
  else
  	return false;
  return true;
  #else

	std::cout<<"3DNow not supported by this version of library.  Please check source(detect_3dnow.cpp)."<<std::endl;
  return false;
	#endif
#elif __GNUC__

	unsigned int RegEDX=0,RegEAX=0;
	try
	{
		asm(
		"mov 0x80000000, %%eax \n" //Get ready for extended functions
		"CPUID \n" //Access CPUID
		: "=a"(RegEAX));
	}
	catch(...) 
	{
		return false;
	}
	if(RegEDX > 0x80000000)
	{
		asm(
				"mov 0x80000001, %%eax \n" //Access extended features
				"CPUID \n"  //Query CPU
				: "=d"(RegEDX));
	}
	else 
	{
		return false;
	}

	if(!(RegEDX & 0x80000000))
	{
		return false;
	}
	try
	{
		asm("femms");
	}
	catch(...)
	{
		return false;
	}
	return true;

#endif
#else
	static bool warning = true;
	if (warning)
	{
		cl_info(0, "CL_System::detect_3dnow() not implemented under unix yet.");
		warning = false;
	}
	return false;
#endif //Endif USE_I386_ASSEMBLER
}

static bool do_ext_3dnow_test()
{
#ifdef USE_I386_ASSEMBLER
#ifdef __MSC__
	unsigned long RegEDX = 0;
	unsigned long RegEAX = 0;

	try 
	{
		_asm
		{
			mov		eax, 0x80000000		// set up CPUID to return processor extended functions
			CPUID						// code bytes = 0fh,  0a2h
			mov		RegEAX, eax
		}

		if (!(RegEAX & 0x80000000))		// no functions > 0x80000000 ?
			return false;				//we don't have any extended features, quit
		else
			_asm
			{
				mov     eax, 0x80000001		// CPUID ext. function 0x80000001
				CPUID                       // EDX contains extended feature flags
				mov		RegEDX, edx			// extended features returned in edx
			}

   	}
	catch(...)					// catch everything
	{
		return false;
	}

	if (RegEDX & 0x40000000) 	// bit 30 indicates presence of extended 3DNow! technology
	{
		try
		{
			//DB 0Fh, 0Eh			// try executing the 3DNow! instruction "femms"
		     	_asm _emit 0x0f;
		     	_asm _emit 0x0e;
		}
		catch(...) { return false; }
	}
   	else
		return false;

	return true;
#elif __BORLANDC__

//This is for detecting assembly when using the Borland compilers
//If you do not have TASM then this will not compile
//If you do have TASM then uncomment the code below, otherwise ignore it

	#ifdef USE_TASM
	int RegEDX=0,RegEAX=0;
  try
  {
  	asm
    {
    	mov		eax, 0x80000000		// set up CPUID to return processor extended functions
    	CPUID						// code bytes = 0fh,  0a2h
    	mov		RegEAX, eax
    }
  }
  catch(...) { return false; }

  if(RegEAX > 0x80000000)
  {
  	asm
    {
			mov     eax, 0x80000001		// CPUID ext. function 0x80000001
    	CPUID                       // EDX contains extended feature flags
    	mov		RegEDX, edx			// extended features returned in edx
    }
  }
  else
  	return false;

  if( RegEDX & 0x40000000)
  {
  	try
		{
			//DB 0Fh, 0Eh			// try executing the 3DNow! instruction "femms"
      __emit__(0x0f,0x0e);
		}
		catch(...) { return false; }
  }
  else
  	return false;
  return true;
  #else

	std::cout<<"3DNow detection not supported by this version of library.  Please check source(detect_3dnow.cpp)."<<std::endl;
  return false;
	#endif
#elif __GNUC__

	unsigned int RegEDX=0,RegEAX=0;
	try
	{
		asm(
		"mov 0x80000000, %%eax \n" //Get ready for extended functions
		"CPUID \n" //Access CPUID
		: "=a"(RegEAX));
	}
	catch(...) 
	{
		return false;
	}
	if(RegEDX > 0x80000000)
	{
		asm(
				"mov 0x80000001, %%eax \n" //Access extended features
				"CPUID \n"  //Query CPU
				: "=d"(RegEDX));
	}
	else 
	{
		return false;
	}

	if(!(RegEDX & 0x40000000))
	{
		return false;
	}
	try
	{
		asm("femms");
	}
	catch(...)
	{
		return false;
	}
	return true;

#endif
#else
	static bool warning = true;
	if (warning)
	{
		cl_info(0, "CL_System::detect_ext_3dnow() not implemented under unix yet.");
		warning = false;
	}
	return false;
#endif //Endif USE_I386_ASSEMBLER

}
