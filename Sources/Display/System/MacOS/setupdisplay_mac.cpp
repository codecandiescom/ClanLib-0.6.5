#include "display_target.h"
#include "API/Core/System/error.h"
#include "API/display.h"
#include "Display/System/Generic/setupdisplay_generic.h"
#include "Display/Display/Quartz/target_quartz.h"
#include "Display/Display/Quartz/display_quartz.h"
#include <Hermes/Hermes.h>

static int display_ref_count = 0;
static CL_Target_Quartz *target;
static CL_DisplayCard_Quartz *display_card;

void CL_SetupDisplay::init(bool register_resources_only)
{
	display_ref_count++;
	if (display_ref_count > 1) return;

	CL_SetupDisplay_Generic::init();

	if (register_resources_only) return;

	Hermes_Init();

	target = new CL_Target_Quartz;
	
	display_card = new CL_DisplayCard_Quartz;	
}

void CL_SetupDisplay::deinit()
{
	display_ref_count--;
	if(display_ref_count > 0) return;

	Hermes_Done();

	if (display_card) delete display_card;

	if (target) delete target;

	CL_SetupDisplay_Generic::deinit();
}
