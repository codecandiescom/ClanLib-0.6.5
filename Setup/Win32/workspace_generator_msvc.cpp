/*
	$Id: workspace_generator_msvc.cpp,v 1.56 2002/11/03 10:17:32 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "workspace_generator_msvc.h"

/////////////////////////////////////////////////////////////////////////////
// WorkspaceGenerator_MSVC construction:

WorkspaceGenerator_MSVC::WorkspaceGenerator_MSVC()
{
}

/////////////////////////////////////////////////////////////////////////////
// WorkspaceGenerator_MSVC attributes:

/////////////////////////////////////////////////////////////////////////////
// WorkspaceGenerator_MSVC operations:

void WorkspaceGenerator_MSVC::write(const Workspace &workspace)
{
	write_dsw(workspace);

	std::list<Project>::const_iterator itProject;
	for (itProject = workspace.projects.begin(); itProject != workspace.projects.end(); itProject++)
	{
		const Project &project = *itProject;
		write_dsp(workspace, project);
	}
}

/////////////////////////////////////////////////////////////////////////////
// WorkspaceGenerator_MSVC implementation:

void WorkspaceGenerator_MSVC::write_dsw(const Workspace &workspace)
{
	std::ofstream dsw("ClanLib.dsw");

	dsw << "Microsoft Developer Studio Workspace File, Format Version 6.00" << std::endl;
	dsw << "# WARNING: DO NOT EDIT OR DELETE THIS WORKSPACE FILE!" << std::endl;
	dsw << std::endl;
	dsw << "###############################################################################" << std::endl;
	dsw << std::endl;

	std::string dsp_path(".\\Sources\\");

	std::list<Project>::const_iterator itProject;
	for (itProject = workspace.projects.begin(); itProject != workspace.projects.end(); itProject++)
	{
		const Project &project = *itProject;

		begin_project(dsw, project.libname, dsp_path+project.libname+std::string(".dsp"));

		std::list<std::string>::const_iterator it;
		for (it = project.dependencies.begin(); it != project.dependencies.end(); it++)
		{
			const std::string &dep = *it;
			add_project_dependency(dsw, dep);
		}

		end_project(dsw);
	}

	begin_project(dsw, "ExampleAlphaTest", ".\\Documentation\\Examples\\AlphaTest\\AlphaTest.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanJPEG");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleCanvas", ".\\Documentation\\Examples\\Canvas\\Canvas.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleCompiler", ".\\Documentation\\Examples\\Compiler\\Compiler.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSound");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleCTalk", ".\\Documentation\\Examples\\CTalk\\CTalk.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanNetwork");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleDirectory", ".\\Documentation\\Examples\\Directory\\Directory.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "Example2D", ".\\Documentation\\Examples\\Example2D\\Example2D.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleFTP", ".\\Documentation\\Examples\\ClanFTP\\ExampleFTP.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanNetwork");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleFont", ".\\Documentation\\Examples\\Font\\ExampleFont.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

 	begin_project(dsw, "ExampleGL", ".\\Documentation\\Examples\\GL\\GL.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleGLLineDemo", ".\\Documentation\\Examples\\GLLineDemo\\GLLineDemo.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanPNG");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

 	begin_project(dsw, "ExampleGLFog", ".\\Documentation\\Examples\\GL\\ClanFog.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleGUIGL", ".\\Documentation\\Examples\\GUIGL\\GUIGL.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleGUIDynamic", ".\\Documentation\\Examples\\GUITest\\GUIDynamic.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleGUIScript", ".\\Documentation\\Examples\\GUITest\\GUIScript.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleGUIEditor", ".\\Documentation\\Examples\\GUIEditor\\GUIEditor.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleInitDisplay", ".\\Documentation\\Examples\\InitDisplay\\InitDisplay.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleInputDevices", ".\\Documentation\\Examples\\InputDevices\\InputDevices.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleJavaScript", ".\\Documentation\\Examples\\JavaScript\\JavaScript.dsp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanJavaScript");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleJPEG", ".\\Documentation\\Examples\\JPEG\\JPEG.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanJPEG");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);
/*
	begin_project(dsw, "ExampleLUA", ".\\Documentation\\Examples\\Lua\\Lua.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanLua");
	add_project_dependency(dsw, "clanDisplay");
	end_project(dsw);
*/
	begin_project(dsw, "ExampleMikMod", ".\\Documentation\\Examples\\MikMod\\MikMod.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanMikMod");
	add_project_dependency(dsw, "clanSound");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleMinimum", ".\\Documentation\\Examples\\Minimum\\Minimum.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleMouse", ".\\Documentation\\Examples\\Mouse\\Mouse.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleNetBrowse", ".\\Documentation\\Examples\\NetBrowse\\NetBrowse.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGUI");
	add_project_dependency(dsw, "clanNetwork");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleNetChannel", ".\\Documentation\\Examples\\NetChannel\\NetChannel.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanNetwork");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleNetChannel2", ".\\Documentation\\Examples\\NetChannel2\\NetChannel2.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanNetwork2");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleNetObjects", ".\\Documentation\\Examples\\NetObjects\\NetObjects.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanGL");
	add_project_dependency(dsw, "clanNetwork");
	add_project_dependency(dsw, "clanGUI");
//	add_project_dependency(dsw, "clanVorbis");
//	add_project_dependency(dsw, "clanTTF");
	add_project_dependency(dsw, "clanPNG");
	add_project_dependency(dsw, "clanJPEG");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSound");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExamplePlasma", ".\\Documentation\\Examples\\Plasma\\Plasma.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExamplePlayCD", ".\\Documentation\\Examples\\PlayCD\\PlayCD.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSound");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleResources", ".\\Documentation\\Examples\\Resources\\Resources.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleScreenShot", ".\\Documentation\\Examples\\ScreenShot\\ScreenShot.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanPNG");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleSignals", ".\\Documentation\\Examples\\Signals\\Signals.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleSoundBuffer", ".\\Documentation\\Examples\\SoundBuffer\\SoundBuffer.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSound");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleStaticSoundProvider", ".\\Documentation\\Examples\\StaticSoundProvider\\StaticSoundProvider.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSound");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleStreamSoundProvider", ".\\Documentation\\Examples\\StreamSoundProvider\\StreamSoundProvider.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSound");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleSurface", ".\\Documentation\\Examples\\Surface\\Surface.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleSurfaceProvider", ".\\Documentation\\Examples\\SurfaceProvider\\SurfaceProvider.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleThreadSync", ".\\Documentation\\Examples\\ThreadSync\\ThreadSync.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleTTF", ".\\Documentation\\Examples\\TTF\\TTF.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanTTF");
	add_project_dependency(dsw, "clanDisplay");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleUDPTunnel", ".\\Documentation\\Examples\\UDPTunnel\\UDPTunnel.dsp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanNetwork");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	begin_project(dsw, "ExampleVorbis", ".\\Documentation\\Examples\\Vorbis\\Vorbis.dsp");
	add_project_dependency(dsw, "clanApp");
	add_project_dependency(dsw, "clanCore");
	add_project_dependency(dsw, "clanVorbis");
	add_project_dependency(dsw, "clanSound");
	add_project_dependency(dsw, "clanSignals");
	end_project(dsw);

	dsw << "Global:" << std::endl;
	dsw << std::endl;
	dsw << "Package=<5>" << std::endl;
	dsw << "{{{" << std::endl;
	dsw << "}}}" << std::endl;
	dsw << std::endl;
	dsw << "Package=<3>" << std::endl;
	dsw << "{{{" << std::endl;
	dsw << "}}}" << std::endl;
	dsw << std::endl;
	dsw << "###############################################################################" << std::endl;
	dsw << std::endl;
}

void WorkspaceGenerator_MSVC::begin_project(
	std::ofstream &dsw,
	const std::string &name,
	const std::string &filename)
{
	dsw << "Project: \"" << name.c_str() << "\"=\"" << filename.c_str() << "\" - Package Owner=<4>" << std::endl;
	dsw << std::endl;
	dsw << "Package=<5>" << std::endl;
	dsw << "{{{" << std::endl;
	dsw << "}}}" << std::endl;
	dsw << std::endl;
	dsw << "Package=<4>" << std::endl;
	dsw << "{{{" << std::endl;
}

void WorkspaceGenerator_MSVC::end_project(std::ofstream &dsw)
{
	dsw << "}}}" << std::endl;
	dsw << std::endl;
	dsw << "###############################################################################" << std::endl;
	dsw << std::endl;
}

void WorkspaceGenerator_MSVC::add_project_dependency(
	std::ofstream &dsw,
	const std::string &dep_name)
{
	dsw << "    Begin Project Dependency" << std::endl;
	dsw << "    Project_Dep_Name " << dep_name.c_str() << std::endl;
	dsw << "    End Project Dependency" << std::endl;
}

void WorkspaceGenerator_MSVC::write_dsp(const Workspace &workspace, const Project &project)
{
	CString filename = "Sources\\";
	filename += project.libname.c_str();
	filename += ".dsp";

	std::ofstream dsp(filename);

	dsp << "# Microsoft Developer Studio Project File - Name=\"" << project.libname.c_str() << "\" - Package Owner=<4>" << std::endl;
	dsp << "# Microsoft Developer Studio Generated Build File, Format Version 6.00" << std::endl;
	dsp << "# ** DO NOT EDIT **" << std::endl;
	dsp << std::endl;
	dsp << "# TARGTYPE \"Win32 (x86) Static Library\" 0x0104" << std::endl;
	dsp << std::endl;
	dsp << "CFG=" << project.libname.c_str() << " - Win32 Release" << std::endl;
	dsp << "!MESSAGE This is not a valid makefile. To build this project using NMAKE," << std::endl;
	dsp << "!MESSAGE use the Export Makefile command and run" << std::endl;
	dsp << "!MESSAGE " << std::endl;
	dsp << "!MESSAGE NMAKE /f \"" << project.libname.c_str() << ".mak\"." << std::endl;
	dsp << "!MESSAGE " << std::endl;
	dsp << "!MESSAGE You can specify a configuration when running NMAKE" << std::endl;
	dsp << "!MESSAGE by defining the macro CFG on the command line. For example:" << std::endl;
	dsp << "!MESSAGE " << std::endl;
	dsp << "!MESSAGE NMAKE /f \"" << project.libname.c_str() << ".mak\" CFG=\"" << project.libname.c_str() << " - Win32 Release\"" << std::endl;
	dsp << "!MESSAGE " << std::endl;
	dsp << "!MESSAGE Possible choices for configuration are:" << std::endl;
	dsp << "!MESSAGE " << std::endl;
	dsp << "!MESSAGE \"" << project.libname.c_str() << " - Win32 Release\" (based on \"Win32 (x86) Static Library\")" << std::endl;
	dsp << "!MESSAGE \"" << project.libname.c_str() << " - Win32 Debug\" (based on \"Win32 (x86) Static Library\")" << std::endl;
	dsp << "!MESSAGE " << std::endl;
	dsp << std::endl;
	dsp << "# Begin Project" << std::endl;
	dsp << "# PROP AllowPerConfigDependencies 0" << std::endl;
	dsp << "# PROP Scc_ProjName \"\"" << std::endl;
	dsp << "# PROP Scc_LocalPath \"\"" << std::endl;
	dsp << "CPP=cl.exe" << std::endl;
	dsp << "RSC=rc.exe" << std::endl;
	dsp << std::endl;
	dsp << "!IF  \"$(CFG)\" == \"" << project.libname.c_str() << " - Win32 Release\"" << std::endl;
	dsp << std::endl;
	dsp << "# PROP BASE Use_MFC 0" << std::endl;
	dsp << "# PROP BASE Use_Debug_Libraries 0" << std::endl;
	dsp << "# PROP BASE Output_Dir \"../Release\"" << std::endl;
	dsp << "# PROP BASE Intermediate_Dir \"../Release/" << project.libname.c_str() << "\"" << std::endl;
	dsp << "# PROP BASE Target_Dir \"\"" << std::endl;
	dsp << "# PROP Use_MFC 0" << std::endl;
	dsp << "# PROP Use_Debug_Libraries 0" << std::endl;
	dsp << "# PROP Output_Dir \"../Release\"" << std::endl;
	dsp << "# PROP Intermediate_Dir \"../Release/" << project.libname.c_str() << "\"" << std::endl;
	dsp << "# PROP Target_Dir \"\"" << std::endl;
	dsp << "# ADD BASE CPP /MT /nologo /W3 /GR /GX /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_WINDOWS\"";
    if (project.name == "Core") // use precomp headers in clanCore.
		dsp << " /Yu\"Core\\precomp.h\" /FD /c" << std::endl;
	else if (project.name == "GUI")
		dsp << " /Yu\"precomp.h\" /FD /c" << std::endl;
	else if (project.name == "Display")
		dsp << " /Yu\"Core\\precomp.h\" /FD /c" << std::endl;
	else
		dsp << " /YX /FD /c" << std::endl;

	dsp << "# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I \".\"";
	if (!workspace.input_include_dir.empty()) dsp << " /I \"" << workspace.input_include_dir.c_str() << "\"";
	dsp << " /D \"NDEBUG\" /D \"WIN32\" /D \"_WINDOWS\"";
	{
		for (
			std::list<std::string>::const_iterator it = project.defines_list.begin();
			it != project.defines_list.end();
			it++)
		{
			dsp << " /D \"" << (*it).c_str() << "\"";
		}
	}
    if (project.name == "Core") // use precomp headers in clanCore.
		dsp << " /Yu\"Core\\precomp.h\" /FD /c" << std::endl;
    else if (project.name == "GUI") // use precomp headers in clanCore.
		dsp << " /Yu\"precomp.h\" /FD /c" << std::endl;
	else if (project.name == "Display")
		dsp << " /Yu\"Core\\precomp.h\" /FD /c" << std::endl;
	else
		dsp << " /YX /FD /c" << std::endl;

	dsp << "# ADD BASE RSC /l 0x409 /d \"NDEBUG\"" << std::endl;
	dsp << "# ADD RSC /l 0x409 /d \"NDEBUG\"" << std::endl;
	dsp << "BSC32=bscmake.exe" << std::endl;
	dsp << "# ADD BASE BSC32 /nologo" << std::endl;
	dsp << "# ADD BSC32 /nologo" << std::endl;
	dsp << "LIB32=link.exe -lib" << std::endl;
	dsp << "# ADD BASE LIB32 /nologo";
	if (!workspace.input_lib_dir.empty()) dsp << " /libpath:\"" << workspace.input_lib_dir.c_str() << "\"";
	dsp << std::endl;

	dsp << "# ADD LIB32 ";
	{
		for (
			std::list<std::string>::const_iterator it = project.libs_shared.begin();
			it != project.libs_shared.end();
			it++)
		{
			dsp << " " << (*it).c_str() << "";
		}
	}
	{
		for (
			std::list<std::string>::const_iterator it = project.libs_release.begin();
			it != project.libs_release.end();
			it++)
		{
			dsp << " " << (*it).c_str() << "";
		}
	}
	dsp << " /nologo /out:\"" << workspace.output_lib_dir.c_str() << "\\" << project.libname.c_str() << ".lib\"" << std::endl;

	dsp << "# Begin Special Build Tool" << std::endl;
	dsp << "SOURCE=\"$(InputPath)\"" << std::endl;
	dsp << "PostBuild_Desc=Installing library and API headers..." << std::endl;
	dsp << "PostBuild_Cmds=call install_" << project.libname.c_str() << ".bat\t";
//	dsp << "copy ..\\Release\\" << project.libname.c_str() << ".lib \"" << output_lib_dir.c_str() << "\"";

	// create the install_libname.bat installation batch file:
	{
		CString bat_file = "Sources\\install_";
		bat_file += project.libname.c_str();
		bat_file += ".bat";

		std::ofstream bat(bat_file);

		CString instdir = workspace.output_include_dir.c_str();
		instdir += "\\ClanLib";
		install_mkdir(bat, "API\\", std::string(instdir), &project);
		install_copydir(bat, "API\\", std::string(instdir), &project);
	}

	dsp << std::endl;
	dsp << "# End Special Build Tool" << std::endl;

	dsp << std::endl;
	dsp << "!ELSEIF  \"$(CFG)\" == \"" << project.libname.c_str() << " - Win32 Debug\"" << std::endl;
	dsp << std::endl;
	dsp << "# PROP BASE Use_MFC 0" << std::endl;
	dsp << "# PROP BASE Use_Debug_Libraries 1" << std::endl;
	dsp << "# PROP BASE Output_Dir \"../Debug\"" << std::endl;
	dsp << "# PROP BASE Intermediate_Dir \"../Debug/" << project.libname.c_str() << "\"" << std::endl;
	dsp << "# PROP BASE Target_Dir \"\"" << std::endl;
	dsp << "# PROP Use_MFC 0" << std::endl;
	dsp << "# PROP Use_Debug_Libraries 1" << std::endl;
	dsp << "# PROP Output_Dir \"../Debug\"" << std::endl;
	dsp << "# PROP Intermediate_Dir \"../Debug/" << project.libname.c_str() << "\"" << std::endl;
	dsp << "# PROP Target_Dir \"\"" << std::endl;
	dsp << "# ADD BASE CPP /MTd /nologo /W3 /Gm /GR /GX /Zi /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_WINDOWS\"";
	if (project.name == "Core") // use precomp headers in clanCore.
		dsp << " /Yu\"Core\\precomp.h\" /FD /GZ /c" << std::endl;
	else if (project.name == "GUI") // use precomp headers in clanCore.
		dsp << " /Yu\"precomp.h\" /FD /GZ /c" << std::endl;
	else if (project.name == "Display")
		dsp << " /Yu\"Core\\precomp.h\" /FD /GZ /c" << std::endl;
	else
		dsp << " /YX /FD /GZ /c" << std::endl;


	dsp << "# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /Zi /Od /I \".\"";
	if (!workspace.input_include_dir.empty()) dsp << " /I \"" << workspace.input_include_dir.c_str() << "\"";
	dsp << " /D \"NDEBUG\" /D \"WIN32\" /D \"_WINDOWS\"";

	{
		for (
			std::list<std::string>::const_iterator it = project.defines_list.begin();
			it != project.defines_list.end();
			it++)
		{
			dsp << " /D \"" << (*it).c_str() << "\"";
		}
	}
	if (project.name == "Core") // use precomp headers in clanCore.
		dsp << " /Yu\"Core\\precomp.h\" /FD /GZ /c" << std::endl;
	else if (project.name == "GUI") // use precomp headers in clanGUI.
		dsp << " /Yu\"precomp.h\" /FD /GZ /c" << std::endl;
	if (project.name == "Display") // use precomp headers in clanDisplay.
		dsp << " /Yu\"Core\\precomp.h\" /FD /GZ /c" << std::endl;
	else
		dsp << " /YX /FD /GZ /c" << std::endl;

	dsp << "# ADD BASE RSC /l 0x409 /d \"_DEBUG\"" << std::endl;
	dsp << "# ADD RSC /l 0x409 /d \"_DEBUG\"" << std::endl;
	dsp << "BSC32=bscmake.exe" << std::endl;
	dsp << "# ADD BASE BSC32 /nologo" << std::endl;
	dsp << "# ADD BSC32 /nologo" << std::endl;
	dsp << "LIB32=link.exe -lib" << std::endl;
	dsp << "# ADD BASE LIB32 /nologo";
	if (!workspace.input_lib_dir.empty()) dsp << " /libpath:\"" << workspace.input_lib_dir.c_str() << "\"";
	dsp << std::endl;

	dsp << "# ADD LIB32 ";
	{
		for (
			std::list<std::string>::const_iterator it = project.libs_shared.begin();
			it != project.libs_shared.end();
			it++)
		{
			dsp << " " << (*it).c_str() << "";
		}
	}
	{
		for (
			std::list<std::string>::const_iterator it = project.libs_debug.begin();
			it != project.libs_debug.end();
			it++)
		{
			dsp << " " << (*it).c_str() << "";
		}
	}
//	dsp << " /nologo /out:\"../Debug/" << project.libname.c_str() << "d.lib\"" << std::endl;
	dsp << " /nologo /out:\"" << workspace.output_lib_dir.c_str() << "\\" << project.libname.c_str() << "d.lib\"" << std::endl;

	dsp << "# Begin Special Build Tool" << std::endl;
	dsp << "SOURCE=\"$(InputPath)\"" << std::endl;
	dsp << "PostBuild_Desc=Installing library and API headers..." << std::endl;
	dsp << "PostBuild_Cmds=call install_" << project.libname.c_str() << ".bat\t";

//	dsp << "copy ..\\Debug\\" << project.libname.c_str() << "d.lib \"" << workspace.output_lib_dir.c_str() << "\"";

	dsp << std::endl;
	dsp << "# End Special Build Tool" << std::endl;

	dsp << std::endl;
	dsp << "!ENDIF " << std::endl;
	dsp << std::endl;
	dsp << "# Begin Target" << std::endl;
	dsp << std::endl;
	dsp << "# Name \"" << project.libname.c_str() << " - Win32 Release\"" << std::endl;
	dsp << "# Name \"" << project.libname.c_str() << " - Win32 Debug\"" << std::endl;

	// Generate list of source files:
	generate_source_files(dsp, project);

	// End .dsp file:
	dsp << "# End Target" << std::endl;
	dsp << "# End Project" << std::endl;
}

void WorkspaceGenerator_MSVC::generate_source_files(std::ofstream &dsp, const Project &project)
{
	std::string chop_str = "Sources\\";
	unsigned int chop_length = chop_str.length();

	std::list<std::string>::iterator itCur, itNew;
	std::list<std::string> cur_path;

	std::list<std::string>::const_iterator itFiles;
	for (itFiles = project.files.begin(); itFiles != project.files.end(); itFiles++)
	{
		std::string file = *itFiles;

		// cut off path to files:
		if (file.length() > chop_length && file.substr(0, chop_length) == chop_str)
		{
			file = file.substr(chop_length);
		}

		std::list<std::string> new_path = extract_path(file);

		itCur = cur_path.begin();
		itNew = new_path.begin();
		while (itCur != cur_path.end())
		{
			if (itNew == new_path.end() || *itCur != *itNew) // path is no longer identical
			{
				while (itCur != cur_path.end())
				{
					end_group(dsp);
					itCur++;
				}
				break;
			}

			itCur++;
			itNew++;
		}
		while (itNew != new_path.end())
		{
			if (file.find("API") == std::string::npos) begin_group(dsp, *itNew + " files");
			else begin_group(dsp, *itNew);
			itNew++;
		}

		cur_path = new_path;

		add_file(dsp, file);
	}

	for (itCur = cur_path.begin(); itCur != cur_path.end(); itCur++) end_group(dsp);
}

std::list<std::string> WorkspaceGenerator_MSVC::extract_path(const std::string &fullname)
{
	std::list<std::string> path;
	int pos = 0;
	int old_pos = 0;

	while (true)
	{
		int pos = fullname.find("\\", old_pos);
		if (pos == std::string::npos) break;

		path.push_back(fullname.substr(old_pos, pos-old_pos));

		old_pos = pos + 1;
	}

	return path;
}

void WorkspaceGenerator_MSVC::begin_group(std::ofstream &dsp, const std::string &group_name)
{
	dsp << "# Begin Group \"" << group_name.c_str() << "\"" << std::endl;
	dsp << std::endl;
	dsp << "# PROP Default_Filter \"\"" << std::endl;
}

void WorkspaceGenerator_MSVC::end_group(std::ofstream &dsp)
{
	dsp << "# End Group" << std::endl;
}

void WorkspaceGenerator_MSVC::add_file(std::ofstream &dsp, const std::string &filename)
{
	dsp << "# Begin Source File" << std::endl;
	dsp << std::endl;
	dsp << "SOURCE=" << filename.c_str() << std::endl;

	if (filename == "Core\\precomp.cpp")
	{
		dsp << "# ADD CPP /Yc\"Core\\precomp.h\"" << std::endl;
	}
	else if (filename == "Display\\display_precomp.cpp")
	{
		dsp << "# ADD CPP /Yc\"Core\\precomp.h\"" << std::endl;
	}
	else if (filename == "GUI\\precomp.cpp")
	{
		dsp << "# ADD CPP /Yc\"precomp.h\"" << std::endl;
	}

	dsp << "# End Source File" << std::endl;
}

void WorkspaceGenerator_MSVC::install_mkdir(
	std::ofstream &dsp,
	const std::string &src_dir,
	const std::string &dest_dir,
	const Project *project)
{
	dsp << "if not exist \"" << dest_dir << "\\nul\"" << " mkdir \"" << dest_dir << "\"" << std::endl;

	CString path = src_dir.c_str();
	if (path.GetAt(path.GetLength() - 1) != '\\')
		path += '\\';

	CString path_dest = dest_dir.c_str();
	if (path_dest.GetAt(path_dest.GetLength() - 1) != '\\')
		path_dest += '\\';

	if (project)
	{
		// first time call: 
		// - make sure we create Module specific directories

		path = path + project->name.c_str();
		if (path.GetAt(path.GetLength() - 1) != '\\')
			path += '\\';

		path_dest = path_dest + project->name.c_str();

		dsp << "if not exist \"" << std::string(path_dest) << "\\nul\"" << " mkdir \"" << std::string(path_dest) << "\"" << std::endl;

		if (path_dest.GetAt(path_dest.GetLength() - 1) != '\\')
			path_dest += '\\';
	}

	CString prefix = "Sources\\";

	WIN32_FIND_DATA	data;
	HANDLE handle = FindFirstFile(prefix + path + "*.*", &data);
	if (handle == INVALID_HANDLE_VALUE) return;

	static const char *exclude_from_build[] =
	{
		".",
		"..",
		"CVS",
		".#", // don't add CVS revision backups.
		NULL
	};

	do
	{
		bool skip = false;

		for (int i=0; exclude_from_build[i] != NULL; i++)
			if (stricmp(data.cFileName, exclude_from_build[i]) == 0) skip = true;

		if (skip) continue;

		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
		{
			CString subdir = data.cFileName;
			install_mkdir(
				dsp,
				std::string(path + subdir),
				std::string(path_dest + subdir),
				0);
		}
	} while (FindNextFile(handle, &data));
}

void WorkspaceGenerator_MSVC::install_copydir(
	std::ofstream &dsp,
	const std::string &src_dir,
	const std::string &dest_dir,
	const Project *project)
{
	CString path = src_dir.c_str();
	if (path.GetAt(path.GetLength() - 1) != '\\')
		path += '\\';

	CString path_dest = dest_dir.c_str();
	if (path_dest.GetAt(path_dest.GetLength() - 1) != '\\')
		path_dest += '\\';

	if (project)
	{
		// first time call: 
		// - make sure we copy the 'module' api header
		// - make sure we get API\\ModuleName\*.h in this run

		dsp << "copy \"" << LPCTSTR (path) << LPCTSTR (project->headername.c_str()) << "\" \"" << LPCTSTR (path_dest) << LPCTSTR (project->headername.c_str()) << "\" > nul" << std::endl; // "\"\t";

		path += project->name.c_str();
		if (path.GetAt(path.GetLength() - 1) != '\\') path += '\\';
		
		path_dest += project->name.c_str();
		if (path_dest.GetAt(path_dest.GetLength() - 1) != '\\') path_dest += '\\';

	}

	CString prefix = "Sources\\";

	WIN32_FIND_DATA	data;
	HANDLE handle = FindFirstFile(prefix + path + "*.*", &data);
	if (handle == INVALID_HANDLE_VALUE) return;

	static const char *exclude_from_build[] =
	{
		".",
		"..",
		"CVS",
		".#", // don't add CVS revision backups.
		NULL
	};

	do
	{
		bool skip = false;

		for (int i=0; exclude_from_build[i] != NULL; i++)
			if (stricmp(data.cFileName, exclude_from_build[i]) == 0) skip = true;

		if (skip) continue;

		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
		{
			CString subdir = data.cFileName;
			install_copydir(
				dsp,
				std::string(path + subdir),
				std::string(path_dest + subdir),
				0);
		}
		else
		{
			CString file = data.cFileName;
			dsp << "copy \"" << LPCTSTR (path) << LPCTSTR (file) << "\" \"" << LPCTSTR (path_dest) << LPCTSTR (file) << "\" > nul" << std::endl; // "\"\t";
		}

	} while (FindNextFile(handle, &data));
}
