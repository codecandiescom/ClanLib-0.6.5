#include "Core/precomp.h"

void cl_throw_error(const char *message)
{
	throw CL_Error(message);
}

void cl_throw_error(const std::string &message)
{
	throw CL_Error(message);
}
