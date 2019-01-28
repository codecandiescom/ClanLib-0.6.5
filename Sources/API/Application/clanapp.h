/*
	$Id: clanapp.h,v 1.10 2001/10/10 17:54:21 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanApp="System"
//! header=application.h

#ifndef header_clanapp
#define header_clanapp

//: The starting place for all applications.
//- The application class must be inherited by all ClanLib applications.
//- <p>In your application create a class inheriting CL_ClanApplication, 
//- make a global instance of it, and fill in the main() and get_title() functions.</p>
class CL_ClanApplication
{
public:
//! Construction:
	//: Clan Application constructor
	CL_ClanApplication();

	//: Clan Application destructor
	virtual ~CL_ClanApplication();

//! Overrideables:

	//: Every program made using ClanLib must implement this function. 
	//: This is the main application function, which will be called _once_ following system initialization.
	//- argc - The number of arguments to the program (c-style)
	//- argv - A pointer to strings, containing the program arguments. The name of the program executable is always passed as first argument.
	//- returns - Program return value
	virtual int main(int argc, char** argv)=0;

//! Attributes:
	//: Get application (window) title
	//- returns - A character string to be put as the application (window) title (when appropriate, ie. not fullscreen, etc)
	virtual char* get_title()=0;

//! Variables:
	//: Static pointer to an initialized instance of this class.
	//: This is set by the constructor.
	static CL_ClanApplication *app;
};

#endif
