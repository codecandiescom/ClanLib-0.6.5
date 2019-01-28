/*
	$Id: configure.cpp,v 1.62 2002/11/02 19:57:05 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
// Configure.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Configure.h"
#include "configure_wizard.h"
#include "workspace_generator_msvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigureApp

BEGIN_MESSAGE_MAP(CConfigureApp, CWinApp)
	//{{AFX_MSG_MAP(CConfigureApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigureApp construction

CConfigureApp::CConfigureApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CConfigureApp object

CConfigureApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CConfigureApp initialization

BOOL CConfigureApp::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	SetRegistryKey("ClanSoft");

	CConfigureWizard wizard;
	m_pMainWnd = &wizard;

	int nResponse = wizard.DoModal();
	if (nResponse == ID_WIZFINISH)
	{
		Workspace workspace = create_workspace(wizard);

		WorkspaceGenerator_MSVC generator;
		generator.write(workspace);

		/* TODO: Update the following registry keys:

		\\LOCAL MACHINE\Software\Microsoft\DevStudio\6.0\Build System\Components\Platforms\Win32 (x86)\Directories\Include Dirs
		\\LOCAL MACHINE\Software\Microsoft\DevStudio\6.0\Build System\Components\Platforms\Win32 (x86)\Directories\Library Dirs
		\\LOCAL MACHINE\Software\Microsoft\DevStudio\6.0\Build System\Components\Platforms\Win32 (x86)\Directories\Path Dirs
		\\LOCAL MACHINE\Software\Microsoft\DevStudio\6.0\Build System\Components\Platforms\Win32 (x86)\Directories\Source Dirs
		*/
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Workspace creation:

Workspace CConfigureApp::create_workspace(CConfigureWizard &wizard)
{
	Workspace workspace;
	workspace.input_lib_dir = std::string(wizard.m_Page4.m_input_lib);
	workspace.input_include_dir = std::string(wizard.m_Page4.m_input_include);
	workspace.output_include_dir = std::string(wizard.m_Page4.m_output_include);
	workspace.output_lib_dir = std::string(wizard.m_Page4.m_output_lib);

	// Write all library project files:
	std::list<std::string> libs_list_shared;
	std::list<std::string> libs_list_release;
	std::list<std::string> libs_list_debug;
	std::list<std::string> defines_list;

	libs_list_shared.push_back("winmm.lib");
	libs_list_shared.push_back("ddraw.lib");
	libs_list_shared.push_back("dsound.lib");
	libs_list_shared.push_back("dinput.lib");
	libs_list_shared.push_back("dxguid.lib");
	libs_list_shared.push_back("ws2_32.lib");
	libs_list_shared.push_back("zlib.lib");
	
	if (wizard.m_Page3.m_use_opengl) defines_list.push_back("USE_OPENGL");
	if (wizard.m_Page3.m_use_network) defines_list.push_back("USE_NETWORK");
	if (wizard.m_Page3.m_use_sound) defines_list.push_back("USE_CLANSOUND");
	
	defines_list.push_back("DIRECTINPUT_VERSION=0x0700");

	Project clanCore(
		"Core",
		"clanCore",
		"core.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	//clean the lib list
	libs_list_shared.clear();
	libs_list_debug.clear();
	libs_list_release.clear();

	Project clanSignals(
		"Signals",
		"clanSignals",
		"signals.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	Project clanApp(
		"Application",
		"clanApp",
		"application.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	Project clanNetwork(
		"Network",
		"clanNetwork",
		"network.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	Project clanNetwork2(
		"Network2",
		"clanNetwork2",
		"network2.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	libs_list_shared.push_back("hermes.lib");
//	libs_list_release.push_back("hermes.lib");
//	libs_list_debug.push_back("hermes.lib");

	Project clanDisplay(
		"Display",
		"clanDisplay",
		"display.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	libs_list_release.clear();
	libs_list_debug.clear();

	Project clanSound(
		"Sound",
		"clanSound",
		"sound.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	//clean the lib list
	libs_list_shared.clear();
	libs_list_debug.clear();
	libs_list_release.clear();

	// libs for clanGL
	libs_list_shared.push_back("OpenGL32.lib");
	libs_list_shared.push_back("GLU32.lib");
		
	Project clanGL(
		"GL",
		"clanGL",
		"gl.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	//clean the lib list
	libs_list_shared.clear();

	Project clanGUI(
		"GUI",
		"clanGUI",
		"gui.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	//clean the lib list
	libs_list_shared.clear();
	libs_list_debug.clear();
	libs_list_release.clear();

	// libs for clanJavaScript
	libs_list_shared.push_back("js32.lib");
		
	Project clanJavaScript(
		"JavaScript",
		"clanJavaScript",
		"javascript.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);
	
	//clean the lib list
	libs_list_shared.clear();
	libs_list_debug.clear();
	libs_list_release.clear();
/*
	libs_list_debug.push_back("luad.lib");
	libs_list_debug.push_back("lualibd.lib");
	libs_list_debug.push_back("tolualibd.lib");
	libs_list_release.push_back("lua.lib");
	libs_list_release.push_back("lualib.lib");
	libs_list_release.push_back("tolualib.lib");

	Project clanLua(
		"Lua",
		"clanLua",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);
*/
	//clean the lib list
	libs_list_shared.clear();
	libs_list_debug.clear();
	libs_list_release.clear();

	libs_list_shared.push_back("vorbis.lib");
	libs_list_shared.push_back("ogg.lib");

	Project clanVorbis(
		"Vorbis",
		"clanVorbis",
		"vorbis.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	libs_list_shared.clear();
	libs_list_debug.clear();
	libs_list_release.clear();

	libs_list_debug.push_back("mikmod.lib");
	libs_list_release.push_back("mikmod.lib");

	Project clanMikMod(
		"MikMod",
		"clanMikMod",
		"mikmod.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	libs_list_shared.clear();
	libs_list_debug.clear();
	libs_list_release.clear();

	libs_list_shared.push_back("jpeg.lib");

	Project clanJPEG(
		"JPEG",
		"clanJPEG",
		"jpeg.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	libs_list_shared.clear();

	libs_list_shared.push_back("libpng.lib");
	
	Project clanPNG(
		"PNG",
		"clanPNG",
		"png.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	libs_list_shared.clear();
	libs_list_debug.push_back("freetype200b8MT.lib");
	libs_list_release.push_back("freetype200b8MT.lib");

	Project clanTTF(
		"TTF",
		"clanTTF",
		"ttf.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	libs_list_shared.clear();
	libs_list_debug.clear();
	libs_list_release.clear();

	Project clanSmallJPEG(
		"SmallJPEG",
		"clanSmallJPEG",
		"smalljpeg.h",
		libs_list_shared,
		libs_list_release,
		libs_list_debug,
		defines_list);

	// Setup project dependencies:

	// Add projects to workspace:
	workspace.projects.push_back(clanSignals);
	workspace.projects.push_back(clanCore);
	workspace.projects.push_back(clanApp);
	workspace.projects.push_back(clanNetwork);
	workspace.projects.push_back(clanNetwork2);
	workspace.projects.push_back(clanDisplay);
	workspace.projects.push_back(clanSound);
	workspace.projects.push_back(clanGL);
	workspace.projects.push_back(clanGUI);
	workspace.projects.push_back(clanJavaScript);
//	workspace.projects.push_back(clanLua);
	workspace.projects.push_back(clanVorbis);
	workspace.projects.push_back(clanMikMod);
//	workspace.projects.push_back(clanMPEG);
	workspace.projects.push_back(clanJPEG);
	workspace.projects.push_back(clanPNG);
	workspace.projects.push_back(clanTTF);
	workspace.projects.push_back(clanSmallJPEG);

	return workspace;
}

/////////////////////////////////////////////////////////////////////////////
// Browse helper function:

BOOL BrowseForFolder(HWND hOwner, char* szTitle, char* szRetval)
{
	BROWSEINFO		info;
	LPITEMIDLIST	itemidlist;
	char			szDirectory[_MAX_PATH];
	LPMALLOC		pMalloc;

	memset(szDirectory, '\0', _MAX_PATH);

	if (::SHGetMalloc(&pMalloc) == NOERROR)
	{
		info.hwndOwner = hOwner;
		info.pidlRoot = NULL;
		info.pszDisplayName = szDirectory;
		info.lpszTitle = szTitle;
		info.ulFlags = 0;
		info.lpfn = NULL;

		itemidlist = SHBrowseForFolder(&info);
		if (itemidlist != NULL)
		{
			SHGetPathFromIDList(itemidlist, szRetval);
			pMalloc->Free(itemidlist);
			pMalloc->Release();
			return TRUE;
		}
		else // User clicked Cancel
		{
			pMalloc->Release();
			return FALSE;
		}
	}
	else
		return FALSE;
}

void CConfigureApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	ShellExecute(
		NULL,
		NULL,
		"notepad.exe",
		"INSTALL.WIN32",
		NULL,
		SW_SHOWNORMAL);
}
