# Microsoft Developer Studio Project File - Name="DownSampling" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=DownSampling - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DownSampling.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DownSampling.mak" CFG="DownSampling - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DownSampling - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "DownSampling - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DownSampling - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /MD /W3 /GR /GX /Ob2 /I "../../src/Base" /I "../../src/Data" /I "../../src/Data/Base" /I "../../src/Data/BasicProcessing" /I "../../src/Data/Descriptors" /I "../../src/Defines" /I "../../src/Defines/Windows" /I "../../src/Errors" /I "../../src/Flow" /I "../../src/Flow/Controls" /I "../../src/Flow/Nodes" /I "../../src/Flow/Ports" /I "../../src/Processing" /I "../../src/Processing/Analysis" /I "../../src/Processing/ArithOps" /I "../../src/Processing/AudioFileIO" /I "../../src/Processing/AudioIO" /I "../../src/Processing/Base" /I "../../src/Processing/Controls" /I "../../src/Processing/Generators" /I "../../src/Processing/MIDIIO" /I "../../src/Processing/Synthesis" /I "../../src/Processing/Transformations" /I "../../src/Standard" /I "../../src/Storage" /I "../../src/Storage/Base" /I "../../src/Storage/XML" /I "../../src/System/Application" /I "../../src/System/Threads" /I "../../src/Tools" /I "../../src/Tools/AudioFileIO" /I "../../src/Tools/AudioIO" /I "../../src/Tools/MIDIIO" /I "../../src/Visualization" /I "../../src/Visualization/Base" /I "../../src/Visualization/GL" /I "../../src/Visualization/Presentation" /I "../../src/Visualization/Services" /I "../../src/Visualization/SysInt" /I "../../src/Visualization/Util" /I "../../src/Visualization/View" /I "../../src/Visualization/Widget" /I "../../externals/CbLib" /I "../../../fftw/include" /I "../../../dxsdk/include" /I "../../../" /I "../../../xercesc/include" /I "../../../fltk/include" /I "../../../portmidi" /I "../../../include/pthreads" /FI"config.h" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "CLAM_USE_XML" /Fo"..\ReleaseFiles\obj/" /Fd"..\ReleaseFiles\obj/" /FD /GZ /c
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib xerces-c_1D.lib rfftw2st.lib fftw2st.lib fltkd.lib wsock32.lib opengl32.lib glu32.lib dsound.lib dxerr8.lib dxguid.lib pthreadVCE.lib /subsystem:console /pdb:"..\ReleaseFiles\bin/CLAM_Skeleton_project.pdb" /machine:I386 /out:"..\ReleaseFiles\bin/CLAM_Skeleton_project.exe" /libpath:"../../../xercesc/lib" /libpath:"../../../fftw/lib" /libpath:"../../../fltk/lib" /libpath:"../../../dxsdk/lib" /libpath:"../../../pthreads/lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "DownSampling - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /MDd /W3 /GR /GX /ZI /Od /I "../../src/Base" /I "../../src/Data" /I "../../src/Data/Base" /I "../../src/Data/BasicProcessing" /I "../../src/Data/Descriptors" /I "../../src/Defines" /I "../../src/Defines/Windows" /I "../../src/Errors" /I "../../src/Flow" /I "../../src/Flow/Controls" /I "../../src/Flow/Nodes" /I "../../src/Flow/Ports" /I "../../src/Processing" /I "../../src/Processing/Analysis" /I "../../src/Processing/ArithOps" /I "../../src/Processing/AudioFileIO" /I "../../src/Processing/AudioIO" /I "../../src/Processing/Base" /I "../../src/Processing/Controls" /I "../../src/Processing/Generators" /I "../../src/Processing/MIDIIO" /I "../../src/Processing/Synthesis" /I "../../src/Processing/Transformations" /I "../../src/Standard" /I "../../src/Storage" /I "../../src/Storage/Base" /I "../../src/Storage/XML" /I "../../src/System/Application" /I "../../src/System/Threads" /I "../../src/Tools" /I "../../src/Tools/AudioFileIO" /I "../../src/Tools/AudioIO" /I "../../src/Tools/MIDIIO" /I "../../src/Visualization" /I "../../src/Visualization/Base" /I "../../src/Visualization/GL" /I "../../src/Visualization/Presentation" /I "../../src/Visualization/Services" /I "../../src/Visualization/SysInt" /I "../../src/Visualization/Util" /I "../../src/Visualization/View" /I "../../src/Visualization/Widget" /I "../../externals/CbLib" /I "../../../fftw/include" /I "../../../dxsdk/include" /I "../../../" /I "../../../xercesc/include" /I "../../../fltk/include" /I "../../../portmidi" /I "../../../include/pthreads" /I "../TDFilterGenerator" /I "../../../fltk" /I "../../src/Visualization/Presentation/Base" /I "../../src/Visualization/Presentation/FLTK" /I "../../src/Visualization/Widget/FLTK" /I "../../../pthreads/include" /I "../../src/Visualization/Plot" /FI"config.h" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "CLAM_USE_XML" /FR /Fo"..\DebugFiles\obj/" /Fd"..\DebugFiles\obj/" /FD /GZ /c
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib xerces-c_1D.lib rfftw2st.lib fftw2st.lib fltkd.lib wsock32.lib opengl32.lib glu32.lib dsound.lib dxerr8.lib dxguid.lib pthreadVCE.lib /subsystem:console /incremental:no /pdb:"..\DebugFiles\bin/CLAM_Skeleton_project.pdb" /debug /machine:I386 /out:"..\DebugFiles\bin/CLAM_Skeleton_project.exe" /libpath:"../../../xercesc/lib" /libpath:"../../../fftw/lib" /libpath:"../../../fltk/lib" /libpath:"../../../dxsdk/lib" /libpath:"../../../pthreads/lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "DownSampling - Win32 Release"
# Name "DownSampling - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "CLAM Souces"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Tools\AudioFileIO\AIFFFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Defines\Assert.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Data\BasicProcessing\Audio.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tools\AudioIO\AudioDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\AudioFileIO\AudioFile.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\AudioFileIO\AudioFileIn.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\AudioFileIO\AudioFileOut.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Storage\XML\BasicXMLable.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Standard\Complex.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Base\DynamicType.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Errors\Err.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Errors\ErrDynamicType.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Errors\ErrOutOfMemory.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Errors\ErrProcessingObj.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Errors\ErrSystem.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tools\AudioFileIO\extended.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Flow\Ports\InPort.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\System\Threads\Mutex.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Flow\Controls\OutControl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Flow\Ports\OutPort.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Standard\Polar.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Flow\Ports\Port.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\Processing.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\ProcessingComposite.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\PublishedInControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\PublishedInPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\PublishedOutControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\PublishedOutPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tools\AudioFileIO\SoundFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tools\AudioFileIO\SoundHeader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\TopLevelProcessing.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Tools\AudioFileIO\WaveFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Storage\XML\XMLStorage.cxx
# End Source File
# End Group
# Begin Group "DownSampling Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DownSampling.cxx
# End Source File
# End Group
# Begin Group "TDFilter Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\TDFilterGenerator\Polynomial.cxx
# End Source File
# Begin Source File

SOURCE=..\TDFilterGenerator\PolynomialRootsFinder.cxx
# End Source File
# Begin Source File

SOURCE=..\TDFilterGenerator\TDFilter.cxx
# End Source File
# Begin Source File

SOURCE=..\TDFilterGenerator\TDFilterGen.cxx
# End Source File
# Begin Source File

SOURCE=..\TDFilterGenerator\TDFilterKernel.cxx
# End Source File
# End Group
# Begin Group "VM Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Visualization\View\AudioAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\View\AudioModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Presentation\Base\AudioPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\AudioRenderingManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Base\Connection.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Base\DrawingSurface2D.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Presentation\FLTK\Fl_Audio.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTK\Fl_Axis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTK\Fl_Gl_2DSurface.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTK\Fl_Gl_Multi_Display.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTK\Fl_Gl_Single_Display.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTK\Fl_GridLayout.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTK\Fl_Palette.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTK\Fl_X_Axis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTK\Fl_Y_Axis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTK\Fl_ZoomSlider.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Widget\FLTKWrapper.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\GLArrayRenderer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\GLMaxMinArrays.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\GLPointsArrays.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\GLRenderer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\GLRenderingManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\GLSampleDetailArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\GLStraightLineArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\GL\HugeArrayRenderer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Base\ModelAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Plot\PlotAudio.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Base\Presentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Base\Signal.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Base\Slot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Base\WidgetTKWrapper.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=.\DownSampling_example.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Storage\XML\XercesDomPrinter.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "CLAM Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Tools\AudioIO\AudioManager.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Errors\ErrSystem.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\System\Threads\Lock.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\System\Threads\Mutex.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Visualization\Plot\Plot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Processing\Base\Processing.hxx
# End Source File
# End Group
# Begin Group "DownSampling Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DownSampling.hxx
# End Source File
# End Group
# Begin Group "TDFilter Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\TDFilterGenerator\Polynomial.hxx
# End Source File
# Begin Source File

SOURCE=..\TDFilterGenerator\PolynomialRootsFinder.hxx
# End Source File
# Begin Source File

SOURCE=..\TDFilterGenerator\TDFilter.hxx
# End Source File
# Begin Source File

SOURCE=..\TDFilterGenerator\TDFilterGen.hxx
# End Source File
# Begin Source File

SOURCE=..\TDFilterGenerator\TDFilterKernel.hxx
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
