/*
	$Id: configure.h,v 1.6 2001/09/08 19:24:17 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
// Configure.h : main header file for the CONFIGURE application
//

#if !defined(AFX_CONFIGURE_H__38147984_A99A_11D3_8F4C_002018383984__INCLUDED_)
#define AFX_CONFIGURE_H__38147984_A99A_11D3_8F4C_002018383984__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <fstream.h>
#include <string>
#include <list>
#include "../Generic/workspace.h"

/////////////////////////////////////////////////////////////////////////////
// CConfigureApp:
// See Configure.cpp for the implementation of this class
//
class CConfigureApp : public CWinApp
{
public:
	CConfigureApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigureApp)
	public:
	virtual BOOL InitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CConfigureApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	Workspace create_workspace(class CConfigureWizard &wizard);
};

BOOL BrowseForFolder(HWND hOwner, char* szTitle, char* szRetval);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGURE_H__38147984_A99A_11D3_8F4C_002018383984__INCLUDED_)
