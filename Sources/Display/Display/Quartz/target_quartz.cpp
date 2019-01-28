#include "target_quartz.h"
#include <iostream>

CL_Target_Quartz::CL_Target_Quartz(CGDirectDisplayID context)
{
	ref_count = 0;
	locked = false;

	width = CGDisplayPixelsWide(context);
	height = CGDisplayPixelsHigh(context);
	bpp = CGDisplayBitsPerPixel(context);
	bpp = bpp / 8;
	pitch = bpp * width;

	display = context;
}

CL_Target_Quartz::~CL_Target_Quartz()
{
	if(ref_count)
		std::cout << "Warning: Not all reference released for CL_Target_Quartz." << std::endl;

}

void CL_Target_Quartz::lock()
{
	locked = true;
	ref_count++;
}

void* CL_Target_Quartz::get_data()
{
	cl_assert(locked);

	return (void*)CGDisplayBaseAddress(display);
}

void CL_Target_Quartz::unlock()
{
	if(--ref_count);
	else
		locked = false;
}


