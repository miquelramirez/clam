# Microsoft Developer Studio Project File - Name="fortesting" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=fortesting - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fortesting.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fortesting.mak" CFG="fortesting - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fortesting - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "fortesting - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "CLAM_Skeleton_project"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fortesting - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\ReleaseFiles\bin"
# PROP Intermediate_Dir "..\ReleaseFiles\obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /I "unittests\source_files_for_testing\subdir" /I "unittests\source_files_for_testing\subdir\subsubdir"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "fortesting - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\DebugFiles\bin"
# PROP Intermediate_Dir "..\DebugFiles\obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /I "unittests\source_files_for_testing\subdir" /I "unittests\source_files_for_testing\subdir\subsubdir"
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "fortesting - Win32 Release"
# Name "fortesting - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group "unittests Sources"

# Begin Group "source_files_for_testing Sources"

# Begin Source File

SOURCE=unittests\source_files_for_testing\a.cxx
# End Source File
# Begin Source File

SOURCE=unittests\source_files_for_testing\d.cxx
# End Source File
# Begin Source File

SOURCE=unittests\source_files_for_testing\e.cxx
# End Source File
# Begin Source File

SOURCE=unittests\source_files_for_testing\j.cxx
# End Source File
# Begin Group "subdir Sources"

# Begin Source File

SOURCE=unittests\source_files_for_testing\subdir\c.cxx
# End Source File
# Begin Group "subsubdir Sources"

# Begin Source File

SOURCE=unittests\source_files_for_testing\subdir\subsubdir\i.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=unittests\source_files_for_testing\subdir\h.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=unittests\source_files_for_testing\g.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Header Files"

# Begin Group "unittests Headers"

# Begin Group "source_files_for_testing Headers"

# Begin Source File

SOURCE=unittests\source_files_for_testing\a.hxx
# End Source File
# Begin Group "subdir Headers"

# Begin Source File

SOURCE=unittests\source_files_for_testing\subdir\c.hxx
# End Source File
# Begin Group "subsubdir Headers"

# Begin Source File

SOURCE=unittests\source_files_for_testing\subdir\subsubdir\i.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=unittests\source_files_for_testing\subdir\h.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=unittests\source_files_for_testing\g.hxx
# End Source File
# Begin Source File

SOURCE=unittests\source_files_for_testing\f.hxx
# End Source File
# Begin Source File

SOURCE=unittests\source_files_for_testing\e.hxx
# End Source File
# Begin Source File

SOURCE=unittests\source_files_for_testing\j.hxx
# End Source File
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "fortesting - Win32 Release"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "fortesting.dsp"
	srcdeps.exe settings.cfg fortesting.dsp

# End Custom Build

!ELSEIF  "$(CFG)" == "fortesting - Win32 Debug"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "fortesting.dsp"
	srcdeps.exe settings.cfg fortesting.dsp

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
