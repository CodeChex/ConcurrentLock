# Microsoft Developer Studio Project File - Name="ConcurrentLock" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ConcurrentLock - Win32 Debug UNICODE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ConcurrentLock.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ConcurrentLock.mak" CFG="ConcurrentLock - Win32 Debug UNICODE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ConcurrentLock - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ConcurrentLock - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ConcurrentLock - Win32 Debug UNICODE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ConcurrentLock - Win32 Release UNICODE" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "/ConcurrentLock?PVCS Version Manager?C:\Program Files\PVCS\VM\vmdevint?"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ConcurrentLock - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /O2 /Ob0 /I "..\include" /I "K:\dev\include" /D "NDEBUG" /D "STRICT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /I "..\include" /I "K:\dev\include" /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\include" /i "K:\dev\include" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /libpath:"..\lib" /libpath:"k:\dev\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy    concurrentlock.h    ..\redist\   	copy    Release\*.dll    ..\redist\release\   	copy    Release\*.lib    ..\redist\release\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ConcurrentLock - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\include" /I "K:\dev\include" /D "_DEBUG" /D "STRICT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D lockDLLEXPORT="_declspec(dllexport)" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /I "..\include" /I "K:\dev\include" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\include" /i "K:\dev\include" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /incremental:no /debug /debugtype:both /machine:I386 /libpath:"..\lib" /libpath:"k:\dev\lib"
# SUBTRACT LINK32 /profile /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy    concurrentlock.h    ..\redist\   	copy    Debug\*.dll    ..\redist\debug\   	copy    Debug\*.lib    ..\redist\debug\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ConcurrentLock - Win32 Debug UNICODE"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ConcurrentLock___Win32_Debug_UNICODE"
# PROP BASE Intermediate_Dir "ConcurrentLock___Win32_Debug_UNICODE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\include" /I "K:\dev\include" /D "_DEBUG" /D "STRICT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE" /D "UNICODE" /D lockDLLEXPORT="_declspec(dllexport)" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\include" /I "K:\dev\include" /D "_DEBUG" /D "STRICT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE" /D "UNICODE" /D lockDLLEXPORT="_declspec(dllexport)" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /I "..\include" /I "K:\dev\include" /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /I "..\include" /I "K:\dev\include" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "..\include" /i "K:\dev\include" /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\include" /i "K:\dev\include" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /incremental:no /debug /debugtype:both /machine:I386 /libpath:"..\lib" /libpath:"k:\dev\lib"
# SUBTRACT BASE LINK32 /profile /pdb:none
# ADD LINK32 /nologo /subsystem:windows /dll /incremental:no /debug /debugtype:both /machine:I386 /out:"DebugU/ConcurrentLockU.dll" /libpath:"..\lib" /libpath:"k:\dev\lib"
# SUBTRACT LINK32 /profile /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy    concurrentlock.h    ..\redist\   	copy    DebugU\*.dll    ..\redist\debug\   	copy    DebugU\*.lib    ..\redist\debug\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ConcurrentLock - Win32 Release UNICODE"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ConcurrentLock___Win32_Release_UNICODE"
# PROP BASE Intermediate_Dir "ConcurrentLock___Win32_Release_UNICODE"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseU"
# PROP Intermediate_Dir "ReleaseU"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GR /GX /O2 /Ob0 /I "..\include" /I "K:\dev\include" /D "NDEBUG" /D "STRICT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE" /D "UNICODE" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /O2 /Ob0 /I "..\include" /I "K:\dev\include" /D "NDEBUG" /D "STRICT" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE" /D "UNICODE" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /I "..\include" /I "K:\dev\include" /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /I "..\include" /I "K:\dev\include" /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "..\include" /i "K:\dev\include" /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\include" /i "K:\dev\include" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /libpath:"..\lib" /libpath:"k:\dev\lib"
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"ReleaseU/ConcurrentLockU.dll" /libpath:"..\lib" /libpath:"k:\dev\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy    concurrentlock.h    ..\redist\   	copy    ReleaseU\*.dll    ..\redist\release\   	copy    ReleaseU\*.lib    ..\redist\release\ 
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "ConcurrentLock - Win32 Release"
# Name "ConcurrentLock - Win32 Debug"
# Name "ConcurrentLock - Win32 Debug UNICODE"
# Name "ConcurrentLock - Win32 Release UNICODE"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ConcurrentLock.cpp
# End Source File
# Begin Source File

SOURCE=.\ConcurrentLock.rc
# End Source File
# Begin Source File

SOURCE=.\concurrentlock_dll.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\cfgRegFunctions.h
# End Source File
# Begin Source File

SOURCE=.\ConcurrentLock.h
# End Source File
# Begin Source File

SOURCE=.\ConcurrentLock_Private.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\ConcurrentLock.rc2
# End Source File
# End Group
# End Target
# End Project
