# Microsoft Developer Studio Project File - Name="ThreadedProcessing" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ThreadedProcessing - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ThreadedProcessing.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ThreadedProcessing.mak" CFG="ThreadedProcessing - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ThreadedProcessing - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ThreadedProcessing - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "CLAM_Skeleton_project"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ThreadedProcessing - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /FD /c /Zm1000 /D "NDEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /I "..\..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\externals\CbLib" /I "..\..\..\..\..\fltk\include" /I "..\..\..\..\..\pthreads\include" /I "..\..\..\..\draft\DownSampling" /I "..\..\..\..\src\Base" /I "..\..\..\..\src\Data\Base" /I "..\..\..\..\src\Data\BasicProcessing" /I "..\..\..\..\src\Defines" /I "..\..\..\..\src\Defines\Windows" /I "..\..\..\..\src\Errors" /I "..\..\..\..\src\Flow\Controls" /I "..\..\..\..\src\Flow\Nodes" /I "..\..\..\..\src\Flow\Ports" /I "..\..\..\..\src\Processing\Analysis" /I "..\..\..\..\src\Processing\ArithOps" /I "..\..\..\..\src\Processing\AudioFileIO" /I "..\..\..\..\src\Processing\Base" /I "..\..\..\..\src\Standard" /I "..\..\..\..\src\Storage\Base" /I "..\..\..\..\src\Storage\XML" /I "..\..\..\..\src\System\Threads" /I "..\..\..\..\src\Tools\AudioFileIO" /I "..\..\..\..\src\Visualization\Base" /I "..\..\..\..\src\Visualization\GL" /I "..\..\..\..\src\Visualization\Plot" /I "..\..\..\..\src\Visualization\Util" /I "..\..\..\..\src\Visualization\Widget" /I "..\..\..\..\src\Visualization\Widget\FLTK" /I "..\..\..\..\..\xercesc\include"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 /libpath:"..\..\..\..\..\fftw\lib" /libpath:"..\..\..\..\..\fltk\lib" /libpath:"..\..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\..\pthreads\lib" RFFTW2st.lib FFTW2st.lib fltk_1_1.lib fltkforms_1_1d.lib fltkgl_1_1d.lib fltkimages_1_1d.lib comctl32.lib kernel32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib user32.lib gdi32.lib opengl32.lib glu32.lib xerces-c_1.lib pthreadVC.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "ThreadedProcessing - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /D "_DEBUG" /FD /GZ /Zm1000 /c /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /I "..\..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\externals\CbLib" /I "..\..\..\..\..\fltk\include" /I "..\..\..\..\..\pthreads\include" /I "..\..\..\..\draft\DownSampling" /I "..\..\..\..\src\Base" /I "..\..\..\..\src\Data\Base" /I "..\..\..\..\src\Data\BasicProcessing" /I "..\..\..\..\src\Defines" /I "..\..\..\..\src\Defines\Windows" /I "..\..\..\..\src\Errors" /I "..\..\..\..\src\Flow\Controls" /I "..\..\..\..\src\Flow\Nodes" /I "..\..\..\..\src\Flow\Ports" /I "..\..\..\..\src\Processing\Analysis" /I "..\..\..\..\src\Processing\ArithOps" /I "..\..\..\..\src\Processing\AudioFileIO" /I "..\..\..\..\src\Processing\Base" /I "..\..\..\..\src\Standard" /I "..\..\..\..\src\Storage\Base" /I "..\..\..\..\src\Storage\XML" /I "..\..\..\..\src\System\Threads" /I "..\..\..\..\src\Tools\AudioFileIO" /I "..\..\..\..\src\Visualization\Base" /I "..\..\..\..\src\Visualization\GL" /I "..\..\..\..\src\Visualization\Plot" /I "..\..\..\..\src\Visualization\Util" /I "..\..\..\..\src\Visualization\Widget" /I "..\..\..\..\src\Visualization\Widget\FLTK" /I "..\..\..\..\..\xercesc\include"
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /libpath:"..\..\..\..\..\fftw\lib" /libpath:"..\..\..\..\..\fltk\lib" /libpath:"..\..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\..\pthreads\lib" FFTW2std.lib RFFTW2std.lib fltk_1_1d.lib fltkforms_1_1d.lib fltkgl_1_1d.lib fltkimages_1_1d.lib comctl32.lib kernel32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib wsock32.lib user32.lib gdi32.lib opengl32.lib glu32.lib xerces-c_1d.lib pthreadVC.lib /nologo /subsystem:console /debug /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "ThreadedProcessing - Win32 Release"
# Name "ThreadedProcessing - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group "examples Sources"

# Begin Source File

SOURCE=..\..\..\..\examples\ThreadedProcessing_example.cxx
# End Source File
# End Group
# Begin Group "Errors Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Errors\Err.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrOutOfMemory.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrProcessingObj.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrSystem.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrDynamicType.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrGUI.cxx
# End Source File
# End Group
# Begin Group "Defines Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Defines\Assert.cxx
# End Source File
# End Group
# Begin Group "Storage Sources"

# Begin Group "XML Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\BasicXMLable.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLComponentAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLStorage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLStaticAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomPrinter.cxx
# End Source File
# End Group
# End Group
# Begin Group "Base Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Base\DynamicType.cxx
# End Source File
# End Group
# Begin Group "Data Sources"

# Begin Group "BasicProcessing Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Audio.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\LPModel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Spectrum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\SpectrumConversions.cxx
# End Source File
# End Group
# End Group
# Begin Group "Flow Sources"

# Begin Group "Controls Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\InControl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\OutControl.cxx
# End Source File
# End Group
# Begin Group "Ports Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\Port.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\OutPort.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\InPort.cxx
# End Source File
# End Group
# Begin Group "Nodes Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\StreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\ReadStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\SourceStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\WriteStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\Node.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\DelayStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\InplaceStreamRegion.cxx
# End Source File
# End Group
# End Group
# Begin Group "Processing Sources"

# Begin Group "Base Sources No. 1"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedInControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedOutControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedInPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedOutPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\Processing.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\ProcessingComposite.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\TopLevelProcessing.cxx
# End Source File
# End Group
# Begin Group "AudioFileIO Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\AudioFile.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\AudioFileIn.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\AudioFileOut.cxx
# End Source File
# End Group
# Begin Group "Analysis Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\LPC_AutoCorrelation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\WindowGenerator.cxx
# End Source File
# End Group
# Begin Group "ArithOps Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\AudioMultiplier.cxx
# End Source File
# End Group
# End Group
# Begin Group "Standard Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Standard\Enum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Complex.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Polar.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\GlobalEnums.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Point.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPF.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Flags.cxx
# End Source File
# End Group
# Begin Group "draft Sources"

# Begin Group "DownSampling Sources"

# Begin Group "TDFilterGenerator Sources"

# Begin Source File

SOURCE=..\..\..\..\draft\DownSampling\..\TDFilterGenerator\PolynomialRootsFinder.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\draft\DownSampling\..\TDFilterGenerator\Polynomial.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\draft\DownSampling\..\TDFilterGenerator\TDFilterKernel.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\draft\DownSampling\..\TDFilterGenerator\TDFilter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\draft\DownSampling\..\TDFilterGenerator\TDFilterGen.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\draft\DownSampling\DownSampling.cxx
# End Source File
# End Group
# End Group
# Begin Group "Visualization Sources"

# Begin Group "Plot Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Plot\PlotBase.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Plot\GenericPlot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Plot\Plot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Plot\SystemPlots.cxx
# End Source File
# End Group
# Begin Group "Base Sources No. 2"

# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Presentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Connection.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Slot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\TooltipTracker2D.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Signal.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\DrawingSurface2D.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\WidgetTKWrapper.cxx
# End Source File
# End Group
# Begin Group "Widget Sources"

# Begin Group "FLTK Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Plot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_SinglePlot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Axis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_X_Axis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Y_Axis.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_ZoomSlider.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_2DSurface.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_Single_Display.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTKWrapper.cxx
# End Source File
# End Group
# Begin Group "GL Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\GLRenderingManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\GLRenderer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\GLStraightLineArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\GLSampleDetailArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\SequenceRenderingManager.cxx
# End Source File
# End Group
# End Group
# Begin Group "System Sources"

# Begin Group "Threads Sources"

# Begin Source File

SOURCE=..\..\..\..\src\System\Threads\Condition.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Threads\Thread.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Threads\Mutex.cxx
# End Source File
# End Group
# End Group
# Begin Group "Tools Sources"

# Begin Group "AudioFileIO Sources No. 1"

# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\SoundHeader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\SoundFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AIFFFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\WaveFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\extended.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Header Files"

# Begin Group "Storage Headers"

# Begin Group "Base Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Storage\Base\Storable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\Base\Storage.hxx
# End Source File
# End Group
# Begin Group "XML Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\BasicXMLable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLComponentAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLIterableAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLStorage.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLArrayAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLStaticAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomPrinter.hxx
# End Source File
# End Group
# End Group
# Begin Group "Errors Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Errors\Err.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrOutOfMemory.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrProcessingObj.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrSystem.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrDynamicType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrSoundFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrGUI.hxx
# End Source File
# End Group
# Begin Group "Defines Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Defines\Assert.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\mtgsstream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\StaticBool.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\TypeInfo.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\TypeInfoStd.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\CLAM_Math.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\DataTypes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\ByteOrder.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\EDataFormat.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\OSDefines.hxx
# End Source File
# Begin Group "Windows Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Defines\Windows\CLAM_windows.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\Defines\CLAMGL.hxx
# End Source File
# End Group
# Begin Group "Base Headers No. 1"

# Begin Source File

SOURCE=..\..\..\..\src\Base\Component.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Base\DynamicTypeMacros.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Base\DynamicType.hxx
# End Source File
# End Group
# Begin Group "Standard Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Standard\Array.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Enum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\ComplexTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Complex.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PolarTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Polar.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PointTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PointTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PointTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\SearchArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\GlobalEnums.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPFTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Point.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPF.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Flags.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\CommonOps.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\ComplexTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\ComplexTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PolarTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PolarTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPFTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPFTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\ArrayToBPFCnv.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\ComplexToPolarCnv.hxx
# End Source File
# End Group
# Begin Group "Data Headers"

# Begin Group "Base Headers No. 2"

# Begin Source File

SOURCE=..\..\..\..\src\Data\Base\ProcessingDataConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Base\ProcessingData.hxx
# End Source File
# End Group
# Begin Group "BasicProcessing Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Audio.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\LPModel.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Spectrum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\SpecTypeFlags.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\SpectrumConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\SpectrumConversions.hxx
# End Source File
# End Group
# End Group
# Begin Group "Flow Headers"

# Begin Group "Controls Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\ControlLinker.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\InControl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\OutControl.hxx
# End Source File
# End Group
# Begin Group "Ports Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\Port.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\OutPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\InPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\AudioInPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\InPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\AudioOutPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\OutPortTmpl.hxx
# End Source File
# End Group
# Begin Group "Nodes Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\StreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\ReadStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\SourceStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\WriteStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\Node.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\DelayStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\InplaceStreamRegion.hxx
# End Source File
# End Group
# End Group
# Begin Group "Processing Headers"

# Begin Group "Base Headers No. 3"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedInControls.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedOutControls.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedInPorts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedOutPorts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\ProcessingConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\Processing.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\ProcessingComposite.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\TopLevelProcessing.hxx
# End Source File
# End Group
# Begin Group "AudioFileIO Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\AudioFile.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\AudioFileIn.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\AudioFileOut.hxx
# End Source File
# End Group
# Begin Group "Analysis Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\LPC_AutoCorrelation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\WindowGeneratorConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Analysis\WindowGenerator.hxx
# End Source File
# End Group
# Begin Group "ArithOps Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\BinaryAudioOp.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\AudioMultiplier.hxx
# End Source File
# End Group
# End Group
# Begin Group "draft Headers"

# Begin Group "DownSampling Headers"

# Begin Group "TDFilterGenerator Headers"

# Begin Source File

SOURCE=..\..\..\..\draft\DownSampling\..\TDFilterGenerator\PolynomialRootsFinder.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\draft\DownSampling\..\TDFilterGenerator\Polynomial.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\draft\DownSampling\..\TDFilterGenerator\TDFilterKernel.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\draft\DownSampling\..\TDFilterGenerator\TDFilter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\draft\DownSampling\..\TDFilterGenerator\TDFilterGen.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\draft\DownSampling\DownSampling.hxx
# End Source File
# End Group
# End Group
# Begin Group "Visualization Headers"

# Begin Group "Util Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Util\Color.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Util\BoundingBoxes.hxx
# End Source File
# End Group
# Begin Group "Plot Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Plot\PlotBase.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Plot\GenericPlot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Plot\Plot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Plot\SystemPlots.hxx
# End Source File
# End Group
# Begin Group "Base Headers No. 4"

# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Presentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Connection.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Slot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Slotv4.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\TooltipTracker2D.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Slotv1.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Slotv2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Signal.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\DrawingSurface2D.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\ConnectionHandler.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Signalv2ImplSerious.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\Signalv2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\WidgetTKWrapper.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Base\SupportedToolkits.hxx
# End Source File
# End Group
# Begin Group "Widget Headers"

# Begin Group "FLTK Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Plot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_SinglePlot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Axis_Enumerations.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Axis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_X_Axis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Y_Axis.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_ZoomSlider.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_2DSurface.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTK\Fl_Gl_Single_Display.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\Widget\FLTKWrapper.hxx
# End Source File
# End Group
# Begin Group "GL Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\GLRenderingManager.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\GLRenderer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\GLStraightLineArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\GLSampleDetailArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Visualization\GL\SequenceRenderingManager.hxx
# End Source File
# End Group
# End Group
# Begin Group "pthreads Headers"

# Begin Group "include Headers"

# Begin Source File

SOURCE=..\..\..\..\..\pthreads\include\pthread.h
# End Source File
# End Group
# End Group
# Begin Group "externals Headers"

# Begin Group "CbLib Headers"

# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\FunctorBase.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor0.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor1.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor3.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor4.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor0R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor1R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor2R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor3R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\Functor4R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\CBLMacros.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\externals\CbLib\CBL.hxx
# End Source File
# End Group
# End Group
# Begin Group "System Headers"

# Begin Group "Threads Headers"

# Begin Source File

SOURCE=..\..\..\..\src\System\Threads\xtime.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Threads\Lock.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Threads\Condition.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Threads\Thread.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\System\Threads\Mutex.hxx
# End Source File
# End Group
# End Group
# Begin Group "fltk Headers"

# Begin Group "include Headers No. 1"

# Begin Group "FL Headers"

# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Export.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Enumerations.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Widget.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Group.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Window.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Image.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Menu_Item.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Menu_.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Choice.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Menu_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Preferences.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Tile.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Valuator.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Slider.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Scrollbar.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Browser_.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Browser.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_File_Icon.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\filename.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\dirent.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_File_Browser.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Box.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Light_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Check_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Input_.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Input.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_File_Input.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Return_Button.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\fl_ask.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\fl_file_chooser.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\fl_draw.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Gl_Window.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\Fl_Tooltip.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\FL\gl.h
# End Source File
# End Group
# Begin Group "Fl Headers"

# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\Fl\Fl_Export.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\Fl\Enumerations.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\Fl\Fl_Widget.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\Fl\Fl_Group.H
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\fltk\include\Fl\Fl_Box.H
# End Source File
# End Group
# End Group
# End Group
# Begin Group "xercesc Headers"

# Begin Group "include Headers No. 2"

# Begin Group "xercesc Headers No. 1"

# Begin Group "util Headers"

# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XercesDefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\PlatformUtils.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XMLString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XMLUniDefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\TranscodingException.hpp
# End Source File
# End Group
# Begin Group "parsers Headers"

# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\parsers\DOMParser.hpp
# End Source File
# End Group
# Begin Group "dom Headers"

# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_Node.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_Element.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_Attr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_CharacterData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_Text.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_CDATASection.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_Comment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_DocumentType.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_DOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_DocumentFragment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_ProcessingInstruction.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_Entity.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_EntityReference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_NodeList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_Notation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_NodeFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_NodeIterator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_TreeWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_XMLDecl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_Range.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_Document.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_DOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM_NamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOM.hpp
# End Source File
# End Group
# Begin Group "framework Headers"

# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\framework\XMLFormatter.hpp
# End Source File
# End Group
# End Group
# End Group
# End Group
# Begin Group "Tools Headers"

# Begin Group "AudioFileIO Headers No. 1"

# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\SoundHeader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\SoundFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AIFFFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\WaveFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\extended.hxx
# End Source File
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "ThreadedProcessing - Win32 Release"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "ThreadedProcessing.dsp"
	srcdeps.exe settings.cfg ThreadedProcessing.dsp

# End Custom Build

!ELSEIF  "$(CFG)" == "ThreadedProcessing - Win32 Debug"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "ThreadedProcessing.dsp"
	srcdeps.exe settings.cfg ThreadedProcessing.dsp

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
