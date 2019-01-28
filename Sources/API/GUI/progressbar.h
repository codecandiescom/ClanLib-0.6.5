/*
	$Id: progressbar.h,v 1.11 2002/02/04 15:21:34 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanGUI="Controls"
//! header=gui.h

#ifndef header_progressbar
#define header_progressbar

#include "component.h"

class CL_ComponentOptions;
class CL_Rect;
class CL_ProgressBar_Generic;

//: ProgressBar component
class CL_ProgressBar : public CL_Component
{
//! Construction:
public:
	//: ProgressBar Constructor
	CL_ProgressBar(
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: ProgressBar Constructor
	CL_ProgressBar(
		int steps,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: ProgressBar Constructor
	CL_ProgressBar(
		const CL_Rect &pos,
		int steps,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: ProgressBar Destructor
	virtual ~CL_ProgressBar();

//! Attributes:
public:
	//: Returns the total number of steps.
	int get_steps() const;

	//: Returns the current amount of progress.
	int get_progress() const;

	//: Returns the current amount of progress in percentage.
	float get_percentage() const;

//! Operations:
public:
	//: Sets the total number of steps to total_steps. 
	void set_steps(int total_steps);

	//: Sets the current amount of progress of the total number of steps. 
	void set_progress(int progress);

	//: Progresses the bar with steps.
	void increase(int steps = 1);

	//: Reset the progress bar. The progress bar `rewinds'. 
	void reset();

//! Signals:
public:

//! Implementation:
private:
	CL_ProgressBar(const CL_ProgressBar &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.
	CL_ProgressBar_Generic *impl;
};

#endif
