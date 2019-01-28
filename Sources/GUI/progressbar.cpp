/*
	$Id: progressbar.cpp,v 1.7 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/GUI/progressbar.h"
#include "API/GUI/component_options.h"
#include "API/GUI/stylemanager.h"
#include "progressbar_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_ProgressBar::CL_ProgressBar(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	impl = new CL_ProgressBar_Generic(this, 0);
	get_style_manager()->connect_styles("progressbar", this);
	find_preferred_size();
}

CL_ProgressBar::CL_ProgressBar(
	int steps,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	impl = new CL_ProgressBar_Generic(this, steps);
	get_style_manager()->connect_styles("progressbar", this);
	find_preferred_size();
}

CL_ProgressBar::CL_ProgressBar(
	const CL_Rect &pos,
	int steps,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(NULL)
{
	impl = new CL_ProgressBar_Generic(this, steps);
	get_style_manager()->connect_styles("progressbar", this);
}

CL_ProgressBar::~CL_ProgressBar()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CL_ProgressBar::get_progress() const
{
	return impl->progress;
}

int CL_ProgressBar::get_steps() const
{
	return impl->steps;
}

float CL_ProgressBar::get_percentage() const
{
	return impl->get_percentage();
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_ProgressBar::set_steps(int total_steps)
{
	impl->set_steps(total_steps);
}

void CL_ProgressBar::set_progress(int progress)
{
	impl->set_progress(progress);
}

void CL_ProgressBar::increase(int steps)
{
	impl->increase(steps);
}

void CL_ProgressBar::reset()
{
	impl->reset();
}
