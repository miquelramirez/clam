# Microsoft Developer Studio Project File - Name="Fl_Envelope_Example" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Fl_Envelope_Example - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Fl_Envelope_Example.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Fl_Envelope_Example.mak" CFG="Fl_Envelope_Example - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Fl_Envelope_Example - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Fl_Envelope_Example - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "CLAM_Skeleton_project"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Fl_Envelope_Example - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /FD /c /Zm1000 /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\fltk\include" /I "..\..\..\src\Errors" /I "..\..\..\src\Defines"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 /libpath:"..\..\..\..\fltk\lib" fltk_1_1.lib fltkforms_1_1d.lib fltkgl_1_1d.lib fltkimages_1_1d.lib comctl32.lib kernel32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib user32.lib gdi32.lib opengl32.lib glu32.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "Fl_Envelope_Example - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /D "_DEBUG" /FD /GZ /Zm1000 /c /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\fltk\include" /I "..\..\..\src\Errors" /I "..\..\..\src\Defines"
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /libpath:"..\..\..\..\fltk\lib" fltk_1_1d.lib fltkforms_1_1d.lib fltkgl_1_1d.lib fltkimages_1_1d.lib comctl32.lib kernel32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib user32.lib gdi32.lib opengl32.lib glu32.lib /nologo /subsystem:console /debug /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "Fl_Envelope_Example - Win32 Release"
# Name "Fl_Envelope_Example - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group "draft Sources"

# Begin Group "Fl_Envelope Sources"

# Begin Group "env Sources"

# Begin Source File

SOURCE=..\..\..\draft\Fl_Envelope\env\example.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\Fl_Envelope\env\Selection.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\Fl_Envelope\env\Fl_Envelope.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\Fl_Envelope\env\Fl_Envelope_Scroll.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Errors Sources"

# Begin Source File

SOURCE=..\..\..\src\Errors\Err.cxx
# End Source File
# End Group
# Begin Group "Defines Sources"

# Begin Source File

SOURCE=..\..\..\src\Defines\Assert.cxx
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# Begin Group "fltk Headers"

# Begin Group "include Headers"

# Begin Group "FL Headers"

# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Export.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Enumerations.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Widget.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Group.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Window.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Double_Window.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\fl_draw.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Valuator.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Slider.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Scrollbar.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Input_.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Input.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Output.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Light_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Check_Button.H
# End Source File
# End Group
# End Group
# End Group
# Begin Group "draft Headers"

# Begin Group "Fl_Envelope Headers"

# Begin Group "env Headers"

# Begin Source File

SOURCE=..\..\..\draft\Fl_Envelope\env\Selection.H
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\Fl_Envelope\env\Fl_Envelope.H
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\Fl_Envelope\env\Fl_Envelope_Scroll.H
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Errors Headers"

# Begin Source File

SOURCE=..\..\..\src\Errors\Err.hxx
# End Source File
# End Group
# Begin Group "Defines Headers"

# Begin Source File

SOURCE=..\..\..\src\Defines\Assert.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\CLAM_Math.hxx
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "Fl_Envelope_Example - Win32 Release"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "Fl_Envelope_Example.dsp"
	srcdeps.exe settings.cfg Fl_Envelope_Example.dsp

# End Custom Build

!ELSEIF  "$(CFG)" == "Fl_Envelope_Example - Win32 Debug"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "Fl_Envelope_Example.dsp"
	srcdeps.exe settings.cfg Fl_Envelope_Example.dsp

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
