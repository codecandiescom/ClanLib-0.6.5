/*
	$Id: system_page.h,v 1.2 2001/09/08 19:24:17 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#if !defined(AFX_SYSTEM_PAGE_H__75F5C8C1_A9F5_11D3_8F4C_002018383984__INCLUDED_)
#define AFX_SYSTEM_PAGE_H__75F5C8C1_A9F5_11D3_8F4C_002018383984__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// system_page.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSystemPage dialog

class CSystemPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSystemPage)

// Construction
public:
	CSystemPage();
	~CSystemPage();

// Dialog Data
	//{{AFX_DATA(CSystemPage)
	enum { IDD = IDD_SYSTEM_PAGE };
	CString	m_input_include;
	CString	m_input_lib;
	CString	m_output_include;
	CString	m_output_lib;
	BOOL	m_setup_search_path;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSystemPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSystemPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnInputIncludeBrowse();
	afx_msg void OnInputLibBrowse();
	afx_msg void OnOutputIncludeBrowse();
	afx_msg void OnOutputLibBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEM_PAGE_H__75F5C8C1_A9F5_11D3_8F4C_002018383984__INCLUDED_)
