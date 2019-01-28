/*
	$Id: progressbar_default.h,v 1.5 2001/12/28 00:00:55 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_progressbar_default
#define header_progressbar_default

#include "API/GUI/progressbar.h"
#include "API/GUI/stylemanager_default.h"
#include "API/GUI/component_style.h"

class CL_ResourceManager;

class CL_ProgressBar_Default : public CL_ComponentStyle
{
public:
	CL_ProgressBar_Default(
		CL_ProgressBar *progressbar,
		CL_StyleManager_Default *style);
	
	virtual ~CL_ProgressBar_Default() {};

private:
	void on_paint();

	CL_Slot slot_paint;

	CL_StyleManager_Default *style;
	CL_ProgressBar *progressbar;
};

#endif

