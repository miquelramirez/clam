# Microsoft Developer Studio Project File - Name="SaltoExample" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SaltoExample - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SaltoExample.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SaltoExample.mak" CFG="SaltoExample - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SaltoExample - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "SaltoExample - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "CLAM_Skeleton_project"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SaltoExample - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /FD /c /Zm1000 /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\fltk\include" /I "..\..\..\src\Base" /I "..\..\..\src\Defines" /I "..\..\..\src\Errors" /I "..\..\..\src\Standard" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Tools\SDIF"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 /libpath:"..\..\..\..\fltk\lib" fltk_1_1.lib fltkforms_1_1d.lib fltkgl_1_1d.lib fltkimages_1_1d.lib comctl32.lib kernel32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib user32.lib gdi32.lib opengl32.lib glu32.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "SaltoExample - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /D "_DEBUG" /FD /GZ /Zm1000 /c /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\fltk\include" /I "..\..\..\src\Base" /I "..\..\..\src\Defines" /I "..\..\..\src\Errors" /I "..\..\..\src\Standard" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Tools\SDIF"
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

# Name "SaltoExample - Win32 Release"
# Name "SaltoExample - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group "examples Sources"

# Begin Group "SDIFDisplay Sources"

# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\SDIFDisplayMain.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\SDIFDisplay.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\ZoomSlider.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\DragValue.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\SDIFDisplayContainer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\SDIFDisplayAxis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\SDIFDisplayTRC.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\SDIFDisplaySTF.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\SDIFDisplayFQ0.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\SDIFDisplayColors.cxx
# End Source File
# End Group
# End Group
# Begin Group "Errors Sources"

# Begin Source File

SOURCE=..\..\..\src\Errors\Err.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrOutOfMemory.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrDynamicType.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrOpenFile.cxx
# End Source File
# End Group
# Begin Group "Defines Sources"

# Begin Source File

SOURCE=..\..\..\src\Defines\Assert.cxx
# End Source File
# End Group
# Begin Group "Tools Sources"

# Begin Group "SDIF Sources"

# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFType.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFHeader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFMatrix.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFFrame.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFStream.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFFile.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFCollection.cxx
# End Source File
# End Group
# End Group
# Begin Group "Base Sources"

# Begin Source File

SOURCE=..\..\..\src\Base\DynamicType.cxx
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# Begin Group "fltk Headers"

# Begin Group "include Headers"

# Begin Group "Fl Headers"

# Begin Source File

SOURCE=..\..\..\..\fltk\include\Fl\Fl_Export.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\Fl\Enumerations.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\Fl\Fl_Widget.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\Fl\Fl_Group.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\Fl\Fl_Window.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\Fl\Fl_Double_Window.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\Fl\Fl_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\Fl\Fl_Return_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\Fl\Fl_Box.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\Fl\Fl_Tile.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\Fl\Fl.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\Fl\fl_file_chooser.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\Fl\filename.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\Fl\fl_ask.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\Fl\Fl_Slider.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\Fl\Fl_Valuator.H
# End Source File
# End Group
# Begin Group "FL Headers"

# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Window.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Group.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Widget.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Enumerations.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Image.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Menu_Item.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Menu_.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Choice.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Menu_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Export.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Preferences.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Tile.H
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

SOURCE=..\..\..\..\fltk\include\FL\Fl_Browser_.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Browser.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_File_Icon.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\filename.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\dirent.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_File_Browser.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Box.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Light_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Check_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Input_.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Input.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_File_Input.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Return_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\fl_ask.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\fl_draw.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Pixmap.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Errors Headers"

# Begin Source File

SOURCE=..\..\..\src\Errors\Err.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrOutOfMemory.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrDynamicType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrOpenFile.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrFormat.hxx
# End Source File
# End Group
# Begin Group "Defines Headers"

# Begin Source File

SOURCE=..\..\..\src\Defines\Assert.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\CLAM_Math.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\DataTypes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\ByteOrder.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\EDataFormat.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\StaticBool.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\TypeInfo.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\TypeInfoStd.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\OSDefines.hxx
# End Source File
# End Group
# Begin Group "Tools Headers"

# Begin Group "SDIF Headers"

# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFHeader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFMatrix.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFFrame.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFStream.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFStorage.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFFile.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\SDIF\SDIFCollection.hxx
# End Source File
# End Group
# End Group
# Begin Group "Storage Headers"

# Begin Group "Base Headers"

# Begin Source File

SOURCE=..\..\..\src\Storage\Base\Storable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\Base\Storage.hxx
# End Source File
# End Group
# End Group
# Begin Group "Base Headers No. 1"

# Begin Source File

SOURCE=..\..\..\src\Base\Component.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Base\DynamicTypeMacros.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Base\DynamicType.hxx
# End Source File
# End Group
# Begin Group "Standard Headers"

# Begin Source File

SOURCE=..\..\..\src\Standard\Array.hxx
# End Source File
# End Group
# Begin Group "examples Headers"

# Begin Group "SDIFDisplay Headers"

# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\SDIFDisplay.H
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\ZoomSlider.H
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\DragValue.H
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\SDIFDisplayContainer.H
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\mag.xpm
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\SDIFDisplayAxis.H
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\SDIFDisplayTRC.H
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\SDIFDisplaySTF.H
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\SDIFDisplayFQ0.H
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\SDIFDisplay\SDIFDisplayColors.H
# End Source File
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "SaltoExample - Win32 Release"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "SaltoExample.dsp"
	srcdeps.exe settings.cfg SaltoExample.dsp

# End Custom Build

!ELSEIF  "$(CFG)" == "SaltoExample - Win32 Debug"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "SaltoExample.dsp"
	srcdeps.exe settings.cfg SaltoExample.dsp

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
