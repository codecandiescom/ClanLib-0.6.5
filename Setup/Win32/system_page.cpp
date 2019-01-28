/*
	$Id: system_page.cpp,v 1.2 2001/09/08 19:24:17 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
// system_page.cpp : implementation file
//

#include "stdafx.h"
#include "configure.h"
#include "system_page.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CConfigureApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSystemPage property page

IMPLEMENT_DYNCREATE(CSystemPage, CPropertyPage)

CSystemPage::CSystemPage() : CPropertyPage(CSystemPage::IDD)
{
	//{{AFX_DATA_INIT(CSystemPage)
	m_input_include = _T("");
	m_input_lib = _T("");
	m_output_include = _T("");
	m_output_lib = _T("");
	m_setup_search_path = FALSE;
	//}}AFX_DATA_INIT
}

CSystemPage::~CSystemPage()
{
}

void CSystemPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemPage)
	DDX_Text(pDX, IDC_INPUT_INCLUDE, m_input_include);
	DDX_Text(pDX, IDC_INPUT_LIB, m_input_lib);
	DDX_Text(pDX, IDC_OUTPUT_INCLUDE, m_output_include);
	DDX_Text(pDX, IDC_OUTPUT_LIB, m_output_lib);
	DDX_Check(pDX, IDC_SETUP_SEARCH_PATH, m_setup_search_path);
	//}}AFX_DATA_MAP

	if(pDX->m_bSaveAndValidate) {
		theApp.WriteProfileString("InstallLocation", "InputInclude", m_input_include);
		theApp.WriteProfileString("InstallLocation", "InputLib", m_input_lib);
		theApp.WriteProfileString("InstallLocation", "OutputInclude", m_output_include);
		theApp.WriteProfileString("InstallLocation", "OutputLib", m_output_lib);
	}
}


BEGIN_MESSAGE_MAP(CSystemPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSystemPage)
	ON_BN_CLICKED(IDC_INPUT_INCLUDE_BROWSE, OnInputIncludeBrowse)
	ON_BN_CLICKED(IDC_INPUT_LIB_BROWSE, OnInputLibBrowse)
	ON_BN_CLICKED(IDC_OUTPUT_INCLUDE_BROWSE, OnOutputIncludeBrowse)
	ON_BN_CLICKED(IDC_OUTPUT_LIB_BROWSE, OnOutputLibBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemPage message handlers

typedef HRESULT (WINAPI *FolderPathFunc)(
				HWND,
				LPTSTR,
				int,
				BOOL);

BOOL CSystemPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_input_include = theApp.GetProfileString("InstallLocation", "InputInclude", "");
	m_input_lib = theApp.GetProfileString("InstallLocation", "InputLib", "");
	m_output_include = theApp.GetProfileString("InstallLocation", "OutputInclude", "");
	m_output_lib = theApp.GetProfileString("InstallLocation", "OutputLib", "");

	/*
		Bloody crap Windows 95. It doesn't have SHGetSpecialFolderPath unless
		the IE 4.0 shell extensions have been installed.

		So we try and dlopen it, and if we can get the function, we use it.
	*/

	CString cpath = "c:\\projects"; // default to c:/projects, if we can't get my documents.

	HINSTANCE dll = LoadLibrary("shell32.dll");
	if (dll != NULL)
	{
		FARPROC func = GetProcAddress(dll, "SHGetSpecialFolderPathA");
		if (func != NULL)
		{
			FolderPathFunc GetSpecialFolderPath = (FolderPathFunc) func;

			char path[MAX_PATH];
			/*SHGetSpecialFolderPath*/
			GetSpecialFolderPath(
				NULL,
				path,
				CSIDL_PERSONAL,
				FALSE);

			cpath = path;
		}

		FreeLibrary(dll);
	}

	if(m_input_include == "")
		m_input_include = cpath + "\\include";

	if(m_input_lib == "")
		m_input_lib = cpath + "\\lib";

	if(m_output_include == "")
		m_output_include = cpath + "\\include";

	if(m_output_lib == "")
		m_output_lib = cpath + "\\lib";

	m_setup_search_path = TRUE;

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSystemPage::OnInputIncludeBrowse() 
{
	char str[1024];

	UpdateData(TRUE);
	if (BrowseForFolder(m_hWnd, "Select input Include directory", str))
		m_input_include = str;
	UpdateData(FALSE);
}

void CSystemPage::OnInputLibBrowse() 
{
	char str[1024];

	UpdateData(TRUE);
	if (BrowseForFolder(m_hWnd, "Select input Library directory", str))
		m_input_lib = str;
	UpdateData(FALSE);
}

void CSystemPage::OnOutputIncludeBrowse() 
{
	char str[1024];

	UpdateData(TRUE);
	if (BrowseForFolder(m_hWnd, "Select output Include directory", str))
		m_output_include = str;
	UpdateData(FALSE);
}

void CSystemPage::OnOutputLibBrowse() 
{
	char str[1024];

	UpdateData(TRUE);
	if (BrowseForFolder(m_hWnd, "Select output Library directory", str))
		m_output_lib = str;
	UpdateData(FALSE);
}

BOOL CSystemPage::OnSetActive() 
{
	CPropertySheet* psheet = (CPropertySheet*) GetParent();
	psheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	
	return CPropertyPage::OnSetActive();
}
