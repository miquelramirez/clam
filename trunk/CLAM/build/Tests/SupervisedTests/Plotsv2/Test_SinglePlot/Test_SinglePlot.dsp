# Microsoft Developer Studio Project File - Name="Test_SinglePlot" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Test_SinglePlot - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Test_SinglePlot.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Test_SinglePlot.mak" CFG="Test_SinglePlot - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Test_SinglePlot - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Test_SinglePlot - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "CLAM_Skeleton_project"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Test_SinglePlot - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /FD /c /Zm1000 /D "NDEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /I "..\..\..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\..\externals\CbLib" /I "..\..\..\..\..\..\fltk\include" /I "..\..\..\..\..\src\Base" /I "..\..\..\..\..\src\Defines" /I "..\..\..\..\..\src\Defines\Windows" /I "..\..\..\..\..\src\Errors" /I "..\..\..\..\..\src\Standard" /I "..\..\..\..\..\src\Storage\Base" /I "..\..\..\..\..\src\Visualization\Base" /I "..\..\..\..\..\src\Visualization\GL" /I "..\..\..\..\..\src\Visualization\Plot" /I "..\..\..\..\..\src\Visualization\Util" /I "..\..\..\..\..\src\Visualization\Widget" /I "..\..\..\..\..\src\Visualization\Widget\FLTK"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 /libpath:"..\..\..\..\..\..\fltk\lib" fltk_1_1.lib fltkforms_1_1d.lib fltkgl_1_1d.lib fltkimages_1_1d.lib comctl32.lib kernel32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib user32.lib gdi32.lib opengl32.lib glu32.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "Test_SinglePlot - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /D "_DEBUG" /FD /GZ /Zm1000 /c /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /I "..\..\..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\..\externals\CbLib" /I "..\..\..\..\..\..\fltk\include" /I "..\..\..\..\..\src\Base" /I "..\..\..\..\..\src\Defines" /I "..\..\..\..\..\src\Defines\Windows" /I "..\..\..\..\..\src\Errors" /I "..\..\..\..\..\src\Standard" /I "..\..\..\..\..\src\Storage\Base" /I "..\..\..\..\..\src\Visualization\Base" /I "..\..\..\..\..\src\Visualization\GL" /I "..\..\..\..\..\src\Visualization\Plot" /I "..\..\..\..\..\src\Visualization\Util" /I "..\..\..\..\..\src\Visualization\Widget" /I "..\..\..\..\..\src\Visualization\Widget\FLTK"
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /libpath:"..\..\..\..\..\..\fltk\lib" fltk_1_1d.lib fltkforms_1_1d.lib fltkgl_1_1d.lib fltkimages_1_1d.lib comctl32.lib kernel32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib user32.lib gdi32.lib opengl32.lib glu32.lib /nologo /subsystem:console /debug /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "Test_SinglePlot - Win32 Release"
# Name "Test_SinglePlot - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group "test Sources"

# Begin Group "Visualization Sources"

# Begin Source File

SOURCE=..\..\..\..\..\test\Visualization\test_singleplot.cxx
# End Source File
# End Group
# End Group
# Begin Group "Errors Sources"

# Begin Source File

SOURCE=..\..\..\..\..\src\Errors\Err.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Errors\ErrOutOfMemory.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Errors\ErrDynamicType.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Errors\ErrGUI.cxx
# End Source File
# End Group
# Begin Group "Defines Sources"

# Begin Source File

SOURCE=..\..\..\..\..\src\Defines\Assert.cxx
# End Source File
# End Group
# Begin Group "Base Sources"

# Begin Source File

SOURCE=..\..\..\..\..\src\Base\DynamicType.cxx
# End Source File
# End Group
# Begin Group "Standard Sources"

# Begin Source File

SOURCE=..\..\..\..\..\src\Standard\Enum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Standard\GlobalEnums.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Standard\Point.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Standard\BPF.cxx
# End Source File
# End Group
# Begin Group "Visualization Sources No. 1"

# Begin Group "Plot Sources"

# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Plot\PlotBase.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Plot\GenericPlot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Plot\Plot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Plot\SystemPlots.cxx
# End Source File
# End Group
# Begin Group "Base Sources No. 1"

# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\Presentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\Connection.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\Slot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\TooltipTracker2D.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\Signal.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\DrawingSurface2D.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\WidgetTKWrapper.cxx
# End Source File
# End Group
# Begin Group "Widget Sources"

# Begin Group "FLTK Sources"

# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTK\Fl_Plot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTK\Fl_SinglePlot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTK\Fl_Axis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTK\Fl_X_Axis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTK\Fl_Y_Axis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTK\Fl_ZoomSlider.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_2DSurface.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_Single_Display.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTKWrapper.cxx
# End Source File
# End Group
# Begin Group "GL Sources"

# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\GL\GLRenderingManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\GL\GLRenderer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\GL\GLStraightLineArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\GL\GLSampleDetailArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\GL\SequenceRenderingManager.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Header Files"

# Begin Group "Errors Headers"

# Begin Source File

SOURCE=..\..\..\..\..\src\Errors\Err.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Errors\ErrOutOfMemory.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Errors\ErrDynamicType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Errors\ErrGUI.hxx
# End Source File
# End Group
# Begin Group "Defines Headers"

# Begin Source File

SOURCE=..\..\..\..\..\src\Defines\Assert.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Defines\CLAM_Math.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Defines\DataTypes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Defines\ByteOrder.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Defines\EDataFormat.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Defines\StaticBool.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Defines\TypeInfo.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Defines\TypeInfoStd.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Defines\mtgsstream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Defines\OSDefines.hxx
# End Source File
# Begin Group "Windows Headers"

# Begin Source File

SOURCE=..\..\..\..\..\src\Defines\Windows\CLAM_windows.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\..\src\Defines\CLAMGL.hxx
# End Source File
# End Group
# Begin Group "Storage Headers"

# Begin Group "Base Headers"

# Begin Source File

SOURCE=..\..\..\..\..\src\Storage\Base\Storable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Storage\Base\Storage.hxx
# End Source File
# End Group
# End Group
# Begin Group "Base Headers No. 1"

# Begin Source File

SOURCE=..\..\..\..\..\src\Base\Component.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Base\DynamicTypeMacros.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Base\DynamicType.hxx
# End Source File
# End Group
# Begin Group "Standard Headers"

# Begin Source File

SOURCE=..\..\..\..\..\src\Standard\Array.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Standard\PointTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Standard\PointTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Standard\PointTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Standard\SearchArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Standard\Enum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Standard\GlobalEnums.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Standard\BPFTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Standard\Point.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Standard\BPF.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Standard\BPFTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Standard\BPFTmpl.hxx
# End Source File
# End Group
# Begin Group "Visualization Headers"

# Begin Group "Util Headers"

# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Util\Color.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Util\BoundingBoxes.hxx
# End Source File
# End Group
# Begin Group "Plot Headers"

# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Plot\PlotBase.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Plot\GenericPlot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Plot\Plot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Plot\SystemPlots.hxx
# End Source File
# End Group
# Begin Group "Base Headers No. 2"

# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\Presentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\Connection.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\Slot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\Slotv4.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\TooltipTracker2D.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\Slotv1.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\Slotv2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\Signal.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\DrawingSurface2D.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\ConnectionHandler.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\Signalv2ImplSerious.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\Signalv2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\WidgetTKWrapper.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Base\SupportedToolkits.hxx
# End Source File
# End Group
# Begin Group "Widget Headers"

# Begin Group "FLTK Headers"

# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTK\Fl_Plot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTK\Fl_SinglePlot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTK\Fl_Axis_Enumerations.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTK\Fl_Axis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTK\Fl_X_Axis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTK\Fl_Y_Axis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTK\Fl_ZoomSlider.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_2DSurface.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_Single_Display.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\Widget\FLTKWrapper.hxx
# End Source File
# End Group
# Begin Group "GL Headers"

# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\GL\GLRenderingManager.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\GL\GLRenderer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\GL\GLStraightLineArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\GL\GLSampleDetailArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\Visualization\GL\SequenceRenderingManager.hxx
# End Source File
# End Group
# End Group
# Begin Group "fltk Headers"

# Begin Group "include Headers"

# Begin Group "Fl Headers"

# Begin Source File

SOURCE=..\..\..\..\..\..\fltk\include\Fl\Fl_Export.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\..\fltk\include\Fl\Enumerations.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\..\fltk\include\Fl\Fl_Widget.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\..\fltk\include\Fl\Fl_Group.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\..\fltk\include\Fl\Fl_Box.H
# End Source File
# End Group
# Begin Group "FL Headers"

# Begin Source File

SOURCE=..\..\..\..\..\..\fltk\include\FL\Fl_Export.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\..\fltk\include\FL\Enumerations.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\..\fltk\include\FL\Fl.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\..\fltk\include\FL\Fl_Widget.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\..\fltk\include\FL\Fl_Group.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\..\fltk\include\FL\Fl_Window.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\..\fltk\include\FL\Fl_Box.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\..\fltk\include\FL\fl_draw.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\..\fltk\include\FL\Fl_Valuator.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\..\fltk\include\FL\Fl_Gl_Window.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\..\fltk\include\FL\Fl_Tooltip.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\..\fltk\include\FL\gl.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "externals Headers"

# Begin Group "CbLib Headers"

# Begin Source File

SOURCE=..\..\..\..\..\externals\CbLib\FunctorBase.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\externals\CbLib\Functor0.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\externals\CbLib\Functor1.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\externals\CbLib\Functor2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\externals\CbLib\Functor3.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\externals\CbLib\Functor4.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\externals\CbLib\Functor0R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\externals\CbLib\Functor1R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\externals\CbLib\Functor2R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\externals\CbLib\Functor3R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\externals\CbLib\Functor4R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\externals\CbLib\CBLMacros.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\externals\CbLib\CBL.hxx
# End Source File
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "Test_SinglePlot - Win32 Release"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "Test_SinglePlot.dsp"
	srcdeps.exe settings.cfg Test_SinglePlot.dsp

# End Custom Build

!ELSEIF  "$(CFG)" == "Test_SinglePlot - Win32 Debug"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "Test_SinglePlot.dsp"
	srcdeps.exe settings.cfg Test_SinglePlot.dsp

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
