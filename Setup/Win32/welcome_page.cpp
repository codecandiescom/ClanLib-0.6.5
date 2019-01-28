/*
	$Id: welcome_page.cpp,v 1.3 2001/09/08 19:24:17 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
// welcome_page.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "welcome_page.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CWelcomePage, CPropertyPage)


/////////////////////////////////////////////////////////////////////////////
// CWelcomePage property page

CWelcomePage::CWelcomePage() : CPropertyPage(CWelcomePage::IDD)
{
	//{{AFX_DATA_INIT(CWelcomePage)
	m_welcomeText = _T("");
	//}}AFX_DATA_INIT
}

CWelcomePage::~CWelcomePage()
{
}

void CWelcomePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWelcomePage)
	DDX_Text(pDX, IDC_WELCOME_TEXT, m_welcomeText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWelcomePage, CPropertyPage)
	//{{AFX_MSG_MAP(CWelcomePage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CWelcomePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_welcomeText  = "Welcome!\n\n";
	
	m_welcomeText += "This is the ClanLib configuration wizard. It will help you set up ";
	m_welcomeText += "a sane ClanLib build environment and customize the build process ";
	m_welcomeText += "for your needs.\n\n";

	m_welcomeText += "Enjoy.\n";
	m_welcomeText += "The ClanLib Development Team.";

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CWelcomePage::OnSetActive() 
{
	CPropertySheet* psheet = (CPropertySheet*) GetParent();
	psheet->SetWizardButtons(PSWIZB_NEXT);
	
	return CPropertyPage::OnSetActive();
}
