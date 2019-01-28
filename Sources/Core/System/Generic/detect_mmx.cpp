/***************************************************************************
 *
 * DetectMMX.cpp
 * by Darryl Agostinelli March 4, 2000
 *
 * Code was adopted from Intel web page
 *
 */

#include "Core/precomp.h"
#include "API/Core/System/system.h"
#include "API/Core/System/cl_assert.h"

static bool do_mmx_test();

bool CL_System::detect_mmx()
{
	static bool has_mmx = false;
	static bool first_time = true;
	
	if (first_time)
	{
		first_time = false;
		has_mmx = do_mmx_test();
	}
	
	return has_mmx;
}

static bool do_mmx_test()
{
#ifdef USE_I386_ASSEMBLER
#ifdef __MSC__
	unsigned long RegEDX;

	try 
	{
		_asm
		{
			mov eax, 1			// set up CPUID to return processor version and features
								//	0 = vendor string, 1 = version info, 2 = cache info
			CPUID				// code bytes = 0fh,  0a2h
			mov RegEDX, edx		// features returned in edx
	   	}
   	}
	catch(...)					// catch everything
	{
		return false;        	// processor does not support CPUID
	}

	if (RegEDX & 0x800000) 		// bit 23 is set for MMX technology
	{
		try { _asm {emms} }		// try executing the MMX instruction "emms"
		catch(...) { return false; }
	}
   	else
		return false;        	// processor supports CPUID but does not support MMX technology

	// if we get here, it means the processor has MMX technology but
	// floating-point emulation is on; so MMX technology is unavailable

	return true;
#elif  __GNUC__

	int edx=0;
	try 
	{
		asm(
				"mov 1,%%eax \n" //Get ready for CPUID
				"CPUID \n" //Access CPUID
				:"=d"(edx) 
			 );
	}
	catch(...) //Catch all
	{
		return false;
	}
	if (edx & 0x800000)
	{
		try{ asm("emms"); }
		catch(...) {return false;}
	}
	else
		return false;
	
	return true;
#elif __BORLANDC__

//This is for detecting assembly when using the Borland compilers
//If you do not have TASM then this will not compile
//If you do have TASM then uncomment the code below, otherwise ignore it

	#ifdef USE_TASM
	int RegEDX=0;
	try
  {
  	asm
    {
    	mov eax, 1
      CPUID
      mov RegEDX, edx
    }
  }
  catch(...) {return false;}

  if (RegEDX & 0x800000)
  {
  	try{asm{emms}}
		catch(...) {return false;}
  }
  else
  	return false;
  return true;
#else

	std::cout<<"Support to detect this has not been compiled in.  Please check source(detect_mmx.cpp)"<<std::endl;
  return false;
#endif //Endif USE_TASM

#else

	cl_info(0, "CL_System::detect_mmx() not implemented under your OS/Compiler/Architecture.");
	return false;

#endif

#else
	return false;
#endif //End USE_I386_ASSEMBLER
}
