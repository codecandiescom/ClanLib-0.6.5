/*
	$Id: compiler_page.h,v 1.2 2001/09/08 19:24:17 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#if !defined(AFX_COMPILERPAGE_H__FCDDAD60_9B8C_11D4_970D_005004CAB900__INCLUDED_)
#define AFX_COMPILERPAGE_H__FCDDAD60_9B8C_11D4_970D_005004CAB900__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// compiler_page.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCompilerPage dialog

class CCompilerPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCompilerPage)

// Construction
public:
	CCompilerPage();
	~CCompilerPage();

// Dialog Data
	//{{AFX_DATA(CCompilerPage)
	enum { IDD = IDD_COMPILER_PAGE };
	int		m_compilertarget;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCompilerPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCompilerPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPILERPAGE_H__FCDDAD60_9B8C_11D4_970D_005004CAB900__INCLUDED_)
