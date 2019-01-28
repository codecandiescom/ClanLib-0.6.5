/*
	$Id: progressbar_generic.h,v 1.3 2001/12/28 00:00:55 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_progressbar_generic
#define header_progressbar_generic

#include <API/Core/System/timer.h>
#include <API/Display/Input/key.h>
#include <API/Core/Math/rect.h>
#include <API/signals.h>

class CL_ProgressBar;
class CL_Button;
class CL_InputDevice;
class CL_Component;
class CL_ComponentOptions;
class CL_StyleManager;

class CL_ProgressBar_Generic
{
// Construction:
public:
	CL_ProgressBar_Generic(CL_ProgressBar *self, int steps);
	~CL_ProgressBar_Generic() {};

// Attributes:
public:
	float get_percentage() const;

	int steps;
	int progress;

// Operations:
public:
	void set_steps(int total_steps);
	void set_progress(int progress);
	void increase(int steps);
	void reset();

// Implementation:
private:
	CL_ProgressBar *progressbar;

	void on_set_options(const CL_ComponentOptions &options);
	CL_Slot slot_set_options;
};

#endif
