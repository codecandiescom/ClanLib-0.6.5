# Microsoft Developer Studio Project File - Name="ExampleNetObjects" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ExampleNetObjects - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NetObjects.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NetObjects.mak" CFG="ExampleNetObjects - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ExampleNetObjects - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ExampleNetObjects - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ExampleNetObjects - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x406 /d "NDEBUG"
# ADD RSC /l 0x406 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "ExampleNetObjects - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "_MBCS" /D "__STL_DEBUG" /D "WIN32" /D "_WINDOWS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x406 /d "_DEBUG"
# ADD RSC /l 0x406 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /debug /machine:I386 /nodefaultlib:"libcmt"

!ENDIF 

# Begin Target

# Name "ExampleNetObjects - Win32 Release"
# Name "ExampleNetObjects - Win32 Debug"
# Begin Group "OpenGL Style"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Gui\button_opengl.cpp
# End Source File
# Begin Source File

SOURCE=.\Gui\button_opengl.h
# End Source File
# Begin Source File

SOURCE=.\Gui\checkbox_opengl.cpp
# End Source File
# Begin Source File

SOURCE=.\Gui\checkbox_opengl.h
# End Source File
# Begin Source File

SOURCE=.\Gui\frame_opengl.cpp
# End Source File
# Begin Source File

SOURCE=.\Gui\frame_opengl.h
# End Source File
# Begin Source File

SOURCE=.\Gui\inputbox_opengl.cpp
# End Source File
# Begin Source File

SOURCE=.\Gui\inputbox_opengl.h
# End Source File
# Begin Source File

SOURCE=.\Gui\listbox_opengl.cpp
# End Source File
# Begin Source File

SOURCE=.\Gui\listbox_opengl.h
# End Source File
# Begin Source File

SOURCE=.\Gui\scrollbar_opengl.cpp
# End Source File
# Begin Source File

SOURCE=.\Gui\scrollbar_opengl.h
# End Source File
# Begin Source File

SOURCE=.\Gui\stylemanager_opengl.cpp
# End Source File
# Begin Source File

SOURCE=.\Gui\stylemanager_opengl.h
# End Source File
# Begin Source File

SOURCE=.\Gui\window_opengl.cpp
# End Source File
# Begin Source File

SOURCE=.\Gui\window_opengl.h
# End Source File
# End Group
# Begin Group "Menus"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\about_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\about_dialog.h
# End Source File
# Begin Source File

SOURCE=.\mainmenu.cpp
# End Source File
# Begin Source File

SOURCE=.\mainmenu.h
# End Source File
# Begin Source File

SOURCE=.\optionsmenu.cpp
# End Source File
# Begin Source File

SOURCE=.\optionsmenu.h
# End Source File
# End Group
# Begin Group "Game World"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\fighter.cpp
# End Source File
# Begin Source File

SOURCE=.\fighter.h
# End Source File
# Begin Source File

SOURCE=.\gameobject.cpp
# End Source File
# Begin Source File

SOURCE=.\gameobject.h
# End Source File
# Begin Source File

SOURCE=.\planet.cpp
# End Source File
# Begin Source File

SOURCE=.\planet.h
# End Source File
# Begin Source File

SOURCE=.\player.cpp
# End Source File
# Begin Source File

SOURCE=.\player.h
# End Source File
# Begin Source File

SOURCE=.\players.cpp
# End Source File
# Begin Source File

SOURCE=.\players.h
# End Source File
# Begin Source File

SOURCE=.\world.cpp
# End Source File
# Begin Source File

SOURCE=.\world.h
# End Source File
# Begin Source File

SOURCE=.\wormhole.cpp
# End Source File
# Begin Source File

SOURCE=.\wormhole.h
# End Source File
# End Group
# Begin Group "Server World"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\server_world.cpp
# End Source File
# Begin Source File

SOURCE=.\server_world.h
# End Source File
# End Group
# Begin Group "Client World"

# PROP Default_Filter ""
# Begin Group "Sprite"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\sprite.h
# End Source File
# Begin Source File

SOURCE=.\spritedata.cpp
# End Source File
# Begin Source File

SOURCE=.\spritedata.h
# End Source File
# Begin Source File

SOURCE=.\texture.cpp
# End Source File
# Begin Source File

SOURCE=.\texture.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\client_fighter.cpp
# End Source File
# Begin Source File

SOURCE=.\client_fighter.h
# End Source File
# Begin Source File

SOURCE=.\client_gameobject.cpp
# End Source File
# Begin Source File

SOURCE=.\client_gameobject.h
# End Source File
# Begin Source File

SOURCE=.\client_planet.cpp
# End Source File
# Begin Source File

SOURCE=.\client_planet.h
# End Source File
# Begin Source File

SOURCE=.\client_world.cpp
# End Source File
# Begin Source File

SOURCE=.\client_world.h
# End Source File
# Begin Source File

SOURCE=.\client_wormhole.cpp
# End Source File
# Begin Source File

SOURCE=.\client_wormhole.h
# End Source File
# End Group
# Begin Group "Misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\.cvsignore
# End Source File
# Begin Source File

SOURCE=.\Makefile
# End Source File
# Begin Source File

SOURCE=.\resources.scr
# End Source File
# End Group
# Begin Group "Lobby"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lobby\client_lobby.cpp
# End Source File
# Begin Source File

SOURCE=.\lobby\client_lobby.h
# End Source File
# Begin Source File

SOURCE=.\lobby\lobby.h
# End Source File
# Begin Source File

SOURCE=.\lobby\lobby_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\lobby\lobby_dialog.h
# End Source File
# Begin Source File

SOURCE=.\Lobby\lobby_player.h
# End Source File
# Begin Source File

SOURCE=.\Lobby\lobby_players.cpp
# End Source File
# Begin Source File

SOURCE=.\Lobby\lobby_players.h
# End Source File
# Begin Source File

SOURCE=.\lobby\server_lobby.cpp
# End Source File
# Begin Source File

SOURCE=.\lobby\server_lobby.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\intro.cpp
# End Source File
# Begin Source File

SOURCE=.\intro.h
# End Source File
# Begin Source File

SOURCE=.\netobjects.cpp
# End Source File
# Begin Source File

SOURCE=.\netobjects.h
# End Source File
# Begin Source File

SOURCE=.\network.h
# End Source File
# Begin Source File

SOURCE=.\options.cpp
# End Source File
# Begin Source File

SOURCE=.\options.h
# End Source File
# Begin Source File

SOURCE=.\README.txt
# End Source File
# Begin Source File

SOURCE=.\TODO.txt
# End Source File
# End Target
# End Project
