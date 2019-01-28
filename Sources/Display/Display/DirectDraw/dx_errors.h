/*
	$Id: dx_errors.h,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_dx_error
#define header_dx_error

char *get_dderr(HRESULT err);
char *get_dinputerr(HRESULT err);
char *get_dsounderr(HRESULT err);
char *get_d3derr(HRESULT err);

#endif