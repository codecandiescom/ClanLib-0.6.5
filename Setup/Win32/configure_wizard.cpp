/*
	$Id: configure_wizard.cpp,v 1.4 2001/09/08 19:24:17 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
// configure_wizard.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "configure_wizard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigureWizard

IMPLEMENT_DYNAMIC(CConfigureWizard, CPropertySheet)

CConfigureWizard::CConfigureWizard(CWnd* pWndParent)
	 : CPropertySheet(IDS_PROPSHT_CAPTION, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
	AddPage(&m_Page4);
	AddPage(&m_Page5);

	SetWizardMode();
}

CConfigureWizard::~CConfigureWizard()
{
}


BEGIN_MESSAGE_MAP(CConfigureWizard, CPropertySheet)
	//{{AFX_MSG_MAP(CConfigureWizard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CConfigureWizard message handlers

