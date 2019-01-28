#ifndef header_write_png
#define header_write_png

#include "writepng.h"

class WritePNG
{
 public:
	static void write_png( CL_Surface *surface, std::string path );
	static void cleanup();
	
	static mainprog_info wpng_info;
};

#endif // header_write_png
