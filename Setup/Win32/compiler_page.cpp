/*
	$Id: compiler_page.cpp,v 1.2 2001/09/08 19:24:17 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
// compiler_page.cpp : implementation file
//

#include "stdafx.h"
#include "configure.h"
#include "compiler_page.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompilerPage property page

IMPLEMENT_DYNCREATE(CCompilerPage, CPropertyPage)

CCompilerPage::CCompilerPage() : CPropertyPage(CCompilerPage::IDD)
{
	//{{AFX_DATA_INIT(CCompilerPage)
	m_compilertarget = -1;
	//}}AFX_DATA_INIT
}

CCompilerPage::~CCompilerPage()
{
}

void CCompilerPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompilerPage)
	DDX_Radio(pDX, IDC_COMPILERTARGET, m_compilertarget);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCompilerPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCompilerPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompilerPage message handlers

BOOL CCompilerPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_compilertarget = 0;
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
