/*
	$Id: finished_page.cpp,v 1.3 2001/09/08 19:24:17 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
// finished_page.cpp : implementation file
//

#include "stdafx.h"
#include "configure.h"
#include "finished_page.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFinishedPage property page

IMPLEMENT_DYNCREATE(CFinishedPage, CPropertyPage)

CFinishedPage::CFinishedPage() : CPropertyPage(CFinishedPage::IDD)
{
	//{{AFX_DATA_INIT(CFinishedPage)
	m_text = _T("");
	//}}AFX_DATA_INIT
}

CFinishedPage::~CFinishedPage()
{
}

void CFinishedPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFinishedPage)
	DDX_Text(pDX, IDC_FINISHED_TEXT, m_text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFinishedPage, CPropertyPage)
	//{{AFX_MSG_MAP(CFinishedPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFinishedPage message handlers

BOOL CFinishedPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_text  = "Congratulations!\n\n";

	m_text += "You have finished the ClanLib configuration. Now press finish to finally ";
	m_text += "produce the ClanLib workspace and its project files. ";

	m_text += "When the installation process is completed, please open the ClanLib.dsw ";
	m_text += "workspace that this wizard has produced for you.\n\n";
	// TODO:
	// If user created a Borland project, write another description.

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CFinishedPage::OnSetActive() 
{
	CPropertySheet* psheet = (CPropertySheet*) GetParent();
	psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);

	return CPropertyPage::OnSetActive();
}
