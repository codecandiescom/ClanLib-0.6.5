/*
	$Id: target_page.cpp,v 1.4 2001/09/08 19:24:17 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
// target_page.cpp : implementation file
//

#include "stdafx.h"
#include "configure.h"
#include "target_page.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTargetPage property page

IMPLEMENT_DYNCREATE(CTargetPage, CPropertyPage)

CTargetPage::CTargetPage() : CPropertyPage(CTargetPage::IDD)
{
	//{{AFX_DATA_INIT(CTargetPage)
	m_disptarget = -1;
	m_use_network = FALSE;
	m_use_opengl = FALSE;
	m_use_sound = FALSE;
	//}}AFX_DATA_INIT
}

CTargetPage::~CTargetPage()
{
}

void CTargetPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTargetPage)
	DDX_Radio(pDX, IDC_DISPTARGET, m_disptarget);
	DDX_Check(pDX, IDC_USE_NETWORK, m_use_network);
	DDX_Check(pDX, IDC_USE_OPENGL, m_use_opengl);
	DDX_Check(pDX, IDC_USE_SOUND, m_use_sound);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTargetPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTargetPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTargetPage message handlers

BOOL CTargetPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_disptarget = 0;
	m_use_network = TRUE;
	m_use_opengl = TRUE;
	m_use_sound = TRUE;
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
