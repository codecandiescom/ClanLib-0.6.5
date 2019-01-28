/*
	$Id: cl_assert.h,v 1.13 2001/09/22 15:52:11 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanCore="System"
//! header=core.h

#ifndef header_cl_assert
#define header_cl_assert

#include <list>

//: Error Channels Enum
enum ErrorChannels
{
	info_display = 1,
	info_sound   = 2,
	info_input   = 4,
	info_network = 8,
	info_gui     = 16
};

#ifndef __PRETTY_FUNCTION__ 
#define __PRETTY_FUNCTION__ NULL
#endif

#ifndef __STRING
#define cl_assert(a) CL_Assert::die(a, __FILE__, __LINE__, __PRETTY_FUNCTION__, #a);
#else
#define cl_assert(a) CL_Assert::die(a, __FILE__, __LINE__, __PRETTY_FUNCTION__, __STRING(a));
#endif

#define cl_info(errchannel, message) \
	CL_Assert::info(errchannel, message, __FILE__, __LINE__);

#ifdef DEBUG
	#define cl_assert_debug(a) cl_assert(a)
	#define cl_info_debug(a,b) cl_info(a,b)
#else
	#define cl_assert_debug(a)
	#define cl_info_debug(a,b)
#endif


class CL_AssertListener;
//: This is ClanLib's assertion class.
//- <p>It is needed because some
//- implementations requires to be notified before ClanLib is exited.
//- For instance the svgalib implementation need release the keyboard lock.</p>
class CL_Assert
{
	static std::list<CL_AssertListener*> listeners;
public:
//! Operations:
	//: Enabled the error channels specified by the 'channel_mask'.
	//: <p>This can be used to get extra debugging information from limited
	//: parts of ClanLib.</p>
	//- channel_mask - Channels to be enabled.
	static void enable_channels(int channel_mask);

	//: Produces an assert.
	//: <p>This function shouldn't be called directly -
	//: instead, use the cl_assert macro.
	//:  For example: cl_assert(should_not_be_zero != 0)</p>
	//- a - Asserts if false.
	//- file - Name of the file where the assert occurred.
	//- line - Line in the file where the assert occurred.
	//- func - Function in which the assert occurred.
	//- assert_str - String that describe the assertion check.
	static void die(bool a, const char *file, int line, const char *func, const char *assert_str);

	//: Writes an info statements on the specified channel.
	//: <p>Don't callthis one directly, instead use the cl_info macro.
	//:  For example: cl_info(info_network, "MyClass::my_funx - Hello World.");</p>
	//- channel - Channel to use. (Use the ErrorChannels enum's)
	//- text - text to print.
	//- file - Name of the file where this was printed from.
	//- line - Line in the file where this was printed from.
	static void info(int channel, const char *text, const char *file, int line);
	
	//: Adds an assertion listener which will be called in case of an assertion.
	//- listener - The listener to be added.
	static void add_listener(CL_AssertListener *listener);

	//: Removes an assertion listener.
	//- listener - The listener to be removed.
	static void remove_listener(CL_AssertListener *listener);
};

//: Interface used by the CL_Assert class.
class CL_AssertListener
{
public:
//! Construction:
	//: Assert Listener Destructor
	virtual ~CL_AssertListener() {;}

//! Overrideables:
	//: Called by CL_Assert::die() when assertion occurs.
	//: <p>Remember to add it as an listener - otherwise
	//: it will _NOT_ be called.</p>
	//- file - Filename where the assert occurred.
	//- line - Line number in the file where the assert occurred.
	virtual void assert_occurred(const char *file, int line)=0;
};

#endif
