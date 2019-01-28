/*
	$Id: progressbar_generic.cpp,v 1.7 2002/01/08 10:26:20 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "progressbar_generic.h"
#include "API/GUI/progressbar.h"
#include "API/GUI/component.h"
#include "API/GUI/component_options.h"
#include "API/GUI/stylemanager.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_ProgressBar_Generic::CL_ProgressBar_Generic(
	CL_ProgressBar *self,
	int steps)
:	progress(0), progressbar(self)
{
	set_steps(steps);

	slot_set_options = progressbar->sig_set_options().connect(
		this, &CL_ProgressBar_Generic::on_set_options);
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

float CL_ProgressBar_Generic::get_percentage() const
{
	return (float)progress / steps;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_ProgressBar_Generic::set_steps(int total_steps)
{
	steps = total_steps;

	if(progress > steps)
		progress = steps;
}

void CL_ProgressBar_Generic::set_progress(int new_progress)
{
	progress = new_progress;
	if(progress > steps)
		progress = steps;
	if(progress < 0)
		progress = 0;
}

void CL_ProgressBar_Generic::increase(int steps)
{
	set_progress(progress + steps);
}

void CL_ProgressBar_Generic::reset()
{
	progress = 0;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_ProgressBar_Generic::on_set_options(const CL_ComponentOptions &options)
{
	if (options.exists("steps"))
		set_steps(options.get_value_as_int("steps"));
}
