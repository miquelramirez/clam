# Microsoft Developer Studio Project File - Name="testsettings" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=testsettings - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "testsettings.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "testsettings.mak" CFG="testsettings - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "testsettings - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "testsettings - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "empty"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "testsettings - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../ReleaseFiles/bin"
# PROP Intermediate_Dir "../ReleaseFiles/obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /FD /c /Zm1000 /D "NDEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_DOUBLE" /I "..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\build\srcdeps\unittests\source_files_for_testing\subdir"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "testsettings - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../DebugFiles/bin"
# PROP Intermediate_Dir "../DebugFiles/obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /D "_DEBUG" /FD /GZ /Zm1000 /c /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_DOUBLE" /I "..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\build\srcdeps\unittests\source_files_for_testing\subdir"
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 /nologo /subsystem:console /debug /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "testsettings - Win32 Release"
# Name "testsettings - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group "build Sources"

# Begin Group "srcdeps Sources"

# Begin Group "unittests Sources"

# Begin Group "source_files_for_testing Sources"

# Begin Source File

SOURCE=..\..\build\srcdeps\unittests\source_files_for_testing\a.cxx
# End Source File
# Begin Source File

SOURCE=..\..\build\srcdeps\unittests\source_files_for_testing\b.cxx
# End Source File
# Begin Source File

SOURCE=..\..\build\srcdeps\unittests\source_files_for_testing\d.cxx
# End Source File
# Begin Group "subdir Sources"

# Begin Source File

SOURCE=..\..\build\srcdeps\unittests\source_files_for_testing\subdir\c.cxx
# End Source File
# End Group
# End Group
# End Group
# End Group
# End Group
# End Group
# Begin Group "Header Files"

# Begin Group "build Headers"

# Begin Group "srcdeps Headers"

# Begin Group "unittests Headers"

# Begin Group "source_files_for_testing Headers"

# Begin Source File

SOURCE=..\..\build\srcdeps\unittests\source_files_for_testing\a.hxx
# End Source File
# Begin Source File

SOURCE=..\..\build\srcdeps\unittests\source_files_for_testing\b.hxx
# End Source File
# Begin Source File

SOURCE=..\..\build\srcdeps\unittests\source_files_for_testing\with_no_impl.hxx
# End Source File
# Begin Group "subdir Headers"

# Begin Source File

SOURCE=..\..\build\srcdeps\unittests\source_files_for_testing\subdir\c.hxx
# End Source File
# End Group
# End Group
# End Group
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "testsettings - Win32 Release"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "testsettings.dsp"
	srcdeps.exe settings.cfg testsettings.dsp

# End Custom Build

!ELSEIF  "$(CFG)" == "testsettings - Win32 Debug"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "testsettings.dsp"
	srcdeps.exe settings.cfg testsettings.dsp

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
