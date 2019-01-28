/*
	$Id: configure_wizard.h,v 1.4 2001/09/08 19:24:17 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
// configure_wizard.h : header file
//
// This class defines custom modal property sheet 
// CConfigureWizard.
 
#ifndef __CONFIGURE5F_WIZARD_H__
#define __CONFIGURE5F_WIZARD_H__

#include "welcome_page.h"
#include "target_page.h"
#include "system_page.h"
#include "finished_page.h"
#include "compiler_page.h"

/////////////////////////////////////////////////////////////////////////////
// CConfigureWizard

class CConfigureWizard : public CPropertySheet
{
	DECLARE_DYNAMIC(CConfigureWizard)

// Construction
public:
	CConfigureWizard(CWnd* pWndParent = NULL);

// Attributes
public:
	CWelcomePage m_Page1;
	CCompilerPage m_Page2;
	CTargetPage m_Page3;
	CSystemPage m_Page4;
	CFinishedPage m_Page5;
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigureWizard)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CConfigureWizard();

// Generated message map functions
protected:
	//{{AFX_MSG(CConfigureWizard)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __CONFIGURE5F_WIZARD_H__
