# Microsoft Developer Studio Project File - Name="FormantTrackerGUI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=FormantTrackerGUI - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FormantTrackerGUI.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FormantTrackerGUI.mak" CFG="FormantTrackerGUI - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FormantTrackerGUI - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "FormantTrackerGUI - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "CLAM_Skeleton_project"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FormantTrackerGUI - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /FD /c /Zm1000 /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_DOUBLE" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\externals\CbLib" /I "..\..\..\..\fltk\include" /I "..\..\..\src\Visualization\Widget\FLTK" /I "..\..\..\src\Visualization\Base" /I "..\..\..\src\Tools\AudioFileIO" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Standard" /I "..\..\..\src\Processing\Base" /I "..\..\..\src\Processing\AudioFileIO" /I "..\..\..\src\Flow\Ports" /I "..\..\..\src\Flow\Nodes" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Errors" /I "..\..\..\src\Defines" /I "..\..\..\src\Data\BasicProcessing" /I "..\..\..\src\Data\Base" /I "..\..\..\src\Base"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 /libpath:"..\..\..\..\fltk\lib" /libpath:"..\..\..\..\pthreads\lib" fltk_1_1.lib fltkforms_1_1d.lib fltkgl_1_1d.lib fltkimages_1_1d.lib comctl32.lib kernel32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib user32.lib gdi32.lib opengl32.lib glu32.lib pthreadVC.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "FormantTrackerGUI - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /D "_DEBUG" /FD /GZ /Zm1000 /c /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_DOUBLE" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\externals\CbLib" /I "..\..\..\..\fltk\include" /I "..\..\..\src\Visualization\Widget\FLTK" /I "..\..\..\src\Visualization\Base" /I "..\..\..\src\Tools\AudioFileIO" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Standard" /I "..\..\..\src\Processing\Base" /I "..\..\..\src\Processing\AudioFileIO" /I "..\..\..\src\Flow\Ports" /I "..\..\..\src\Flow\Nodes" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Errors" /I "..\..\..\src\Defines" /I "..\..\..\src\Data\BasicProcessing" /I "..\..\..\src\Data\Base" /I "..\..\..\src\Base"
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /libpath:"..\..\..\..\fltk\lib" /libpath:"..\..\..\..\pthreads\lib" fltk_1_1d.lib fltkforms_1_1d.lib fltkgl_1_1d.lib fltkimages_1_1d.lib comctl32.lib kernel32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib user32.lib gdi32.lib opengl32.lib glu32.lib pthreadVC.lib /nologo /subsystem:console /debug /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "FormantTrackerGUI - Win32 Release"
# Name "FormantTrackerGUI - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group "draft Sources"

# Begin Group "FormantTracker Sources"

# Begin Group "GUI Sources"

# Begin Source File

SOURCE=..\..\..\draft\FormantTracker\GUI\mainGFT.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\draft\FormantTracker\GUI\FT_MainWindow.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Visualization Sources"

# Begin Group "Base Sources"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signal.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Connection.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slot.cxx
# End Source File
# End Group
# Begin Group "Widget Sources"

# Begin Group "FLTK Sources"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Menu_Builder.cxx
# End Source File
# End Group
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

SOURCE=..\..\..\src\Errors\ErrProcessingObj.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrDynamicType.cxx
# End Source File
# End Group
# Begin Group "Defines Sources"

# Begin Source File

SOURCE=..\..\..\src\Defines\Assert.cxx
# End Source File
# End Group
# Begin Group "Base Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\Base\DynamicType.cxx
# End Source File
# End Group
# Begin Group "Data Sources"

# Begin Group "BasicProcessing Sources"

# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Audio.cxx
# End Source File
# End Group
# End Group
# Begin Group "Flow Sources"

# Begin Group "Controls Sources"

# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\InControl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\OutControl.cxx
# End Source File
# End Group
# Begin Group "Ports Sources"

# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\Port.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\OutPort.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\InPort.cxx
# End Source File
# End Group
# Begin Group "Nodes Sources"

# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\ReadStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\SourceStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\WriteStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\Node.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\DelayStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\InplaceStreamRegion.cxx
# End Source File
# End Group
# End Group
# Begin Group "Processing Sources"

# Begin Group "Base Sources No. 2"

# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedInControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedOutControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedInPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedOutPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\Processing.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\ProcessingComposite.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\TopLevelProcessing.cxx
# End Source File
# End Group
# Begin Group "AudioFileIO Sources"

# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFile.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFileIn.cxx
# End Source File
# End Group
# End Group
# Begin Group "Standard Sources"

# Begin Source File

SOURCE=..\..\..\src\Standard\Enum.cxx
# End Source File
# End Group
# Begin Group "Tools Sources"

# Begin Group "AudioFileIO Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\SoundHeader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\SoundFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AIFFFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\WaveFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\extended.cxx
# End Source File
# End Group
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

SOURCE=..\..\..\..\fltk\include\FL\Fl.H
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

SOURCE=..\..\..\..\fltk\include\FL\fl_ask.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\fl_message.H
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

SOURCE=..\..\..\..\fltk\include\FL\fl_file_chooser.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\fltk\include\FL\Fl_Menu_Bar.H
# End Source File
# End Group
# End Group
# End Group
# Begin Group "externals Headers"

# Begin Group "CbLib Headers"

# Begin Source File

SOURCE=..\..\..\externals\CbLib\FunctorBase.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor0.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor1.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor3.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor4.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor0R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor1R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor2R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor3R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor4R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\CBLMacros.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\CBL.hxx
# End Source File
# End Group
# End Group
# Begin Group "Visualization Headers"

# Begin Group "Base Headers"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signal.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Connection.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slotv0.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ConnectionHandler.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv0ImplSerious.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv0.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slotv1.hxx
# End Source File
# End Group
# Begin Group "Widget Headers"

# Begin Group "FLTK Headers"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Widget\FLTK\Fl_Menu_Builder.hxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "draft Headers"

# Begin Group "FormantTracker Headers"

# Begin Group "GUI Headers"

# Begin Source File

SOURCE=..\..\..\draft\FormantTracker\GUI\FT_MainWindow.hxx
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

SOURCE=..\..\..\src\Errors\ErrProcessingObj.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrDynamicType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrSoundFileIO.hxx
# End Source File
# End Group
# Begin Group "Defines Headers"

# Begin Source File

SOURCE=..\..\..\src\Defines\Assert.hxx
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

SOURCE=..\..\..\src\Defines\mtgsstream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\OSDefines.hxx
# End Source File
# End Group
# Begin Group "Base Headers No. 1"

# Begin Source File

SOURCE=..\..\..\src\Base\DynamicTypeMacros.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Base\Component.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Base\DynamicType.hxx
# End Source File
# End Group
# Begin Group "Storage Headers"

# Begin Group "Base Headers No. 2"

# Begin Source File

SOURCE=..\..\..\src\Storage\Base\Storable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\Base\Storage.hxx
# End Source File
# End Group
# End Group
# Begin Group "Standard Headers"

# Begin Source File

SOURCE=..\..\..\src\Standard\Array.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Enum.hxx
# End Source File
# End Group
# Begin Group "Data Headers"

# Begin Group "Base Headers No. 3"

# Begin Source File

SOURCE=..\..\..\src\Data\Base\ProcessingDataConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\Base\ProcessingData.hxx
# End Source File
# End Group
# Begin Group "BasicProcessing Headers"

# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Audio.hxx
# End Source File
# End Group
# End Group
# Begin Group "Flow Headers"

# Begin Group "Controls Headers"

# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\ControlLinker.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\InControl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\OutControl.hxx
# End Source File
# End Group
# Begin Group "Ports Headers"

# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\Port.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\OutPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\InPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\AudioInPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\InPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\AudioOutPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\OutPortTmpl.hxx
# End Source File
# End Group
# Begin Group "Nodes Headers"

# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\ReadStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\SourceStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\WriteStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\Node.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\DelayStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\InplaceStreamRegion.hxx
# End Source File
# End Group
# End Group
# Begin Group "Processing Headers"

# Begin Group "Base Headers No. 4"

# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedInControls.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedOutControls.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedInPorts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedOutPorts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\ProcessingConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\Processing.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\ProcessingComposite.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\TopLevelProcessing.hxx
# End Source File
# End Group
# Begin Group "AudioFileIO Headers"

# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFile.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\AudioFileIO\AudioFileIn.hxx
# End Source File
# End Group
# End Group
# Begin Group "Tools Headers"

# Begin Group "AudioFileIO Headers No. 1"

# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\SoundHeader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\SoundFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\AIFFFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\WaveFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\AudioFileIO\extended.hxx
# End Source File
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "FormantTrackerGUI - Win32 Release"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "FormantTrackerGUI.dsp"
	srcdeps.exe settings.cfg FormantTrackerGUI.dsp

# End Custom Build

!ELSEIF  "$(CFG)" == "FormantTrackerGUI - Win32 Debug"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "FormantTrackerGUI.dsp"
	srcdeps.exe settings.cfg FormantTrackerGUI.dsp

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
